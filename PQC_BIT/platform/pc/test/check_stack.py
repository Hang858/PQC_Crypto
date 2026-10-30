#!/usr/bin/env python3
"""Conservative direct-call frame sum for the exact generated assembly.

Tail calls are counted as nested calls (safe overestimate). All OP algorithms
are included, even when BiT uses only a subset. OP_hash's self-call implements
chain modes; BiT always passes OP_MODE_NORMAL, so that unreachable edge is
excluded explicitly. External libc and interrupt stack are not inferred here.
"""
import json
import pathlib
import re
import sys

root = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else "build")
frames = {}
names = sys.argv[2:]
usage_files = [root / (n + ".su") for n in names] if names else list(root.glob("*.su"))
for p in usage_files:
    for line in p.read_text().splitlines():
        loc, size, kind = line.split("\t")
        name = loc.rsplit(":", 1)[-1]
        if "dynamic" in kind:
            sys.exit(f"FAIL: dynamic frame {name}")
        frames[name] = int(size)
if not frames:
    sys.exit("FAIL: no stack-usage files")
asmfiles = [root / (n + ".s") for n in names] if names else list(root.glob("*.s"))
if not asmfiles:
    sys.exit("FAIL: no matching assembly (run make stack)")

def symbol(s):
    s = s.split("@", 1)[0]
    return s if s in frames else s[1:] if s.startswith("_") else s

graph = {name: set() for name in frames}
seen = set()
external = set()
for p in asmfiles:
    current = None
    for line in p.read_text().splitlines():
        label = re.match(r"^([\w.$]+):", line)
        if label and symbol(label[1]) in frames:
            current = symbol(label[1]); seen.add(current)
        if ".cfi_endproc" in line or ".fnend" in line:
            current = None
        call = re.match(r"\s*(?:bl|b|callq?|jmpq?)\s+([\w.$@]+)", line)
        if current and call:
            target = symbol(call[1])
            if target in frames:
                if not (current == target == "OP_hash"):
                    graph[current].add(target)
            elif not target.startswith(("LBB", ".L", "Lloh")):
                external.add(target)
        if current and re.match(r"\s*(?:blr|callq?\s+\*)", line):
            sys.exit(f"FAIL: unresolved indirect call in {current}")
if frames.keys() - seen:
    sys.exit(f"FAIL: no assembly for {sorted(frames.keys() - seen)}")

def bound(name, active=()):
    if name in active:
        sys.exit(f"FAIL: recursive call cycle {' -> '.join((*active, name))}")
    options = [bound(c, (*active, name)) for c in graph[name]]
    subtotal, path = max(options, default=(0, []), key=lambda x: x[0])
    return frames[name] + subtotal, [name] + path

result = {}
for name in ("bit_sig_keygen", "bit_sig_sign", "bit_sig_verify", "bit_random_init"):
    if name not in frames:
        sys.exit(f"FAIL: missing root {name}")
    size, path = bound(name)
    result[name] = size
    print(f"{name}: {size} B known C frames; {' -> '.join(path)}")
print("External symbols (not included): " + ", ".join(sorted(external)))
print("Excluded unreachable recursion: OP_hash chain mode (BiT passes mode 0)")
(root / "stack-report.json").write_text(json.dumps(result, indent=2) + "\n")
if max(result.values()) > 3072:
    sys.exit("FAIL: known algorithm/OP call-chain bound exceeds 3072 B")
print("PASS: known algorithm/OP call chains <= 3072 B; libc/interrupt budget is separate")
