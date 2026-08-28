#ifndef NTT_REG_H
#define NTT_REG_H

#define NTT_REG_BASE_ADDR       0x40060000
#define NTT_RAM1_BASE_ADDR      0x50000000
#define NTT_RAM2_BASE_ADDR      0x50002000
#define NTT_RAM3_BASE_ADDR      0x50004000
#define NTT_MAX_RAM1_SIZE       0x2000

#define NTTCR_OFFSET        0x00
#define NTTMR_OFFSET        0x04
#define NTTIER_OFFSET       0x08
#define NTTSR_OFFSET        0x0C
#define NTTMODQ_OFFSET      0x10
#define NTTOPA_OFFSET       0x14
#define NTTOPB_OFFSET       0x18
#define NTTRES_OFFSET       0x1C
#define NTTRAM2_OFFSET      0x20

#define NTT_REG_NTTCR           (NTT_REG_BASE_ADDR + NTTCR_OFFSET)
#define NTT_REG_NTTMR           (NTT_REG_BASE_ADDR + NTTMR_OFFSET)
#define NTT_REG_NTTIER          (NTT_REG_BASE_ADDR + NTTIER_OFFSET)
#define NTT_REG_NTTSR           (NTT_REG_BASE_ADDR + NTTSR_OFFSET)
#define NTT_REG_NTTMODQ         (NTT_REG_BASE_ADDR + NTTMODQ_OFFSET)
#define NTT_REG_NTTOPA          (NTT_REG_BASE_ADDR + NTTOPA_OFFSET)
#define NTT_REG_NTTOPB          (NTT_REG_BASE_ADDR + NTTOPB_OFFSET)
#define NTT_REG_NTTRES          (NTT_REG_BASE_ADDR + NTTRES_OFFSET)
#define NTT_REG_NTTRAM2         (NTT_REG_BASE_ADDR + NTTRAM2_OFFSET)

/* *************************** */
/* NTT Control(NTTCR) Register */
/* *************************** */
#define NTTCR_AE            (1 << 0)    // NTT Enable

/* *************************** */
/* NTT Mode(NTTMR) Register    */
/* *************************** */
#define NTTMR_MODE_SHIFT        0
#define NTTMR_MODE_MASK         (0b111)    // NTT mode mask
#define NTTMR_MODE_NTT          (0x00000000)    // NTT mode
#define NTTMR_MODE_INTT         (0x00000001)    // INTT mode
#define NTTMR_MODE_PWM7         (0x00000002)    // PWM7 mode
#define NTTMR_MODE_PWM8         (0x00000003)    // PWM8 mode
#define NTTMR_MODE_POLY_ADD     (0x00000004)    // POLY_ADD mode
#define NTTMR_MODE_POLY_SUB     (0x00000005)    // POLY_SUB mode

#define NTTMR_COENUM_SHIFT      3
#define NTTMR_COENUM_MASK       (0b11 << NTTMR_COENUM_SHIFT)    // COENUM mask
#define NTTMR_COENUM_256        (0b00 << NTTMR_COENUM_SHIFT)    // COENUM_256
#define NTTMR_COENUM_512        (0b01 << NTTMR_COENUM_SHIFT)    // COENUM_512
#define NTTMR_COENUM_1024       (0b10 << NTTMR_COENUM_SHIFT)    // COENUM_1024

#define NTTMR_LOOP_SHIFT        5
#define NTTMR_LOOP_MASK         (0b11 << NTTMR_LOOP_SHIFT)
#define NTTMR_LOOP_8            (0b00 << NTTMR_LOOP_SHIFT)    // LOOP_8
#define NTTMR_LOOP_7            (0b01 << NTTMR_LOOP_SHIFT)    // LOOP_7
#define NTTMR_LOOP_6            (0b10 << NTTMR_LOOP_SHIFT)    // LOOP_6
#define NTTMR_LOOP_5            (0b11 << NTTMR_LOOP_SHIFT)    // LOOP_5

#define NTTMR_SEL_SHIFT         7
#define NTTMR_SEL_MASK          (0b1 << NTTMR_SEL_SHIFT)    // SEL mask
#define NTTMR_SEL_16BIT         (0b0 << NTTMR_SEL_SHIFT)    // SEL_16BIT
#define NTTMR_SEL_32BIT         (0b1 << NTTMR_SEL_SHIFT)    // SEL_32BIT

