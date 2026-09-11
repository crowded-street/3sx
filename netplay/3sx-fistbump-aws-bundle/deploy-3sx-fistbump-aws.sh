#!/usr/bin/env bash
set -euo pipefail

# Deploy the 3SX-compatible Fistbump coordination server to AWS Lightsail.
#
# Cost target: Lightsail Nano with public IPv4 (bundle nano_3_0).
# 3SX uses TCP 9000 for control/matchmaking and UDP 9001 for NAT endpoint discovery.
#
# Usage:
#   ./deploy-3sx-fistbump-aws.sh deploy
#   ./deploy-3sx-fistbump-aws.sh update
#   ./deploy-3sx-fistbump-aws.sh smoke
#   ./deploy-3sx-fistbump-aws.sh status
#   ./deploy-3sx-fistbump-aws.sh logs
#   ./deploy-3sx-fistbump-aws.sh destroy
#
# Optional environment variables:
#   AWS_REGION=us-east-2
#   INSTANCE_NAME=3sx-fistbump
#   STATIC_IP_NAME=3sx-fistbump-ip
#   BUNDLE_ID=nano_3_0
#   BLUEPRINT_ID=ubuntu_24_04
#   BINARY=/path/to/3sx-fistbump-server-linux-amd64
#   SSH_CIDR=203.0.113.25/32
#   SSH_KEY_FILE=/path/to/existing-key.pem
#   SSH_HOST=3.16.58.140
#
# Default SSH connection: ssh -i ~/.ssh/3sx-fistbump ubuntu@3.16.58.140

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
AWS_REGION="${AWS_REGION:-us-east-2}"
INSTANCE_NAME="${INSTANCE_NAME:-3sx-fistbump}"
STATIC_IP_NAME="${STATIC_IP_NAME:-3sx-fistbump-ip}"
KEY_PAIR_NAME="${KEY_PAIR_NAME:-3sx-fistbump-key}"
BUNDLE_ID="${BUNDLE_ID:-nano_3_0}"
BLUEPRINT_ID="${BLUEPRINT_ID:-ubuntu_24_04}"
TCP_PORT="${TCP_PORT:-9000}"
UDP_PORT="${UDP_PORT:-9001}"
STATE_DIR="${STATE_DIR:-${HOME}/.3sx-fistbump-aws}"
DEFAULT_KEY_FILE="${STATE_DIR}/${KEY_PAIR_NAME}.pem"
KEY_FILE="${SSH_KEY_FILE:-${HOME}/.ssh/3sx-fistbump}"
SSH_HOST="${SSH_HOST-3.16.58.140}"
BINARY="${BINARY:-${SCRIPT_DIR}/3sx-fistbump-server-linux-amd64}"
SSH_USER="ubuntu"
SSH_OPTS=(-o IdentitiesOnly=yes -o BatchMode=yes -o StrictHostKeyChecking=accept-new -o ConnectTimeout=8 -o ServerAliveInterval=10)

AWS=(aws --no-cli-pager --region "$AWS_REGION")

say() { printf '\n==> %s\n' "$*"; }
die() { printf 'ERROR: %s\n' "$*" >&2; exit 1; }
need() { command -v "$1" >/dev/null 2>&1 || die "Missing required command: $1"; }

require_aws() {
  need aws
  "${AWS[@]}" sts get-caller-identity >/dev/null
}

instance_exists() {
  "${AWS[@]}" lightsail get-instance --instance-name "$INSTANCE_NAME" >/dev/null 2>&1
}

static_ip_exists() {
  "${AWS[@]}" lightsail get-static-ip --static-ip-name "$STATIC_IP_NAME" >/dev/null 2>&1
}

key_pair_exists() {
  "${AWS[@]}" lightsail get-key-pair --key-pair-name "$KEY_PAIR_NAME" >/dev/null 2>&1
}

