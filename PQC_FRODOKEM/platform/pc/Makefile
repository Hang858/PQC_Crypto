CC = gcc
AR = ar

PROJECT_PATH = ../..
INCLUDE_PATH = $(PROJECT_PATH)/include
CORE_PATH = $(PROJECT_PATH)/core
COMMON_PATH = $(CORE_PATH)/common
TESTS_PATH = $(PROJECT_PATH)/tests
TEST_PATH = test

CFLAGS = -O3 -Wall -Wextra -Wpedantic -std=c99 -DNIX -D_AMD64_ -D_REFERENCE_ -D_SHAKE128_FOR_A_ -DUSE_HARDWARE_HASH \
	-I$(INCLUDE_PATH) -I$(CORE_PATH) -I$(COMMON_PATH) \
	-I$(CORE_PATH)/frodo640 -I$(CORE_PATH)/frodo976 -I$(CORE_PATH)/frodo1344 \
	-I. -Isoft -I$(TEST_PATH) -I$(TESTS_PATH)

LEVEL_SRCS = efrodo kem_ref noise util frodo_macrify_reference
PLATFORM_SRCS = fips202 random aes_c op_hash sha256 sm3 endian

LIB = libpqc_frodokem.a
TEST_EXE = test_frodokem_ref

all: $(LIB)

$(LIB): libfrodokem640_ref.a libfrodokem976_ref.a libfrodokem1344_ref.a api.o $(PLATFORM_SRCS:%=%.o)
	$(AR) -r $@ api.o $(PLATFORM_SRCS:%=%.o)

libfrodokem640_ref.a: $(LEVEL_SRCS:%=frodokem640_%.o)
	$(AR) -r $@ $^

libfrodokem976_ref.a: $(LEVEL_SRCS:%=frodokem976_%.o)
	$(AR) -r $@ $^

libfrodokem1344_ref.a: $(LEVEL_SRCS:%=frodokem1344_%.o)
	$(AR) -r $@ $^

api.o:
	$(CC) $(CFLAGS) -c $(CORE_PATH)/api.c -o $@

frodokem640_efrodo.o:
	$(CC) $(CFLAGS) -DFRODOKEM_LEVEL_640 -c $(CORE_PATH)/frodo640/efrodo640.c -o $@

frodokem976_efrodo.o:
	$(CC) $(CFLAGS) -DFRODOKEM_LEVEL_976 -c $(CORE_PATH)/frodo976/efrodo976.c -o $@

frodokem1344_efrodo.o:
	$(CC) $(CFLAGS) -DFRODOKEM_LEVEL_1344 -c $(CORE_PATH)/frodo1344/efrodo1344.c -o $@

frodokem640_%.o:
	$(CC) $(CFLAGS) -DFRODOKEM_LEVEL_640 -c $(COMMON_PATH)/$*.c -o $@

frodokem976_%.o:
	$(CC) $(CFLAGS) -DFRODOKEM_LEVEL_976 -c $(COMMON_PATH)/$*.c -o $@

frodokem1344_%.o:
	$(CC) $(CFLAGS) -DFRODOKEM_LEVEL_1344 -c $(COMMON_PATH)/$*.c -o $@

fips202.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/fips202.c -o $@

random.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/random.c -o $@

aes_c.o:
	$(CC) $(CFLAGS) -c $(COMMON_PATH)/aes_c.c -o $@

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
	$(CC) $(CFLAGS) -o $@ tests_main.o api.o $(PLATFORM_SRCS:%=%.o) \
		-Wl,--whole-archive libfrodokem640_ref.a libfrodokem976_ref.a libfrodokem1344_ref.a -Wl,--no-whole-archive \
		-lm

test: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE)
