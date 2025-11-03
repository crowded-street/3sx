import sys
from pathlib import Path
from enum import Enum
from dataclasses import dataclass
import re
import subprocess

@dataclass
class Member:
    name: str
    type: int
    typename: str
    location: int

@dataclass
class Struct:
    name: str
    offset: int
    size: int
    members: list[Member]

class DWARFParser:
    class TypedefParserState(Enum):
        TYPEDEF = 0
        TYPE = 1
        NAME = 2

    class StructParserState(Enum):
        STRUCT = 0
        STRUCT_NAME = 1
        STRUCT_SIZE = 2
        MEMBER = 3
        MEMBER_NAME = 4
        MEMBER_TYPE = 5
        MEMBER_LOCATION = 6
        IGNORED_TYPE = 7

    def __init__(self):
        self.typedefs: dict[int, str] = {}
        self.structs: list[Struct] = []
        self.struct_name_to_struct: dict[str, Struct] = {}

    def parse_object(self, path: Path):
        lines = self.__run_dwarfdump(path).splitlines()
        self.__parse_typedefs(lines)
        self.__parse_structs(lines)

    def find_member(self, struct_name: str, offset: int) -> list[str]:
        path: list[str] = []
        struct = self.struct_name_to_struct.get(struct_name)

        while struct:
            if offset >= struct.size:
                print(f"⚠️ 0x{offset:X} does not fit in {struct.name} (size 0x{struct.size:X})")
                break

            member = [x for x in struct.members if offset >= x.location][-1]
            path_component = member.name
            offset -= member.location
            name, array_dimensions = self.__split_typename(member.typename)
            elem_size = 0

            if member.name == "frw":
                struct = self.struct_name_to_struct.get("WORK_Other")
                elem_size = 448 * 8
                array_dimensions = [128]
            else:
                struct = self.struct_name_to_struct.get(name)

                if struct:
                    elem_size = struct.size

            if array_dimensions and elem_size:
                indices = self.__offset_to_indices(offset, elem_size, array_dimensions)

                for index in indices:
                    path_component += f"[{index}]"

                flat_index = offset // elem_size
                offset -= flat_index * elem_size

            path.append(path_component)

        return path
    
    def __split_typename(self, typename: str) -> tuple[str, list[int]]:
        name_end: int | None = None
        dimensions: list[int] = []

        for match in re.finditer(r"\[(\d+)\]", typename):
            dimensions.append(int(match.group(1), base=16))

            if name_end == None:
                name_end = match.start()

        name = typename

        if name_end != None:
            name = typename[:name_end]

        return (name, dimensions)

    def __offset_to_indices(self, offset: int, elem_size: int, dims: list[int]) -> list[int]:
        flat_index = offset // elem_size
        indices = []

        for dim in reversed(dims):
            indices.append(flat_index % dim)
            flat_index //= dim

        return list(reversed(indices))

    def __run_dwarfdump(self, path: Path) -> str:
        result = subprocess.run(
            ["dwarfdump", path],
            capture_output=True,
            text=True
        )

        if result.returncode != 0:
            raise RuntimeError(f"dwarfdump failed: {result.stderr.strip()}")

        return result.stdout

    def __parse_typedefs(self, lines: list[str]):
        current_type: int | None = None
        state = self.TypedefParserState.TYPEDEF

        type_re = re.compile(r"DW_AT_type\t\((\w+)")
        name_re = re.compile(r"DW_AT_name\t\(\"(\w+)\"\)")

        for line in lines:
            match state:
                case self.TypedefParserState.TYPEDEF:
                    if line.endswith("DW_TAG_typedef"):
                        state = self.TypedefParserState.TYPE

                case self.TypedefParserState.TYPE:
                    if match := re.search(type_re, line):
                        current_type = int(match.group(1), base=16)
                        state = self.TypedefParserState.NAME

                case self.TypedefParserState.NAME:
                    if match := re.search(name_re, line):
                        self.typedefs[current_type] = match.group(1)
                        state = self.TypedefParserState.TYPEDEF

    def __parse_structs(self, lines: list[str]):
        state = self.StructParserState.STRUCT
        current_struct: Struct | None = None
        current_member: Member | None = None
        nested_type_level = 0
        return_to_state = self.StructParserState.STRUCT

        struct_re = re.compile(r"(\w+):\s+DW_TAG_structure_type")
        struct_size_re = re.compile(r"DW_AT_byte_size\s+\((\w+)\)")
        name_re = re.compile(r"DW_AT_name\t\(\"(\w+)\"\)")
        member_location_re = re.compile(r"DW_AT_data_member_location\s+\((\w+)\)")
        member_type_re = re.compile(r"DW_AT_type\s+\((\w+) \"(.+)\"\)")

        for line in lines:
            match state:
                case self.StructParserState.STRUCT:
                    if line.endswith("DW_TAG_union_type"):
                        nested_type_level = 1
                        return_to_state = self.StructParserState.STRUCT
                        state = self.StructParserState.IGNORED_TYPE
                        continue

                    if match := re.search(struct_re, line):
                        struct_offset = int(match.group(1), base=16)

                        current_struct = Struct(
                            name=self.typedefs.get(struct_offset, ""),
                            offset=struct_offset,
                            size=0,
                            members=[]
                        )

                        if current_struct.name != "":
                            state = self.StructParserState.STRUCT_SIZE
                        else:
                            state = self.StructParserState.STRUCT_NAME

                case self.StructParserState.STRUCT_NAME:
                    if match := re.search(name_re, line):
                        current_struct.name = match.group(1)
                        state = self.StructParserState.STRUCT_SIZE

                case self.StructParserState.STRUCT_SIZE:
                    if match := re.search(struct_size_re, line):
                        current_struct.size = int(match.group(1), base=16)
                        state = self.StructParserState.MEMBER

                case self.StructParserState.MEMBER:
                    if line.endswith("NULL"):
                        assert(current_struct.name != "")
                        self.structs.append(current_struct)
                        state = self.StructParserState.STRUCT
                        continue

                    if line.endswith("DW_TAG_structure_type") or line.endswith("DW_TAG_union_type"):
                        nested_type_level = 1
                        return_to_state = self.StructParserState.MEMBER
                        state = self.StructParserState.IGNORED_TYPE
                        continue

                    if line.endswith("DW_TAG_member"):
                        current_member = Member("", 0, "", 0)
                        state = self.StructParserState.MEMBER_NAME

                case self.StructParserState.MEMBER_NAME:
                    if match := re.search(name_re, line):
                        current_member.name = match.group(1)
                        state = self.StructParserState.MEMBER_TYPE

                case self.StructParserState.MEMBER_TYPE:
                    if match := re.search(member_type_re, line):
                        current_member.type = int(match.group(1), base=16)
                        current_member.typename = match.group(2)
                        state = self.StructParserState.MEMBER_LOCATION

                case self.StructParserState.MEMBER_LOCATION:
                    if match := re.search(member_location_re, line):
                        assert(current_member.type != 0)
                        assert(current_member.name != "")
                        current_member.location = int(match.group(1), base=16)
                        current_struct.members.append(current_member)
                        state = self.StructParserState.MEMBER

                case self.StructParserState.IGNORED_TYPE:
                    if line.endswith("DW_TAG_structure_type") or line.endswith("DW_TAG_union_type"):
                        nested_type_level += 1
                        continue

                    if line.endswith("NULL"):
                        nested_type_level -= 1

                        if nested_type_level == 0:
                            state = return_to_state

        for struct in self.structs:
            self.struct_name_to_struct[struct.name] = struct

