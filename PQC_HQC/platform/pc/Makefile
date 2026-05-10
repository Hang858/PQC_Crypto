CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common
MUNIT_PATH = $(PROJECT_PATH)/tests/external/munit
TEST_PATH = test

BASE_CFLAGS = -O3 -Wall -Wextra -Wpedantic -std=c99 \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) \
	-I. -Isoft -I$(TEST_PATH) -I$(PROJECT_PATH)/tests -I$(MUNIT_PATH)

RUNTIME_CFLAGS = $(BASE_CFLAGS)

COMMON_SRCS = code hqc kem parsing symmetric vector fft gf gf2x reed_muller reed_solomon hqc_params
PLATFORM_SRCS = crypto_memset fips202 endian op_hash op_matrix op_trng sha256 sm3

LIB = libpqc_hqc.a
TEST_EXE = test_hqc_ref
KAT_EXE = kat_runtime

all: $(LIB)

$(LIB): $(COMMON_SRCS:%=%.o) $(PLATFORM_SRCS:%=%.o)
	$(AR) -r $@ $^

%.o:
	$(CC) $(RUNTIME_CFLAGS) -c $(COMMON_PATH)/$*.c -o $@

crypto_memset.o:
	$(CC) $(BASE_CFLAGS) -c $(COMMON_PATH)/crypto_memset.c -o $@

fips202.o:
	$(CC) $(BASE_CFLAGS) -c $(COMMON_PATH)/fips202.c -o $@

endian.o:
	$(CC) $(BASE_CFLAGS) -c soft/endian.c -o $@

op_hash.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_hash.c -o $@

op_matrix.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_matrix.c -o $@

op_trng.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_trng.c -o $@

sha256.o:
	$(CC) $(BASE_CFLAGS) -c soft/sha256.c -o $@

sm3.o:
	$(CC) $(BASE_CFLAGS) -c soft/sm3.c -o $@

tests_main.o:
	$(CC) $(RUNTIME_CFLAGS) -c $(TEST_PATH)/tests_main.c -o $@

test_kem_ref.o:
	$(CC) $(RUNTIME_CFLAGS) -c $(TEST_PATH)/test_kem_ref.c -o $@

test_pke.o:
	$(CC) $(RUNTIME_CFLAGS) -c $(TEST_PATH)/test_pke.c -o $@

munit.o:
	$(CC) $(BASE_CFLAGS) -c $(MUNIT_PATH)/munit.c -o $@

$(TEST_EXE): $(LIB) tests_main.o test_kem_ref.o test_pke.o munit.o
	$(CC) $(BASE_CFLAGS) -o $@ tests_main.o test_kem_ref.o test_pke.o munit.o \
		-Wl,--whole-archive $(LIB) -Wl,--no-whole-archive

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE) $(KAT_EXE) kat_runtime.o

kat_runtime.o: $(TEST_PATH)/kat_runtime.c
	$(CC) $(BASE_CFLAGS) -DHQC_USE_DETERMINISTIC_PRNG -c $(TEST_PATH)/kat_runtime.c -o $@

$(KAT_EXE): $(LIB) kat_runtime.o
	$(CC) $(BASE_CFLAGS) -o $@ kat_runtime.o -Wl,--whole-archive $(LIB) -Wl,--no-whole-archive