get_public_ip() {
  if [[ -n "$SSH_HOST" ]]; then
    printf '%s\n' "$SSH_HOST"
    return
  fi
  if static_ip_exists; then
    "${AWS[@]}" lightsail get-static-ip \
      --static-ip-name "$STATIC_IP_NAME" \
      --query 'staticIp.ipAddress' --output text
  else
    "${AWS[@]}" lightsail get-instance \
      --instance-name "$INSTANCE_NAME" \
      --query 'instance.publicIpAddress' --output text
  fi
}

get_ssh_cidr() {
  if [[ -n "${SSH_CIDR:-}" ]]; then
    printf '%s\n' "$SSH_CIDR"
    return
  fi
  need curl
  local ip
  ip="$(curl -fsS --max-time 10 https://checkip.amazonaws.com | tr -d '[:space:]')"
  [[ "$ip" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]] || \
    die "Could not determine your public IPv4 address. Set SSH_CIDR=x.x.x.x/32."
  printf '%s/32\n' "$ip"
}

base64_decode() {
  if base64 --help 2>&1 | grep -q -- '--decode'; then
    base64 --decode
  else
    base64 -D
  fi
}

create_key_pair() {
  mkdir -p "$STATE_DIR"
  chmod 700 "$STATE_DIR"

  if [[ -s "$KEY_FILE" ]]; then
    chmod 600 "$KEY_FILE"
    return
  fi

  if key_pair_exists; then
    die "Lightsail key pair '$KEY_PAIR_NAME' already exists, but $KEY_FILE is missing. Set SSH_KEY_FILE to its private key or choose a new KEY_PAIR_NAME."
  fi

  say "Creating Lightsail SSH key pair"
  local private_b64
  private_b64="$("${AWS[@]}" lightsail create-key-pair \
    --key-pair-name "$KEY_PAIR_NAME" \
    --query privateKeyBase64 --output text)"
  umask 077
  printf '%s' "$private_b64" | base64_decode > "$KEY_FILE"
  chmod 600 "$KEY_FILE"
}

