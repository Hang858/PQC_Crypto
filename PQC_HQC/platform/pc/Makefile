CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common
MUNIT_PATH = $(PROJECT_PATH)/tests/external/munit
TEST_PATH = test

CFLAGS = -O3 -Wall -Wextra -Wpedantic -std=c99 \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) \
	-I$(CORE_PATH)/hqc1 -I$(CORE_PATH)/hqc3 -I$(CORE_PATH)/hqc5 \
	-I. -Isoft -I$(TEST_PATH) -I$(PROJECT_PATH)/tests -I$(MUNIT_PATH)

COMMON_SRCS = code fft gf gf2x hqc kem parsing reed_muller reed_solomon symmetric vector
PLATFORM_SRCS = crypto_memset fips202 endian op_hash sha256 sm3

LIB = libpqc_hqc.a
TEST_EXE = test_hqc_ref

all: $(LIB)

$(LIB): libhqc1_ref.a libhqc3_ref.a libhqc5_ref.a api.o $(PLATFORM_SRCS:%=%.o)
	$(AR) -r $@ api.o $(PLATFORM_SRCS:%=%.o)

libhqc1_ref.a: $(COMMON_SRCS:%=hqc1_%.o)
	$(AR) -r $@ $^

libhqc3_ref.a: $(COMMON_SRCS:%=hqc3_%.o)
	$(AR) -r $@ $^

libhqc5_ref.a: $(COMMON_SRCS:%=hqc5_%.o)
	$(AR) -r $@ $^

api.o:
	$(CC) $(CFLAGS) -c $(CORE_PATH)/api.c -o $@

hqc1_%.o:
	$(CC) $(CFLAGS) -DHQC_LEVEL_HQC1 -c $(COMMON_PATH)/$*.c -o $@

hqc3_%.o:
	$(CC) $(CFLAGS) -DHQC_LEVEL_HQC3 -c $(COMMON_PATH)/$*.c -o $@

hqc5_%.o:
	$(CC) $(CFLAGS) -DHQC_LEVEL_HQC5 -c $(COMMON_PATH)/$*.c -o $@

crypto_memset.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/crypto_memset.c -o $@

fips202.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/fips202.c -o $@

endian.o:
	$(CC) $(CFLAGS) -c soft/endian.c -o $@

op_hash.o:
	$(CC) $(CFLAGS) -c soft/op_hash.c -o $@

sha256.o:
	$(CC) $(CFLAGS) -c soft/sha256.c -o $@

sm3.o:
	$(CC) $(CFLAGS) -c soft/sm3.c -o $@

tests_main.o:
	$(CC) $(CFLAGS) -c $(TEST_PATH)/tests_main.c -o $@

test_kem_ref.o:
	$(CC) $(CFLAGS) -c $(TEST_PATH)/test_kem_ref.c -o $@

test_pke.o:
	$(CC) $(CFLAGS) -c $(TEST_PATH)/test_pke.c -o $@

munit.o:
	$(CC) $(CFLAGS) -c $(MUNIT_PATH)/munit.c -o $@

$(TEST_EXE): $(LIB) tests_main.o test_kem_ref.o test_pke.o munit.o
	$(CC) $(CFLAGS) -o $@ tests_main.o test_kem_ref.o test_pke.o munit.o \
		api.o $(PLATFORM_SRCS:%=%.o) \
		-Wl,--whole-archive libhqc1_ref.a libhqc3_ref.a libhqc5_ref.a -Wl,--no-whole-archive

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE)
