CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common
TEST_PATH = test

CFLAGS = -O3 -Wall -Wextra -Wpedantic -std=c99 -DUSE_HARDWARE_HASH \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) \
	-I$(TEST_PATH) -I. -Isoft

COMMON_SRCS = encode kem matrix pke sample scloudplus_params aes_c fips202 random util
PLATFORM_SRCS = op_hash op_matrix op_trng sha256 sm3 endian
LIB = libpqc_scloudplus.a
TEST_EXE = test_scloudplus_ref

all: $(LIB)

$(LIB): $(COMMON_SRCS:%=%.o) $(PLATFORM_SRCS:%=%.o)
	$(AR) -r $@ $^

encode.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/encode.c -o $@

kem.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/kem.c -o $@

matrix.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/matrix.c -o $@

pke.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/pke.c -o $@

sample.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/sample.c -o $@

scloudplus_params.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/scloudplus_params.c -o $@

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

op_matrix.o:
	$(CC) $(CFLAGS) -c soft/op_matrix.c -o $@

op_trng.o:
	$(CC) $(CFLAGS) -c soft/op_trng.c -o $@

sha256.o:
	$(CC) $(CFLAGS) -c soft/sha256.c -o $@

sm3.o:
	$(CC) $(CFLAGS) -c soft/sm3.c -o $@

endian.o:
	$(CC) $(CFLAGS) -c soft/endian.c -o $@

tests_main.o:
	$(CC) $(CFLAGS) -c $(TEST_PATH)/tests_main.c -o $@

$(TEST_EXE): $(LIB) tests_main.o
	$(CC) $(CFLAGS) -o $@ tests_main.o $(LIB)

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE)