select_blueprint() {
  local selected
  selected="$("${AWS[@]}" lightsail get-blueprints \
    --query "blueprints[?blueprintId=='${BLUEPRINT_ID}' && isActive==\`true\`].blueprintId | [0]" \
    --output text 2>/dev/null || true)"
  if [[ -n "$selected" && "$selected" != "None" ]]; then
    printf '%s\n' "$selected"
    return
  fi

  selected="$("${AWS[@]}" lightsail get-blueprints \
    --query "blueprints[?isActive==\`true\` && type=='os' && contains(blueprintId, 'ubuntu')].blueprintId | [0]" \
    --output text)"
  [[ -n "$selected" && "$selected" != "None" ]] || die "No active Ubuntu Lightsail blueprint found in $AWS_REGION"
  printf '%s\n' "$selected"
}

select_az() {
  local az
  az="$("${AWS[@]}" lightsail get-regions --include-availability-zones \
    --query "regions[?name=='${AWS_REGION}'].availabilityZones[0].zoneName | [0]" \
    --output text 2>/dev/null || true)"
  if [[ -z "$az" || "$az" == "None" ]]; then
    az="${AWS_REGION}a"
  fi
  printf '%s\n' "$az"
}

validate_bundle() {
  local active price
  active="$("${AWS[@]}" lightsail get-bundles \
    --query "bundles[?bundleId=='${BUNDLE_ID}' && isActive==\`true\`].bundleId | [0]" \
    --output text)"
  [[ -n "$active" && "$active" != "None" ]] || die "Lightsail bundle '$BUNDLE_ID' is not active in $AWS_REGION"
  price="$("${AWS[@]}" lightsail get-bundles \
    --query "bundles[?bundleId=='${BUNDLE_ID}'].price | [0]" --output text)"
  say "Using Lightsail bundle $BUNDLE_ID (reported price: \$$price/month)"
}

create_instance() {
  create_key_pair
  validate_bundle
  local blueprint az
  blueprint="$(select_blueprint)"
  az="$(select_az)"

  say "Creating $INSTANCE_NAME in $az with $blueprint"
  "${AWS[@]}" lightsail create-instances \
    --instance-names "$INSTANCE_NAME" \
    --availability-zone "$az" \
    --blueprint-id "$blueprint" \
    --bundle-id "$BUNDLE_ID" \
    --key-pair-name "$KEY_PAIR_NAME" \
    --ip-address-type ipv4 \
    --tags key=app,value=3sx-fistbump key=protocol,value=fistbump >/dev/null

  say "Waiting for Lightsail instance to enter running state"
  local i state
  for i in $(seq 1 60); do
    state="$("${AWS[@]}" lightsail get-instance --instance-name "$INSTANCE_NAME" --query 'instance.state.name' --output text 2>/dev/null || true)"
    [[ "$state" == "running" ]] && return
    sleep 5
  done
  die "Instance did not reach running state"
}

ensure_static_ip() {
  if ! static_ip_exists; then
    say "Allocating attached Lightsail static IPv4"
    "${AWS[@]}" lightsail allocate-static-ip --static-ip-name "$STATIC_IP_NAME" >/dev/null
  fi

  local attached
  attached="$("${AWS[@]}" lightsail get-static-ip --static-ip-name "$STATIC_IP_NAME" --query 'staticIp.attachedTo' --output text)"
  if [[ "$attached" != "None" && -n "$attached" && "$attached" != "$INSTANCE_NAME" ]]; then
    die "Static IP '$STATIC_IP_NAME' is already attached to '$attached'"
  fi
  if [[ "$attached" != "$INSTANCE_NAME" ]]; then
    "${AWS[@]}" lightsail attach-static-ip \
      --static-ip-name "$STATIC_IP_NAME" \
      --instance-name "$INSTANCE_NAME" >/dev/null
  fi
}

open_deploy_firewall() {
  local cidr="$1"
  say "Opening TCP $TCP_PORT + UDP $UDP_PORT publicly; SSH only from $cidr"
  "${AWS[@]}" lightsail put-instance-public-ports \
    --instance-name "$INSTANCE_NAME" \
    --port-infos \
      "fromPort=${TCP_PORT},toPort=${TCP_PORT},protocol=tcp,cidrs=0.0.0.0/0" \
      "fromPort=${UDP_PORT},toPort=${UDP_PORT},protocol=udp,cidrs=0.0.0.0/0" \
      "fromPort=22,toPort=22,protocol=tcp,cidrs=${cidr}" >/dev/null
}

close_ssh_firewall() {
  say "Closing SSH; leaving only 3SX Fistbump ports open"
  "${AWS[@]}" lightsail put-instance-public-ports \
    --instance-name "$INSTANCE_NAME" \
    --port-infos \
      "fromPort=${TCP_PORT},toPort=${TCP_PORT},protocol=tcp,cidrs=0.0.0.0/0" \
      "fromPort=${UDP_PORT},toPort=${UDP_PORT},protocol=udp,cidrs=0.0.0.0/0" >/dev/null
}

wait_for_ssh() {
  local ip="$1"
  say "Waiting for SSH on $ip"
  for _ in $(seq 1 60); do
    if ssh "${SSH_OPTS[@]}" -i "$KEY_FILE" "$SSH_USER@$ip" true >/dev/null 2>&1; then
      return
    fi
    sleep 5
  done
  die "SSH did not become available at $ip"
}

install_binary() {
  local ip="$1"
  [[ -f "$BINARY" ]] || die "Binary not found: $BINARY"
  [[ -s "$KEY_FILE" ]] || die "SSH private key not found: $KEY_FILE"
  need ssh
  need scp

  say "Uploading $(basename "$BINARY")"
  scp "${SSH_OPTS[@]}" -i "$KEY_FILE" "$BINARY" "$SSH_USER@$ip:/tmp/3sx-fistbump-server"

  say "Installing hardened systemd service"
  ssh "${SSH_OPTS[@]}" -i "$KEY_FILE" "$SSH_USER@$ip" \
    "TCP_PORT='$TCP_PORT' UDP_PORT='$UDP_PORT' bash -s" <<'REMOTE'
set -euo pipefail
sudo install -o root -g root -m 0755 /tmp/3sx-fistbump-server /usr/local/bin/3sx-fistbump-server
rm -f /tmp/3sx-fistbump-server

sudo tee /etc/systemd/system/3sx-fistbump.service >/dev/null <<UNIT
[Unit]
Description=3SX Fistbump coordination server
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
ExecStart=/usr/local/bin/3sx-fistbump-server -tcp :${TCP_PORT} -udp :${UDP_PORT}
Restart=on-failure
RestartSec=2
DynamicUser=yes
NoNewPrivileges=yes
PrivateTmp=yes
PrivateDevices=yes
ProtectSystem=strict
ProtectHome=yes
ProtectKernelTunables=yes
ProtectKernelModules=yes
ProtectControlGroups=yes
RestrictAddressFamilies=AF_INET AF_UNIX
LockPersonality=yes
MemoryDenyWriteExecute=yes
CapabilityBoundingSet=
AmbientCapabilities=
LimitNOFILE=4096

[Install]
WantedBy=multi-user.target
UNIT

sudo systemctl daemon-reload
sudo systemctl enable --now 3sx-fistbump.service
sudo systemctl restart 3sx-fistbump.service
sleep 1
sudo systemctl --no-pager --full status 3sx-fistbump.service
REMOTE
}

with_temporary_ssh() {
  local callback="$1"
  local cidr ip
  cidr="$(get_ssh_cidr)"
  ip="$(get_public_ip)"
  open_deploy_firewall "$cidr"
  wait_for_ssh "$ip"
  "$callback" "$ip"
  close_ssh_firewall
}

do_deploy() {
  require_aws
  need ssh
  need scp
  if ! instance_exists; then
    create_instance
  else
    [[ -s "$KEY_FILE" ]] || die "Existing instance found, but SSH key is missing: $KEY_FILE (or set SSH_KEY_FILE)"
    say "Reusing existing Lightsail instance $INSTANCE_NAME"
  fi
  ensure_static_ip
  with_temporary_ssh install_binary
  do_status
  do_smoke
}

do_update() {
  require_aws
  instance_exists || die "Instance '$INSTANCE_NAME' does not exist. Run deploy first."
  [[ -s "$BINARY" ]] || die "Binary not found: $BINARY"
  [[ -s "$KEY_FILE" ]] || die "SSH key not found: $KEY_FILE (or set SSH_KEY_FILE)"
  with_temporary_ssh install_binary
  do_smoke
}

do_status() {
  require_aws
  instance_exists || die "Instance '$INSTANCE_NAME' does not exist"
  local state ip price
  state="$("${AWS[@]}" lightsail get-instance --instance-name "$INSTANCE_NAME" --query 'instance.state.name' --output text)"
  ip="$(get_public_ip)"
  price="$("${AWS[@]}" lightsail get-bundles --query "bundles[?bundleId=='${BUNDLE_ID}'].price | [0]" --output text 2>/dev/null || echo '?')"
  cat <<STATUS

3SX Fistbump AWS status
  Instance:    $INSTANCE_NAME
  State:       $state
  Region:      $AWS_REGION
  Public IPv4: $ip
  TCP control: $ip:$TCP_PORT
  UDP NAT:     $ip:$UDP_PORT
  Bundle:      $BUNDLE_ID (~\$$price/month as reported by Lightsail)

Run 3SX Debug builds with:
  ./3sx --matchmaking-ip $ip --matchmaking-port $TCP_PORT
STATUS
}

do_smoke() {
  require_aws
  instance_exists || die "Instance '$INSTANCE_NAME' does not exist"
  need python3
  local ip
  ip="$(get_public_ip)"
  say "Protocol smoke test against $ip:$TCP_PORT"
  python3 - "$ip" "$TCP_PORT" <<'PY'
import socket, sys
host, port = sys.argv[1], int(sys.argv[2])
s = socket.create_connection((host, port), timeout=5)
f = s.makefile("rwb", buffering=0)
def recvline():
    line = f.readline().decode("utf-8", "replace").strip()
    if not line:
        raise SystemExit("server closed connection")
    print("server:", line)
    return line
session = recvline()
assert session.startswith("SESSION "), session
print("client: HELLO")
f.write(b"HELLO\n")
token = recvline()
profile = recvline()
assert token.startswith("TOKEN refresh "), token
assert profile.startswith("PROFILE "), profile
print("PASS: 3SX login/control handshake looks compatible")
s.close()
PY
}

do_logs() {
  require_aws
  instance_exists || die "Instance '$INSTANCE_NAME' does not exist"
  [[ -s "$KEY_FILE" ]] || die "SSH key not found: $KEY_FILE (or set SSH_KEY_FILE)"
  local cidr ip
  cidr="$(get_ssh_cidr)"
  ip="$(get_public_ip)"
  open_deploy_firewall "$cidr"
  wait_for_ssh "$ip"
  ssh "${SSH_OPTS[@]}" -i "$KEY_FILE" "$SSH_USER@$ip" \
    'sudo journalctl -u 3sx-fistbump.service -n 150 --no-pager'
  close_ssh_firewall
}

do_destroy() {
  require_aws
  if [[ -t 0 && "${CONFIRM_DESTROY:-}" != "yes" ]]; then
    printf "Destroy Lightsail instance '%s' and release its static IP? [y/N] " "$INSTANCE_NAME"
    read -r answer
    [[ "$answer" == "y" || "$answer" == "Y" ]] || exit 0
  elif [[ ! -t 0 && "${CONFIRM_DESTROY:-}" != "yes" ]]; then
    die "Non-interactive destroy requires CONFIRM_DESTROY=yes"
  fi

  if instance_exists; then
    say "Deleting Lightsail instance $INSTANCE_NAME"
    "${AWS[@]}" lightsail delete-instance --instance-name "$INSTANCE_NAME" >/dev/null
  fi
  if static_ip_exists; then
    say "Releasing static IP $STATIC_IP_NAME"
    "${AWS[@]}" lightsail release-static-ip --static-ip-name "$STATIC_IP_NAME" >/dev/null || true
  fi
  if key_pair_exists; then
    say "Deleting Lightsail key pair $KEY_PAIR_NAME"
    "${AWS[@]}" lightsail delete-key-pair --key-pair-name "$KEY_PAIR_NAME" >/dev/null || true
  fi
  if [[ "$KEY_FILE" == "$DEFAULT_KEY_FILE" ]]; then
    rm -f "$KEY_FILE"
  fi
  say "Destroyed. Recurring Lightsail instance cost is removed."
}

usage() {
  cat <<'USAGE'
Deploy the 3SX-compatible Fistbump coordination server to AWS Lightsail.

Usage:
  ./deploy-3sx-fistbump-aws.sh deploy
  ./deploy-3sx-fistbump-aws.sh update
  ./deploy-3sx-fistbump-aws.sh smoke
  ./deploy-3sx-fistbump-aws.sh status
  ./deploy-3sx-fistbump-aws.sh logs
  ./deploy-3sx-fistbump-aws.sh destroy

Defaults:
  AWS_REGION=us-east-2
  BUNDLE_ID=nano_3_0
  TCP_PORT=9000
  UDP_PORT=9001
  SSH_USER=ubuntu
  KEY_PAIR_NAME=3sx-fistbump-key
  SSH_KEY_FILE=~/.ssh/3sx-fistbump
  SSH_HOST=3.16.58.140

Set BINARY, SSH_CIDR, SSH_KEY_FILE, SSH_HOST, INSTANCE_NAME, etc. with environment variables if needed.
Set SSH_HOST='' to discover the instance address through AWS instead.
USAGE
}

cmd="${1:-}"
case "$cmd" in
  deploy)  do_deploy ;;
  update)  do_update ;;
  smoke)   do_smoke ;;
  status)  do_status ;;
  logs)    do_logs ;;
  destroy) do_destroy ;;
  -h|--help|help|"") usage ;;
  *) die "Unknown command '$cmd'. Use: deploy, update, smoke, status, logs, destroy" ;;
esac
