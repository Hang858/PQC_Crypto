CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common
TEST_PATH = test

CFLAGS = -O3 -Wall -Wextra -Wpedantic -std=c99 -DUSE_HARDWARE_HASH \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) \
	-I$(CORE_PATH)/scloudplus128 -I$(CORE_PATH)/scloudplus192 -I$(CORE_PATH)/scloudplus256 \
	-I$(TEST_PATH) -I. -Isoft

LEVEL_SRCS = encode kem matrix pke sample
COMMON_SRCS = aes_c fips202 random util op_hash sha256 sm3 endian
LIB = libpqc_scloudplus.a
TEST_EXE = test_scloudplus_ref

all: $(LIB)

$(LIB): libscloudplus128_ref.a libscloudplus192_ref.a libscloudplus256_ref.a api.o $(COMMON_SRCS:%=%.o)
	$(AR) -r $@ api.o $(COMMON_SRCS:%=%.o)

libscloudplus128_ref.a: $(LEVEL_SRCS:%=scloudplus128_%.o)
	$(AR) -r $@ $^

libscloudplus192_ref.a: $(LEVEL_SRCS:%=scloudplus192_%.o)
	$(AR) -r $@ $^

libscloudplus256_ref.a: $(LEVEL_SRCS:%=scloudplus256_%.o)
	$(AR) -r $@ $^

api.o:
	$(CC) $(CFLAGS) -c $(CORE_PATH)/api.c -o $@

scloudplus128_%.o:
	$(CC) $(CFLAGS) -DSCLOUDPLUS_LEVEL_128 -c $(COMMON_PATH)/$*.c -o $@

scloudplus192_%.o:
	$(CC) $(CFLAGS) -DSCLOUDPLUS_LEVEL_192 -c $(COMMON_PATH)/$*.c -o $@

scloudplus256_%.o:
	$(CC) $(CFLAGS) -DSCLOUDPLUS_LEVEL_256 -c $(COMMON_PATH)/$*.c -o $@

aes_c.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/aes_c.c -o $@

fips202.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/fips202.c -o $@

random.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/random.c -o $@

util.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/util.c -o $@

op_hash.o:
	$(CC) $(CFLAGS) -c soft/op_hash.c -o $@

sha256.o:
	$(CC) $(CFLAGS) -c soft/sha256.c -o $@

sm3.o:
	$(CC) $(CFLAGS) -c soft/sm3.c -o $@

endian.o:
	$(CC) $(CFLAGS) -c soft/endian.c -o $@

tests_main.o:
	$(CC) $(CFLAGS) -c $(TEST_PATH)/tests_main.c -o $@

$(TEST_EXE): $(LIB) tests_main.o
	$(CC) $(CFLAGS) -o $@ tests_main.o api.o $(COMMON_SRCS:%=%.o) \
		-Wl,--whole-archive libscloudplus128_ref.a libscloudplus192_ref.a libscloudplus256_ref.a -Wl,--no-whole-archive \
		-lm

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE)
