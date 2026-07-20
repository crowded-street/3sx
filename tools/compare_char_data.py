import sys
import os
import struct
import re
from pathlib import Path
from dataclasses import dataclass

# A tool for comparing CharInitData between the arcade version and the port

cps3_plid_data = (6, 3, 5, 1, 2, 9, 7, 4, 10, 8, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22)
cps3_vram_offset = 0x6000000
cps3_chdata_array_offset = 0x18B148

gill = 0
alex = 1
ryu = 2
yun = 3
dudley = 4
necro = 5
hugo = 6
ibuki = 7
elena = 8
oro = 9
yang = 10
ken = 11
sean = 12
urien = 13
akuma = 14
chunli = 15
makoto = 16
q = 17
twelve = 18
remy = 19

char_name_to_id = {
    "gill": gill,
    "alex": alex,
    "ryu": ryu,
    "yun": yun,
    "dudley": dudley,
    "necro": necro,
    "hugo": hugo,
    "ibuki": ibuki,
    "elena": elena,
    "oro": oro,
    "yang": yang,
    "ken": ken,
    "sean": sean,
    "urien": urien,
    "akuma": akuma,
    "chunli": chunli,
    "makoto": makoto,
    "q": q,
    "twelve": twelve,
    "remy": remy
}

chdata_offsets = {
    gill: 0x2CF158,
    alex: 0x24EDE0,
    ryu: 0x178F38,
    yun: 0x1FAA0C,
    dudley: 0x1E1D54,
    necro: 0x2BCE28,
    hugo: 0x31392C,
    ibuki: 0x25D908,
    elena: 0x1A0CCC,
    oro: 0x205694,
    yang: 0x216E64,
    ken: 0x17C0F8,
    sean: 0x18B378,
    urien: 0x215C68,
    akuma: 0x26DFC8,
    chunli: 0x26C73C,
    makoto: 0x2837C4,
    q: 0x37F0B4,
    twelve: 0x21B47C,
    remy: 0x1A4F50
}

chdata_structs = {
    "nmca": "I",
    "dmca": "I",
    "btca": "I",
    "caca": "I",
    "cuca": "I",
    "atca": "I",
    "saca": "I",
    "exca": "I",
    "cbca": "I",
    "yuca": "I",
    "stxy": "h",
    "mvxy": "h",
    "sernd": "I",
    "ovct": "hhBBBBBBhHH",
    "ovix": "hhhh",
    "rict": "hhBBh",
    "hiit": "HHHHHHHH",
    "boda": "hhhhhhhhhhhhhhhh",
    "hana": "hhhhhhhhhhhhhhhh",
    "cata": "hhhh",
    "caua": "hhhh",
    "atta": "hhhhhhhhhhhhhhhh",
    "hosa": "hhhh",
    "atit": "BBBBBBBBBBBBbbBB",
    "prot": "hhhhhhhhhhhhhhhhhhhhhhhh"
}

chdata_fields = tuple(x for x in chdata_structs)

@dataclass
class PortCharDataArrayInfo:
    offset: int
    elem_size: int
    length: int

def colored(obj, color: str) -> str:
    colors = {"red": 31, "green": 32, "white": 37}

    if color not in colors:
        color = "white"

    color_code = colors[color]
    prefix = f"\033[{color_code}m"
    suffix = "\033[0m"

    return f"{prefix}{str(obj)}{suffix}"

def character_to_plnum(character: int) -> int:
    # It seems Capcom wanted to add Shin Akuma as a separate character and some of his
    # data still remains in the arcade rom. He must've had his own id (15) in the arcade
    # version. But ports remove him entirely, which is why we have to adjust indices accordingly.
    if character > akuma:
        character += 1

    return character

def bin_name(character: int) -> Path:
    pl_number = character_to_plnum(character)
    return f"pl{pl_number:02}.bin"

def calculate_array_info(afs_path: Path, character: int) -> dict[str, PortCharDataArrayInfo]:
    result: dict[str, PortCharDataArrayInfo] = dict()
    chdata_offset = chdata_offsets[character]
    
    with open(afs_path / bin_name(character), "rb") as f:
        f.seek(chdata_offset)

        array_offsets = list()

        for i in range(25):
            offset = int.from_bytes(f.read(4), "little")
            array_offsets.append(chdata_offset + offset)

        f.seek(0, os.SEEK_END)
        end_offset = f.tell()
        data = tuple(sorted(zip(chdata_fields, array_offsets), key=lambda x: x[1]))

        for i in range(25):
            field = data[i][0]
            current_offset = data[i][1]
            next_offset = 0

            if i < 24:
                next_offset = data[i + 1][1]
            else:
                next_offset = end_offset

            diff = next_offset - current_offset
            field_size = struct.calcsize(chdata_structs[field])
            length = diff // field_size

            array_info = PortCharDataArrayInfo(
                offset=current_offset, 
                elem_size=field_size,
                length=length
            )

            result[field] = array_info

    return result

def calculate_cps3_array_offsets(rom_path: Path, character: int) -> dict[str, int]:
    result: dict[str, int] = dict()
    pl_num = character_to_plnum(character)
    chdata_offset = cps3_chdata_array_offset + 0x6C * cps3_plid_data[pl_num] # 0x6C is the size of chdata in CPS3 version

    with open(rom_path, "rb") as f:
        f.seek(chdata_offset)
        
        for field in chdata_fields:
            result[field] = int.from_bytes(f.read(4), "big") - cps3_vram_offset

    return result

