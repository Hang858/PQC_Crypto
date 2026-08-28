#include <stdint.h>
#include <stddef.h>
#include "ntt_reg.h"
#include "ntt_drv.h"

static uint8_t g_is_16bit = 0;

static inline uint32_t ntt_reg_read(uint32_t offset)
{
    return *(volatile uint32_t *)((uintptr_t)NTT_REG_BASE_ADDR + offset);
}

static inline void ntt_reg_write(uint32_t offset, uint32_t val)
{
	*(volatile uint32_t *)((uintptr_t)NTT_REG_BASE_ADDR + offset) = val;
}

// For 32-bit mode, write 32 bits directly to RAM from a 32-bit variable
static inline void ntt_write_ram(uint32_t *ram_addr, uint32_t const *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        *(ram_addr + i) = data[i];
    }
}

// For 16-bit mode, we write only the lower 16 bits of each data element to RAM
static inline void ntt_write_ram16(uint16_t *ram_addr, uint16_t const *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i+=2) {
    	uint32_t temp = data[i + 1] << 16 | data[i];
        *(uint32_t *)(ram_addr + i) = temp;
    }
}

// For 32-bit mode, read 32 bits directly from RAM into a 32-bit variable
static inline void ntt_read_ram(uint32_t const *ram_addr, uint32_t *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        data[i] = *(ram_addr + i);
    }
}

// For 16-bit mode, we read 16 bits from RAM but store it in a 32-bit variable
static inline void ntt_read_ram16(uint16_t const *ram_addr, uint16_t *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i+=2) {
        uint32_t temp = *(uint32_t *)(ram_addr + i);
        data[i] = temp & 0xFFFF;
        data[i+1] = (temp >> 16) & 0xFFFF;
    }
}

static inline uint32_t is_16bit(uint32_t mr_value) {
    return (mr_value & NTTMR_SEL_MASK) == NTTMR_SEL_16BIT;
}

int32_t ntt_op(uint32_t mode)
{
    // Set NTT mode
    int32_t mr_value = ntt_reg_read(NTTMR_OFFSET) & ~NTTMR_MODE_MASK; // Clear mode bits
    mr_value |= mode; // Set desired mode
    ntt_reg_write(NTTMR_OFFSET, mr_value);

    // Enable NTT interrupt
    ntt_reg_write(NTTIER_OFFSET, NTTIER_IE_ENABLE);

    // Enable NTT operation
    ntt_reg_write(NTTCR_OFFSET, NTTCR_AE);

    // Wait for operation to complete
    while ((ntt_reg_read(NTTSR_OFFSET) & NTTSR_DONE_MASK) != NTTSR_DONE_DONE);
    ntt_reg_write(NTTSR_OFFSET, NTTSR_DONE_CLEAR); // Clear DONE status

    return NTT_SUCCESS; // Success
}

int32_t ntt_init(uint32_t const *omega, uint32_t omega_length, uint32_t mod_q, int32_t mr_value)
{
    if (omega_length > 0x1000) {
        return NTT_ERROR; // Invalid parameters
    }
    if (omega && omega_length != 0) {
        // Write omega values to RAM
        ntt_write_ram((uint32_t *)NTT_RAM3_BASE_ADDR, omega, omega_length);
    }
    // Set modulus Q
    ntt_reg_write(NTTMODQ_OFFSET, mod_q);
    // Set NTT mode and parameters
    ntt_reg_write(NTTMR_OFFSET, mr_value);

    g_is_16bit = is_16bit(mr_value);

    return NTT_SUCCESS; // Success
}

int32_t ntt_set_opa(void const *opa, uint32_t ram_off, uint32_t length) {
    if ((ram_off + length > NTT_MAX_RAM1_SIZE) || (ram_off < 0) || (length < 0)) {
        return NTT_ERROR;
    }
    if (opa != NULL) {
        if (g_is_16bit) {
            ntt_write_ram16((uint16_t *)(NTT_RAM1_BASE_ADDR + ram_off), (const uint16_t *)opa, length);
        } else {
            ntt_write_ram((uint32_t *)(NTT_RAM1_BASE_ADDR + ram_off), (const uint32_t *)opa, length);
        }
    }
    ntt_reg_write(NTTOPA_OFFSET, ram_off);
    return NTT_SUCCESS;
}

int32_t ntt_set_opb(void const *opb, uint32_t ram_off, uint32_t length) {
    if ((ram_off + length > NTT_MAX_RAM1_SIZE) || (ram_off < 0) || (length < 0)) {
        return NTT_ERROR;
    }
    if (opb != NULL) {
        if (g_is_16bit) {
            ntt_write_ram16((uint16_t *)(NTT_RAM1_BASE_ADDR + ram_off), (const uint16_t *)opb, length);
        } else {
            ntt_write_ram((uint32_t *)(NTT_RAM1_BASE_ADDR + ram_off), (const uint32_t *)opb, length);
        }
    }
    ntt_reg_write(NTTOPB_OFFSET, ram_off);
    return NTT_SUCCESS;
}

int32_t ntt_set_res_addr(uint32_t ram_off) {
    if (ram_off < 0) {
        return NTT_ERROR;
    }
    ntt_reg_write(NTTRES_OFFSET, ram_off);
    return NTT_SUCCESS;
}

int32_t ntt_read_res(void *res, uint32_t ram_off, uint32_t length) {
    if (res == NULL || length == 0 || length > 0x1000) {
        return NTT_ERROR; // Invalid parameters
    }
    if (g_is_16bit) {
        ntt_read_ram16((uint16_t const *)(NTT_RAM1_BASE_ADDR + ram_off), (uint16_t *)res, length);
    } else {
        ntt_read_ram((uint32_t const *)(NTT_RAM1_BASE_ADDR + ram_off), (uint32_t *)res, length);
    }
    return NTT_SUCCESS; // Success
}
