CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common

CFLAGS = -O2 -Wall -Wextra -Wpedantic -std=c99 -DUSE_HARDWARE_HASH \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) -I. -Isoft

COMMON_SRCS = codec common fft fpr keygen nist rng shake sign vrfy falcon_params
PLATFORM_SRCS = randombytes
OPERATOR_SRCS = endian op_hash op_matrix op_trng op_fpr sha256 sm3
LIBS = -lm

LIB = libpqc_falcon.a
TEST_EXE = test_falcon
KAT_EXE = kat_runtime

all: $(LIB)

$(LIB): $(COMMON_SRCS:%=%.o) $(PLATFORM_SRCS:%=%.o) $(OPERATOR_SRCS:%=%.o)
	$(AR) -r $@ $^

%.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/$*.c -o $@

randombytes.o:
	$(CC) $(CFLAGS) -c randombytes.c -o $@

op_fpr.o:
	$(CC) $(CFLAGS) -c soft/op_fpr.c -o $@

op_hash.o:
	$(CC) $(CFLAGS) -c soft/op_hash.c -o $@

op_matrix.o:
	$(CC) $(CFLAGS) -c soft/op_matrix.c -o $@

op_trng.o:
	$(CC) $(CFLAGS) -c soft/op_trng.c -o $@

endian.o:
	$(CC) $(CFLAGS) -c soft/endian.c -o $@

sha256.o:
	$(CC) $(CFLAGS) -c soft/sha256.c -o $@

sm3.o:
	$(CC) $(CFLAGS) -c soft/sm3.c -o $@

test_falcon.o:
	$(CC) $(CFLAGS) -c test_falcon.c -o $@

$(TEST_EXE): $(LIB) test_falcon.o
	$(CC) $(CFLAGS) -o $@ test_falcon.o -Wl,--whole-archive $(LIB) -Wl,--no-whole-archive $(LIBS)

kat_runtime.o:
	$(CC) $(CFLAGS) -c kat_runtime.c -o $@

katrng.o:
	$(CC) $(CFLAGS) -c katrng.c -o $@

$(KAT_EXE): $(COMMON_SRCS:%=%.o) $(OPERATOR_SRCS:%=%.o) kat_runtime.o katrng.o
	$(CC) $(CFLAGS) -o $@ kat_runtime.o katrng.o $(COMMON_SRCS:%=%.o) $(OPERATOR_SRCS:%=%.o) $(LIBS)

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE) $(KAT_EXE)
