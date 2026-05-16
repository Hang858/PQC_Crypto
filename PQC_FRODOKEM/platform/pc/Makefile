CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common
TEST_PATH = test

BASE_CFLAGS = -O3 -Wall -Wextra -Wpedantic -std=c99 -DNIX -D_AMD64_ -D_REFERENCE_ -D_SHAKE128_FOR_A_ -DUSE_HARDWARE_HASH \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) \
	-I. -Isoft -I$(TEST_PATH)

COMMON_SRCS = kem noise util frodo_macrify_reference frodokem_params random fips202
PLATFORM_SRCS = op_hash op_matrix op_trng op_fpr sha256 sm3 endian

LIB = libpqc_frodokem.a
TEST_EXE = test_frodokem_ref
KAT_EXE = kat_runtime

all: $(LIB)

$(LIB): $(COMMON_SRCS:%=%.o) $(PLATFORM_SRCS:%=%.o)
	$(AR) -r $@ $^

%.o:
	$(CC) $(BASE_CFLAGS) -c $(COMMON_PATH)/$*.c -o $@

op_hash.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_hash.c -o $@

op_matrix.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_matrix.c -o $@

op_trng.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_trng.c -o $@

op_fpr.o:
	$(CC) $(BASE_CFLAGS) -c soft/op_fpr.c -o $@

sha256.o:
	$(CC) $(BASE_CFLAGS) -c soft/sha256.c -o $@

sm3.o:
	$(CC) $(BASE_CFLAGS) -c soft/sm3.c -o $@

endian.o:
	$(CC) $(BASE_CFLAGS) -c soft/endian.c -o $@

tests_main.o:
	$(CC) $(BASE_CFLAGS) -c $(TEST_PATH)/tests_main.c -o $@

kat_runtime.o:
	$(CC) $(BASE_CFLAGS) -c $(TEST_PATH)/kat_runtime.c -o $@

rng.o:
	$(CC) $(BASE_CFLAGS) -c $(TEST_PATH)/rng.c -o $@

aes_c.o:
	$(CC) $(BASE_CFLAGS) -c $(TEST_PATH)/aes_c.c -o $@

$(TEST_EXE): $(LIB) tests_main.o
	$(CC) $(BASE_CFLAGS) -o $@ tests_main.o -Wl,--whole-archive $(LIB) -Wl,--no-whole-archive

$(KAT_EXE): $(LIB) kat_runtime.o rng.o aes_c.o
	$(CC) $(BASE_CFLAGS) -o $@ kat_runtime.o rng.o aes_c.o $(LIB)

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE) $(KAT_EXE)
