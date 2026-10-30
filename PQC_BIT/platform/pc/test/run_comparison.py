#!/usr/bin/env python3
"""Build an untouched temporary original; compare bytes, cross verify, benchmark.
Run from any directory. No changes are made to either source release.
"""
import argparse
import hashlib
import json
import os
import pathlib
import statistics
import subprocess
import sys
import tempfile

p = argparse.ArgumentParser()
p.add_argument("--rounds", type=int, default=7)
p.add_argument("--cc", default=os.environ.get("CC", "cc"))
p.add_argument("--opt-level", choices=("O2", "O3"), default="O2")
p.add_argument("--output", type=pathlib.Path)
p.add_argument("--reference", type=pathlib.Path,
               help="original BiP release directory (default: local-reference/BiP-release-20261030)")
args = p.parse_args()
if args.rounds < 3:
    p.error("at least three measured rounds are required")
test = pathlib.Path(__file__).resolve().parent
pc = test.parent
module = pc.parents[1]
release = (args.reference or module.parent / "local-reference/BiP-release-20261030").resolve()
ref = release / "ref/BiP-128"
common = release / "common"
if not (ref / "ntt.c").is_file() or not (common / "sign.c").is_file():
    p.error("original BiP release not found at " + str(release) +
            "; provide --reference /path/to/BiP-release-20261030")
flags = ["-" + args.opt_level, "-std=c11", "-Wall", "-Wextra", "-Wpedantic"]

def run(cmd, **kwargs):
    try:
        return subprocess.run([str(x) for x in cmd], check=True, capture_output=True,
                              text=True, **kwargs).stdout
    except subprocess.CalledProcessError as exc:
        sys.stderr.write(exc.stdout or "")
        sys.stderr.write(exc.stderr or "")
        raise

with tempfile.TemporaryDirectory(prefix="bit128-comparison-") as tmp:
    tmp = pathlib.Path(tmp)
    baseline, adapted, kat_adapted = tmp / "baseline", tmp / "adapted", tmp / "kat_adapted"
    original = [ref / (n + ".c") for n in ("drng", "fips202", "ntt", "packing", "parabola", "poly")]
    original += [common / (n + ".c") for n in ("sample", "sign", "spectral", "symmetric", "signature_api", "polyvec_ref", "signature_codec_128")]
    run([args.cc, *flags, "-DBIT_IMPL_AVX=0", "-DBIT_BENCH_BASELINE=1", "-I" + str(ref),
         "-I" + str(common), test / "benchmark.c", *original, "-lm", "-o", baseline])
    # Build fresh objects at the selected level so stale flags cannot bias timing.
    run(["make", "-C", pc, "BUILD=" + str(tmp / "objects"), "CC=" + args.cc,
         "CFLAGS=" + " ".join(flags), "all"])
    run([args.cc, *flags, "-I" + str(module / "include"), "-DBIT_BENCH_PRODUCTION=1", test / "benchmark.c",
         tmp / "objects/libpqc_bit.a", "-lm", "-o", adapted])
    run([args.cc, *flags, "-I" + str(module / "include"), "-I" + str(test / "reference"),
         "-I" + str(module / "core/common"), "-I" + str(pc), test / "benchmark.c",
         test / "reference/drng.c", test / "reference/op_trng_kat.c",
         tmp / "objects/libpqc_bit.a", "-lm", "-o", kat_adapted])
    a, b, c = tmp / "original.bin", tmp / "kat.bin", tmp / "production.bin"
    run([baseline, "--emit", a]); run([kat_adapted, "--emit", b])
    if a.read_bytes() != b.read_bytes():
        raise SystemExit("FAIL: deterministic OP fixture differs from original")
    print("PASS: deterministic OP fixture byte-identical to original (512 signatures)")
    run([adapted, "--emit", c])
    print("Production verifies original: " + run([adapted, "--verify", a]).strip())
    print("Original verifies production: " + run([baseline, "--verify", c]).strip())
    production_resigned, original_resigned = tmp / "production-resigned.bin", tmp / "original-resigned.bin"
    print(run([adapted, "--resign", a, production_resigned]).strip())
    print("Original verifies production using original secret keys: " + run([baseline, "--verify", production_resigned]).strip())
    print(run([baseline, "--resign", c, original_resigned]).strip())
    print("Production verifies original using production secret keys: " + run([adapted, "--verify", original_resigned]).strip())
    digest = hashlib.sha256(a.read_bytes()).hexdigest()
    samples = {"baseline": [], "adapted": []}
    # Warm both paths before alternating measurement order across rounds.
    run([baseline]); run([adapted])
    for r in range(args.rounds):
        order = [("baseline", baseline), ("adapted", adapted)]
        if r % 2: order.reverse()
        for name, exe in order:
            samples[name].append(json.loads(run([exe])))
    results = {"compiler": run([args.cc, "--version"]).splitlines()[0],
               "flags": flags, "rounds": args.rounds,
               "random_sources": {"baseline": "original SM3 DRNG", "adapted": "production OP_trng (system entropy)", "byte_comparison": "separate TEST ONLY OP_trng backed by original stream"},
               "cross_validation": {"signatures_per_direction": 512, "cross_verify_both_directions": True, "cross_sign_imported_secret_keys_both_directions": True}, "keys_per_round": 32,
               "signatures_per_round": 512, "vectors_sha256": digest, "samples": samples}
    results["summary"] = {}
    for op in ("keygen_ns", "sign_ns", "verify_ns"):
        av = [s[op] for s in samples["baseline"]]
        bv = [s[op] for s in samples["adapted"]]
        am, bm = statistics.median(av), statistics.median(bv)
        results["summary"][op] = {"baseline_median": am, "adapted_median": bm,
                                  "change_percent": 100 * (bm / am - 1),
                                  "baseline_range": [min(av), max(av)],
                                  "adapted_range": [min(bv), max(bv)]}
    report = json.dumps(results, indent=2) + "\n"
    print(report)
    if args.output:
        args.output.write_text(report)