def find_state_pairs() -> list[tuple[Path, Path, int]]:
    pairs: list[tuple[Path, Path]] = []
    files = sorted(Path("states").iterdir(), key=lambda x: x.name)

    for file in files:
        plnum, frame = file.name.split("_")

        if plnum == "1":
            continue

        file1 = Path(f"states/0_{frame}")
        file2 = Path(f"states/1_{frame}")

        if not file2.exists():
            continue

        pairs.append((file1, file2, int(frame)))

    return pairs

def compare_states(parser: DWARFParser):
    pairs = find_state_pairs()

    for pl1_state_path, pl2_state_path, frame in pairs:
        pl1_state = pl1_state_path.read_bytes()
        pl2_state = pl2_state_path.read_bytes()

        length = min(len(pl1_state), len(pl2_state))

        for i in range(length):
            byte1 = pl1_state[i]
            byte2 = pl2_state[i]

            if byte1 != byte2:
                path = ".".join(parser.find_member("State", i))
                print(f"{frame}: mismatch at byte 0x{i:X} (0x{byte1:X} vs 0x{byte2:X}). Path: {path}")

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 compare_states.py <path_to_obj_file>")

    parser = DWARFParser()
    parser.parse_object(Path(sys.argv[1]))
    compare_states(parser)

if __name__ == "__main__":
    main()
