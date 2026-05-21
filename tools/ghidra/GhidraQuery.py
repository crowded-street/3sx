# Emits selected Ghidra program information as JSON for repo-side tooling.
# @category 3sx

from ghidra.app.decompiler import DecompInterface, DecompileOptions
from ghidra.program.model.mem import MemoryAccessException
from ghidra.util.task import TaskMonitor

import json


DEFAULT_LIMIT = 25


def parse_args(args):
    query = {
        "target": None,
        "search": None,
        "out": None,
        "decompile": False,
        "bytes": 0,
        "limit": DEFAULT_LIMIT,
    }
    i = 0
    while i < len(args):
        arg = args[i]
        if arg == "--target":
            i += 1
            query["target"] = args[i]
        elif arg == "--search":
            i += 1
            query["search"] = args[i]
        elif arg == "--out":
            i += 1
            query["out"] = args[i]
        elif arg == "--decompile":
            query["decompile"] = True
        elif arg == "--bytes":
            i += 1
            query["bytes"] = int(args[i])
        elif arg == "--limit":
            i += 1
            query["limit"] = int(args[i])
        else:
            raise Exception("Unknown argument: %s" % arg)
        i += 1
    return query


def addr_to_string(address):
    if address is None:
        return None
    return str(address)


def resolve_address(value):
    try:
        normalized = value
        if normalized.startswith("0x") or normalized.startswith("0X"):
            normalized = normalized[2:]
        return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(normalized)
    except Exception:
        return None


def get_symbol(name):
    symbols = currentProgram.getSymbolTable().getLabelOrFunctionSymbols(name, None)
    if symbols:
        return symbols[0]

    symbol_iter = currentProgram.getSymbolTable().getAllSymbols(True)
    while symbol_iter.hasNext():
        symbol = symbol_iter.next()
        if symbol.getName(True) == name or symbol.getName() == name:
            return symbol
    return None


def decompile_function(function):
    decompiler = DecompInterface()
    decompiler.setOptions(DecompileOptions())
    decompiler.openProgram(currentProgram)
    try:
        results = decompiler.decompileFunction(function, 30, TaskMonitor.DUMMY)
        if not results.decompileCompleted():
            return "DECOMPILATION_FAILED: %s" % results.getErrorMessage()
        return results.getDecompiledFunction().getC()
    finally:
        decompiler.dispose()


def function_json(function, include_decompile):
    result = {
        "name": function.getName(True),
        "entry": str(function.getEntryPoint()),
        "signature": function.getSignature().getPrototypeString(),
        "bodyMin": str(function.getBody().getMinAddress()),
        "bodyMax": str(function.getBody().getMaxAddress()),
    }
    if include_decompile:
        result["decompile"] = decompile_function(function)
    return result


def refs_json(ref_iter, limit):
    refs = []
    if hasattr(ref_iter, "hasNext"):
        iterator = iter(lambda: ref_iter.next() if ref_iter.hasNext() else None, None)
    else:
        iterator = iter(ref_iter)

    for ref in iterator:
        if len(refs) >= limit:
            break
        item = {
            "from": str(ref.getFromAddress()),
            "to": str(ref.getToAddress()),
            "type": str(ref.getReferenceType()),
        }
        from_function = getFunctionContaining(ref.getFromAddress())
        if from_function is not None:
            item["fromFunction"] = from_function.getName(True)
        refs.append(item)
    refs.sort(key=lambda item: item["from"])
    return refs


def bytes_json(address, byte_count):
    if byte_count <= 0:
        return None
    buf = bytearray(byte_count)
    try:
        read = currentProgram.getMemory().getBytes(address, buf)
    except MemoryAccessException as exc:
        return {"error": str(exc)}
    return " ".join("%02x" % (buf[i] & 0xff) for i in range(read))


def emit_target(query):
    address = resolve_address(query["target"])
    symbol = None
    if address is None:
        symbol = get_symbol(query["target"])
        if symbol is not None:
            address = symbol.getAddress()

    function = None
    if address is not None:
        function = getFunctionContaining(address)
        if function is None:
            function = getFunctionAt(address)

    result = {
        "query": query["target"],
        "address": addr_to_string(address),
    }

    if symbol is None and address is not None:
        symbol = getSymbolAt(address)
    if symbol is not None:
        result["symbol"] = symbol.getName(True)
    if function is not None:
        result["function"] = function_json(function, query["decompile"])
    if address is not None:
        byte_data = bytes_json(address, query["bytes"])
        if isinstance(byte_data, dict):
            result["bytesError"] = byte_data["error"]
        elif byte_data is not None:
            result["bytes"] = byte_data
        result["referencesTo"] = refs_json(getReferencesTo(address), query["limit"])
        result["referencesFrom"] = refs_json(getReferencesFrom(address), query["limit"])
    return result


def emit_matches(search, limit):
    matches = []
    symbol_iter = currentProgram.getSymbolTable().getAllSymbols(True)
    needle = search.lower()
    while symbol_iter.hasNext() and len(matches) < limit:
        symbol = symbol_iter.next()
        if needle in symbol.getName(True).lower():
            matches.append({
                "name": symbol.getName(True),
                "address": str(symbol.getAddress()),
                "type": str(symbol.getSymbolType()),
            })
    return matches


query = parse_args(getScriptArgs())
payload = {
    "program": currentProgram.getName(),
    "executablePath": currentProgram.getExecutablePath(),
    "language": currentProgram.getLanguageID().getIdAsString(),
    "compilerSpec": currentProgram.getCompilerSpec().getCompilerSpecID().getIdAsString(),
}

if query["target"]:
    payload["target"] = emit_target(query)
if query["search"]:
    payload["matches"] = emit_matches(query["search"], query["limit"])

text = json.dumps(payload, sort_keys=True)
if query["out"]:
    with open(query["out"], "w") as handle:
        handle.write(text)
        handle.write("\n")
else:
    print(text)
