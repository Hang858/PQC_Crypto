CC ?= cc
AR ?= ar
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Wpedantic
CPPFLAGS += -I../../include -I../../core/common -I. -Isoft
LDLIBS += -lm
ifeq ($(OS),Windows_NT)
LDLIBS += -lbcrypt
endif
BUILD ?= build
BENCH_OPT ?= O2
CORE = ../../core/common
MODULES = randombytes ntt packing parabola poly polyvec_ref sample sign spectral symmetric signature_api signature_codec_128
SOFT = op_hash op_trng op_ntt sha256 sm3 endian
OBJECTS = $(addprefix $(BUILD)/,$(addsuffix .o,$(MODULES) $(SOFT)))
TESTFLAGS = -Itest -Itest/reference

.DEFAULT_GOAL := all
.PHONY: all check kat clean op-contract correctness production-correctness random-contract memory stack sanitize benchmark
all: $(BUILD)/libpqc_bit.a
$(BUILD):
	mkdir -p $@
$(BUILD)/%.o: $(CORE)/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -fstack-usage -c $< -o $@
$(BUILD)/%.o: soft/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -fstack-usage -c $< -o $@
$(BUILD)/%.s: $(CORE)/%.c FORCE | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fstack-usage -S $< -o $@
$(BUILD)/%.s: soft/%.c FORCE | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fstack-usage -S $< -o $@
$(BUILD)/libpqc_bit.a: $(OBJECTS)
	$(RM) $@
	$(AR) rcs $@ $^
$(BUILD)/reference.o: test/reference/fips202.c | $(BUILD)
	$(CC) $(CFLAGS) -Itest/reference -c $< -o $@
$(BUILD)/kat_drng.o: test/reference/drng.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) -MMD -MP -c $< -o $@
$(BUILD)/kat_random.o: test/reference/op_trng_kat.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) -c $< -o $@
$(BUILD)/correctness: test/test_correctness.c $(BUILD)/reference.o $(BUILD)/kat_drng.o $(BUILD)/kat_random.o $(BUILD)/libpqc_bit.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) $< $(BUILD)/reference.o $(BUILD)/kat_drng.o $(BUILD)/kat_random.o $(BUILD)/libpqc_bit.a $(LDLIBS) -o $@
correctness: $(BUILD)/correctness
	./$(BUILD)/correctness
$(BUILD)/production_correctness: test/test_correctness.c $(BUILD)/reference.o $(BUILD)/kat_drng.o $(BUILD)/libpqc_bit.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) $^ $(LDLIBS) -o $@
production-correctness: $(BUILD)/production_correctness
	./$(BUILD)/production_correctness
$(BUILD)/op_real.o: soft/op_hash.c soft/sha3.h soft/sm3.h operator_interface.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -DOP_hash=real_OP_hash -DOP_hash_init=real_OP_hash_init -DOP_hash_absorb=real_OP_hash_absorb -DOP_hash_squeeze=real_OP_hash_squeeze -c $< -o $@
$(BUILD)/trng_real.o: soft/op_trng.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DOP_trng=real_OP_trng -c $< -o $@
$(BUILD)/test_op_contract: test/test_op_contract.c test/memory_probe.c $(addprefix $(BUILD)/mem/,$(addsuffix .o,$(MODULES))) $(addprefix $(BUILD)/,$(addsuffix .o,op_ntt sha256 sm3 endian)) $(BUILD)/op_real.o $(BUILD)/trng_real.o $(BUILD)/reference.o $(BUILD)/kat_drng.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) $^ $(LDLIBS) -o $@
op-contract: $(BUILD)/test_op_contract
	./$(BUILD)/test_op_contract
$(BUILD)/KAT_SIG: test/KAT_SIG.c $(BUILD)/kat_drng.o $(BUILD)/kat_random.o $(BUILD)/libpqc_bit.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) $^ $(LDLIBS) -o $@
kat: $(BUILD)/KAT_SIG
	cd $(BUILD) && ./KAT_SIG
check: correctness production-correctness op-contract random-contract ntt-contract
$(BUILD)/mem:
	mkdir -p $@
$(BUILD)/mem/%.o: $(CORE)/%.c | $(BUILD)/mem
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -Itest -include test/memory_probe.h -Dmalloc=bit_test_malloc -Dfree=bit_test_free -c $< -o $@
$(BUILD)/test_memory: test/test_memory.c test/memory_probe.c $(addprefix $(BUILD)/mem/,$(addsuffix .o,$(MODULES))) $(addprefix $(BUILD)/,$(addsuffix .o,$(SOFT)))
	$(CC) $(CPPFLAGS) $(CFLAGS) -Itest $^ $(LDLIBS) -o $@
memory: $(BUILD)/test_memory
	./$(BUILD)/test_memory
$(BUILD)/test_random_operator: test/test_random_operator.c test/memory_probe.c $(addprefix $(BUILD)/mem/,$(addsuffix .o,$(MODULES))) $(addprefix $(BUILD)/,$(addsuffix .o,op_hash op_ntt sha256 sm3 endian))
	$(CC) $(CPPFLAGS) $(CFLAGS) -Itest $^ $(LDLIBS) -o $@
random-contract: $(BUILD)/test_random_operator
	./$(BUILD)/test_random_operator
$(BUILD)/ntt_real.o: soft/op_ntt.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DOP_ntt256_init=real_OP_ntt256_init -DOP_ntt256=real_OP_ntt256 -DOP_intt256=real_OP_intt256 -DOP_cwm=real_OP_cwm -c $< -o $@
$(BUILD)/ntt_generic.o: test/reference/op_ntt_generic.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD)/ntt_scalar.o: test/reference/ntt_scalar.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
NTT_TEST_CORE = $(addprefix $(BUILD)/mem/,$(addsuffix .o,$(MODULES))) $(addprefix $(BUILD)/,$(addsuffix .o,op_hash sha256 sm3 endian kat_drng kat_random ntt_scalar))
$(BUILD)/test_ntt_contract: test/test_ntt_contract.c test/memory_probe.c $(NTT_TEST_CORE) $(BUILD)/ntt_real.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) $^ $(LDLIBS) -o $@
$(BUILD)/test_ntt_generic: test/test_ntt_contract.c test/memory_probe.c $(NTT_TEST_CORE) $(BUILD)/ntt_generic.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TESTFLAGS) $^ $(LDLIBS) -o $@
.PHONY: ntt-contract
ntt-contract: $(BUILD)/test_ntt_contract $(BUILD)/test_ntt_generic
	./$(BUILD)/test_ntt_contract
	./$(BUILD)/test_ntt_generic
$(BUILD)/benchmark: test/benchmark.c $(BUILD)/libpqc_bit.a
	$(CC) $(CPPFLAGS) $(CFLAGS) -DBIT_BENCH_PRODUCTION=1 $^ $(LDLIBS) -o $@
benchmark: | $(BUILD)
	python3 test/run_comparison.py --cc '$(CC)' --opt-level $(BENCH_OPT) --output $(BUILD)/comparison.json
sanitize:
	$(MAKE) BUILD=build-asan CFLAGS='-O1 -g -std=c11 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer' check memory
stack: all $(OBJECTS:.o=.s)
	python3 test/check_stack.py $(BUILD) $(MODULES) $(SOFT)
clean:
	$(RM) -r $(BUILD)
-include $(OBJECTS:.o=.d)
-include $(BUILD)/kat_drng.d $(BUILD)/op_real.d $(BUILD)/trng_real.d
-include $(addprefix $(BUILD)/mem/,$(addsuffix .d,$(MODULES)))
.PHONY: FORCE
FORCE:
