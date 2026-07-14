#ifndef __SHA3_H__
#define __SHA3_H__

#define SHA3CHAIN_BASE_ADDR    0x40400000

#define SCCFG_OFFSET        0x00
#define SCCR_OFFSET         0x04
#define SCSR_OFFSET         0x08
#define SCIER_OFFSET        0x0C
#define SCMR_OFFSET         0x10
#define SCDMACNT_OFFSET     0x14
#define SCLEN_OFFSET        0x18
#define SCDR_OFFSET         0x1C
#define SCITERCNT_OFFSET    0x20

#define SCCFG_DMA_EN         (1U << 0)
#define SCCFG_SOFT_PADDING   (1U << 1)
#define SCCFG_WR_RAM_EN      (1U << 2)
#define SCCFG_SHA3_CLR       (1U << 4)
#define SCCFG_BYTE_LITEND    (1U << 5)
#define SCCFG_ASCII_EN       (1U << 6)

#define LITTLE_ENDIAN       1
#define BIG_ENDIAN          0

#define DMA_EN              1
#define CPU_EN              0

#define SOFT_PADDING        1
#define HW_PADDING          0




// ==============================
// SCCR
// ==============================
#define SCCR_AE_SHAKE        (1U << 1)
#define SCCR_AE              (1U << 0)

// ==============================
// SCSR
// ==============================
#define SCSR_DONE            (1U << 0)
#define SCSR_LOOP_DONE       (1U << 1)

// ==============================
// SCMR
// ==============================
#define SCMR_CHAIN_SEL       (1U << 3)
#define SCMR_NO_CHAIN       (0U << 3)

#define SCMR_SHAKE256        0x5
#define SCMR_SHAKE128        0x4
#define SCMR_SHA3_512        0x3
#define SCMR_SHA3_384        0x2
#define SCMR_SHA3_256        0x1
#define SCMR_SHA3_224        0x0



#define SHA3_BLOCK_SIZE_224             144
#define SHA3_BLOCK_SIZE_256             136
#define SHA3_BLOCK_SIZE_384             104
#define SHA3_BLOCK_SIZE_512              72
#define SHA3_BLOCK_SIZE_SHAKE128        168
#define SHA3_BLOCK_SIZE_SHAKE256        136  

#define SHA3_HASH_SIZE_224              28
#define SHA3_HASH_SIZE_256              32
#define SHA3_HASH_SIZE_384              48
#define SHA3_HASH_SIZE_512              64
#define SHA3_HASH_SIZE_SHAKE128         168
#define SHA3_HASH_SIZE_SHAKE256         136

typedef struct {
    uint8_t block_size;   
    uint8_t padding;      
    uint8_t output_size;  
    uint8_t scmr_val;     
} sha3_alg_params_t;

// ==============================
 // Register access inline wrapper, reduce function call overhead
 // ==============================
static inline int32_t sha3_reg_read(uint32_t offset)
 {
	 return *(volatile uint32_t *)(uintptr_t)(SHA3CHAIN_BASE_ADDR + offset);
 }
 
static inline void sha3_reg_write(uint32_t offset, uint32_t val)
 {
	 *(volatile uint32_t *)(uintptr_t)(SHA3CHAIN_BASE_ADDR + offset) = val;

 }



void sha3_set_hw_config(uint8_t is_little_endian, uint8_t use_dma, uint8_t use_soft_padding);



#endif