def calculate_cps3_array_lengths() -> list[dict[str, int]]:
    source_path = Path(__file__).parents[1] / "src/arcade/arcade_char_data.c"
    source = source_path.read_text()
    table = source.split("static const LocationData location_data[NUM_CHARS] = {", 1)[1]
    entries = re.findall(
        r"\.(\w+)\s*=\s*\{\s*\.offset\s*=\s*0x[0-9A-Fa-f]+,\s*\.size\s*=\s*0x([0-9A-Fa-f]+)\s*\}",
        table,
    )
    expected_count = len(char_name_to_id) * len(chdata_fields)

    if len(entries) != expected_count:
        raise ValueError(f"Expected {expected_count} arcade table locations, found {len(entries)}")

    result = []

    for character in range(len(char_name_to_id)):
        lengths = {}
        character_entries = entries[character * len(chdata_fields):(character + 1) * len(chdata_fields)]

        for expected_field, (field, size_hex) in zip(chdata_fields, character_entries):
            if field != expected_field:
                raise ValueError(f"Arcade table order changed: expected {expected_field}, found {field}")

            elem_size = struct.calcsize(chdata_structs[field])
            size = int(size_hex, 16)

            lengths[field] = size // elem_size

        result.append(lengths)

    return result

def read_array(path: Path, field: str, offset: int, length: int, little_endian: bool) -> list:
    result = list()
    endianness_specifier = "<" if little_endian else ">"
    format = endianness_specifier + chdata_structs[field]
    elem_size = struct.calcsize(format)

    with open(path, "rb") as f:
        f.seek(offset)

        for i in range(length):
            chunk = f.read(elem_size)

            if len(chunk) != elem_size:
                raise ValueError(f"Unexpected end of {path} while reading {field}[{i}]")

            result.append(struct.unpack(format, chunk))

    return result

def analyze_and_print(rom_path: Path, afs_path: Path, character: int, field: str):
    array_info = calculate_array_info(afs_path, character)
    cps3_array_offsets = calculate_cps3_array_offsets(rom_path, character)
    cps3_array_lengths = calculate_cps3_array_lengths()

    # Parse values

    length = min(array_info[field].length, cps3_array_lengths[character][field])
    bin_path = afs_path / bin_name(character)

    port_values = read_array(
        path=bin_path, 
        field=field, 
        offset=array_info[field].offset,
        length=length,
        little_endian=True
    )

    cps3_values = read_array(
        path=rom_path,
        field=field,
        offset=cps3_array_offsets[field],
        length=length,
        little_endian=False
    )

    # Print

    mismatch_count = 0

    for index, cps3_value, port_value in zip(range(length), cps3_values, port_values):
        matching = port_value == cps3_value

        if matching:
            continue

        mismatch_count += 1

        cps3_value_string_parts = []
        port_value_string_parts = []

        for cps3_part, port_part in zip(cps3_value, port_value):
            if cps3_part == port_part:
                cps3_value_string_parts.append(str(cps3_part))
                port_value_string_parts.append(str(port_part))
            else:
                cps3_value_string_parts.append(colored(cps3_part, "green"))
                port_value_string_parts.append(colored(port_part, "red"))

        cps3_value_string = f"({", ".join(cps3_value_string_parts)})"
        port_value_string = f"({", ".join(port_value_string_parts)})"

        print(f"0x{index:X}:\t{cps3_value_string}\t{port_value_string}")

    if mismatch_count > 0:
        print(f"Non-matching: {mismatch_count}")
    else:
        print("All elements match ✅")

def analyze_rendering_bindings(rom_path: Path, afs_path: Path, character: int) -> bool:
    field = "ovct"
    array_info = calculate_array_info(afs_path, character)[field]
    cps3_offset = calculate_cps3_array_offsets(rom_path, character)[field]
    cps3_length = calculate_cps3_array_lengths()[character][field]
    common_length = min(array_info.length, cps3_length)
    port_values = read_array(afs_path / bin_name(character), field, array_info.offset, common_length, True)
    cps3_values = read_array(rom_path, field, cps3_offset, common_length, False)
    binding_fields = {3: "palette", 8: "texture mode", 10: "CG handle"}
    binding_mismatches = {name: 0 for name in binding_fields.values()}
    behavior_mismatches = 0

    for cps3_value, port_value in zip(cps3_values, port_values):
        for index, (cps3_part, port_part) in enumerate(zip(cps3_value, port_value)):
            if cps3_part == port_part:
                continue

            if index in binding_fields:
                binding_mismatches[binding_fields[index]] += 1
            else:
                behavior_mismatches += 1

    name = next(name for name, value in char_name_to_id.items() if value == character)
    bindings = ", ".join(f"{key}={value}" for key, value in binding_mismatches.items())
    print(f"{name}: arcade={cps3_length}, port={array_info.length}, common={common_length}; {bindings}; behavior={behavior_mismatches}")
    return behavior_mismatches == 0

def main():
    if len(sys.argv) < 5:
        print("Incorrect number of arguments")
        print("Usage: python3 compare_char_data.py <decrypted rom path> <unpacked afs path> <character> <field|rendering>")
        return
    
    rom_path = Path(sys.argv[1])
    afs_path = Path(sys.argv[2])
    char_name = sys.argv[3]
    field = sys.argv[4]
    
    if field == "rendering":
        characters = char_name_to_id.values() if char_name == "all" else (char_name_to_id[char_name],)
        results = [analyze_rendering_bindings(rom_path, afs_path, character) for character in characters]

        if not all(results):
            raise SystemExit(1)
    elif char_name == "all":
        is_first = True

        for name in char_name_to_id.keys():
            if not is_first:
                print()

            is_first = False
            print(name + ":")
            character = char_name_to_id[name]
            analyze_and_print(rom_path, afs_path, character, field)
    else:
        character = char_name_to_id[char_name]
        analyze_and_print(rom_path, afs_path, character, field)

if __name__ == "__main__":
    main()
