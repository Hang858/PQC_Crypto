/* PC entropy backend. Embedded builds provide their hardware OP_trng instead. */
#include "operator_interface.h"
#include <errno.h>
#if !defined(_WIN32)
#include <fcntl.h>
#include <unistd.h>
#endif
#if defined(__APPLE__)
#include <sys/random.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#endif
int OP_trng(void *buffer, int size)
{
    if (size < 0 || (buffer == NULL && size != 0)) return OP_FAILURE;
    if (size == 0) return OP_SUCCESS;
#if defined(_WIN32)
    return BCryptGenRandom(NULL, buffer, (ULONG)size, BCRYPT_USE_SYSTEM_PREFERRED_RNG) == 0 ? OP_SUCCESS : OP_FAILURE;
#else
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__)
    /* These OS APIs accept at most 256 bytes per request, without opening a
     * file or allocating stdio buffers. Normal BiT requests are 16/32 bytes. */
    unsigned char *out = buffer;
    size_t left = (size_t)size;
    while (left) {
        size_t take = left < 256 ? left : 256;
        if (getentropy(out, take) != 0) {
            if (errno == EINTR) continue;
            return OP_FAILURE;
        }
        out += take; left -= take;
    }
    return OP_SUCCESS;
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) return OP_FAILURE;
    unsigned char *out = buffer;
    size_t left = (size_t)size;
    while (left) {
        ssize_t n = read(fd, out, left);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0) { close(fd); return OP_FAILURE; }
        out += (size_t)n; left -= (size_t)n;
    }
    return close(fd) == 0 ? OP_SUCCESS : OP_FAILURE;
#endif
#endif
}
