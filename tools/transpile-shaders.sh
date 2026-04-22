#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SHADER_DIR="$ROOT_DIR/src/platform/video/sdl_gpu/shaders"
COMPILED_DIR="$SHADER_DIR/compiled"
SHADERCROSS="$ROOT_DIR/third_party/SDL_shadercross/build/bin/shadercross"

if [ ! -x "$SHADERCROSS" ]; then
    echo "shadercross not found at $SHADERCROSS"
    echo "Run ./tools/build-shadercross.sh first."
    exit 1
fi

detect_stage() {
    local filename="$1"

    case "$filename" in
        *.vert.hlsl|vert.hlsl|vertex.hlsl)
            echo "vertex"
            ;;
        *.frag.hlsl|frag.hlsl|fragment.hlsl)
            echo "fragment"
            ;;
        *.comp.hlsl|comp.hlsl|compute.hlsl)
            echo "compute"
            ;;
        *)
            echo "Unable to infer shader stage from $filename" >&2
            echo "Expected names like vert.hlsl, *.frag.hlsl, or *.comp.hlsl." >&2
            return 1
            ;;
    esac
}

transpile_shader() {
    local input="$1"
    local dest="$2"
    local stage="$3"
    local output="$4"

    echo "Transpiling $(basename "$input") -> $(basename "$(dirname "$output")")/$(basename "$output")"

    "$SHADERCROSS" "$input" \
        --source HLSL \
        --dest "$dest" \
        --stage "$stage" \
        --entrypoint main \
        --output "$output"
}

mkdir -p \
    "$COMPILED_DIR/spirv" \
    "$COMPILED_DIR/msl"

shopt -s nullglob
hlsl_shaders=("$SHADER_DIR"/*.hlsl)

if [ "${#hlsl_shaders[@]}" -eq 0 ]; then
    echo "No HLSL shaders found in $SHADER_DIR"
    exit 0
fi

for shader in "${hlsl_shaders[@]}"; do
    filename="$(basename "$shader")"
    output_name="${filename%.hlsl}"
    stage="$(detect_stage "$filename")"

    transpile_shader "$shader" SPIRV "$stage" "$COMPILED_DIR/spirv/$output_name"
    transpile_shader "$shader" MSL "$stage" "$COMPILED_DIR/msl/$output_name"
done

echo "Compiled shaders written to $COMPILED_DIR"