#define NTTMR_PARAM_D_SHIFT     8
#define NTTMR_PARAM_D_MASK      (0b1111 << NTTMR_PARAM_D_SHIFT)    // PARAM_D mask
#define NTTMR_PARAM_D_8         (0b0000 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_8
#define NTTMR_PARAM_D_9         (0b0001 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_9
#define NTTMR_PARAM_D_10        (0b0010 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_10
#define NTTMR_PARAM_D_11        (0b0011 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_11
#define NTTMR_PARAM_D_12        (0b0100 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_12
#define NTTMR_PARAM_D_13        (0b0101 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_13
#define NTTMR_PARAM_D_14        (0b0110 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_14
#define NTTMR_PARAM_D_15        (0b0111 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_15
#define NTTMR_PARAM_D_16        (0b1000 << NTTMR_PARAM_D_SHIFT)    // PARAM_D_16

#define NTTMR_REPEAT_SHIFT      12
#define NTTMR_REPEAT_MASK       (0b1111 << NTTMR_REPEAT_SHIFT)    // REPEAT mask

/* **************************************** */
/* NTT Interrupt Enable(NTTIER) Register    */
/* **************************************** */
#define NTTIER_IE_SHIFT         0
#define NTTIER_IE_MASK          (0b1 << NTTIER_IE_SHIFT)    // IE mask
#define NTTIER_IE_DISABLE       (0b0 << NTTIER_IE_SHIFT)    // IE disable
#define NTTIER_IE_ENABLE        (0b1 << NTTIER_IE_SHIFT)    // IE enable

/* ***************************** */
/* NTT Status(NTTSR) Register    */
/* ***************************** */
#define NTTSR_DONE_SHIFT          0
#define NTTSR_DONE_MASK           (0b1 << NTTSR_DONE_SHIFT)    // DONE mask
#define NTTSR_DONE_CLEAR          (0b0 << NTTSR_DONE_SHIFT)    // DONE clear
#define NTTSR_DONE_DONE           (0b1 << NTTSR_DONE_SHIFT)    // Operation DONE, read-only

/* ****************************** */
/* NTT Mod-Q(NTTMODQ) Register    */
/* ****************************** */
#define NTTMODQ_MODQ_SHIFT        0
#define NTTMODQ_MODQ_MASK         (0xFFFFFFFF << NTTMODQ_MODQ_SHIFT)    // MODQ mask

/* *************************************** */
/* NTT Operator-A Addr(NTTOPA) Register    */
/* *************************************** */
#define NTTOPA_OPA_SHIFT          2
#define NTTOPA_OPA_MASK           (0xFFF << NTTOPA_OPA_SHIFT)    // OPA mask
#define NTT_OPA_ADDR              (0x0000)    // OPA address (word address)
#define NTT_RAM1_BASE_OFF         (0x0000)    // OPA address (word address)

/* *************************************** */
/* NTT Operator-B Addr(NTTOPB) Register    */
/* *************************************** */
#define NTTOPB_OPB_SHIFT          2
#define NTTOPB_OPB_MASK           (0xFFF << NTTOPB_OPB_SHIFT)    // OPB mask
#define NTT_OPB_ADDR              (0x1000)    // OPB address (word address)

/* ******************************** */
/* NTT RES Addr(NTTRES) Register    */
/* ******************************** */
#define NTTRES_RES_SHIFT          2
#define NTTRES_RES_MASK           (0xFFF << NTTRES_RES_SHIFT)    // RES mask
#define NTT_RES_ADDR              (0x0000)    // RES address (word address)

/* ******************************** */
/* NTT RAM2(NTTRAM2) Register    */
/* ******************************** */
#define NTTRAM2_RAM2_SHIFT        0
#define NTTRAM2_RAM2_MASK         (0b1 << NTTRAM2_RAM2_SHIFT)    // RAM2 mask
#define NTTRAM2_RAM2_DISABLE      (0b0 << NTTRAM2_RAM2_SHIFT)    // RAM2 Disable
#define NTTRAM2_RAM2_ENABLE       (0b1 << NTTRAM2_RAM2_SHIFT)    // RAM2 Enable

#endif // NTT_REG_H
