#include "riscvcpu.h"

#define MSECURITY 0x7F2
#define SECKRY    0x05EC0000
#define SECKEY 0x05EC
#define MSECURITY_PEI BIT(6)
#define MSECURITY_PFD BIT(5)
#define MSECURITY_KSC BIT(4)
#define MSECURITY_UIT BIT(3)
#define MSECURITY_RII BIT(2)
#define MSECURITY_CCT BIT(1)
#define MSECURITY_ERESET BIT(0)



#define DEBUG
#ifdef DEBUG
#define debug(argc, argv...) ({printf(argc, ##argv);})
#else
#define debug(argc, argv...)
#endif



/* Security Control */
typedef union {
    struct {
        __IOM uint32_t Request_value0_index : 2;            /* bit location 1:0 */
        __IOM uint32_t Request_value1_index : 2;            /* bit location 3:2 */
        __IM  uint32_t RSV0 : 3;                            /* bit location 6:4 */
        __IOM uint32_t dppi_random_enable : 1;              /* bit location 7 */
        __IOM uint32_t acg_random_enable : 1;               /* bit location 8 */
        __IOM uint32_t security_irq_enable : 1;             /* bit location 9 */
        __IOM uint32_t vector_remap_enable : 1 ;            /* bit location 10 */
        __IOM uint32_t quick_key_states_clear : 1;          /* bit location 11 */
        __IOM uint32_t key_states_clear_on_IRQ : 1 ;        /* bit location 12 */
        __IOM uint32_t key_states_clear_on_exception : 1;   /* bit location 13 */
        __IM  uint32_t RSV1 : 2;                            /* bit location 15:14 */
        __IOM uint32_t alu2_tr_wbck_enable:1;               /* bit location 16 */
        __IOM uint32_t div_distb_enable : 1;                /* bit location 17 */
        __IOM uint32_t mul_distb_enable :1 ;                /* bit location 18 */
        __IOM uint32_t alu1_distb_enable :1;                /* bit location 19 */
        __IOM uint32_t alu2_distb_enable :1;                /* bit location 20 */
        __IOM uint32_t alu1_flush_enable : 1 ;              /* bit location 21 */
        __IOM uint32_t alu2_flush_enable :1;                /* bit location 22 */
        __IOM uint32_t div_pcmps_enable : 1 ;               /* bit location 23 */
    } b;
    __IOM uint32_t d;
}SEC_CTRL_REG_Typedef;

/* Security Status */
typedef union {
    struct {
        __IOM uint32_t RF_parity_error : 1;                      /* bit location 0 */
        __IOM uint32_t Pipe_parity_error : 1;                    /* bit location 1 */
        __IOM uint32_t CSR_parity_error : 1;                     /* bit location 2 */
        __IOM uint32_t CLIC_parity_error : 1;                    /* bit location 3 */
		__IOM uint32_t PMP_parity_error : 1;                     /* bit location 4 */
		__IOM uint32_t sPMP_parity_error : 1;                    /* bit location 5 */
		__IOM uint32_t CSR_lock_error : 1;                       /* bit location 6 */
		__IOM uint32_t CLIC_lock_error : 1;                      /* bit location 7 */
		__IOM uint32_t Timer_lock_error : 1;                     /* bit location 8 */
		__IOM uint32_t PMP_lock_error : 1;                       /* bit location 9 */
		__IOM uint32_t sPMP_lock_error : 1;                      /* bit location 10 */
		__IOM uint32_t PPI_lock_error : 16;                      /* bit location 26:11 */
		__IOM uint32_t BBOX_lock_error : 1;                      /* bit location 27 */
		__IOM uint32_t MPU_lock_error : 1;                       /* bit location 28 */
		__IOM uint32_t ILM_DLM_ICACHE_DCACHE_single_bit_error: 1;/* bit location 29 */
		__IOM uint32_t ILM_DLM_ICACHE_DCACHE_double_bit_error: 1;/* bit location 30 */
		__IOM uint32_t sec_ctrl_lock_error: 1;                   /* bit location 31 */

    } b;
    __IOM uint32_t d;
}SEC_STA_REG_Typedef;

/* Lock Control 0 */
typedef union {
    struct {
        __IOM uint32_t lock_CSR : 1;                                  /* bit location 0 */
        __IOM uint32_t lock_Timer : 1;                                /* bit location 1 */
        __IOM uint32_t lock_CLIC_cfg : 1;                             /* bit location 2 */
        __IOM uint32_t lock_PPI : 16;                                 /* bit location 18:3 */
		__IOM uint32_t CLIC_per_source_select : 5;                    /* bit location 23:19 */
		__IOM uint32_t CLIC_per_source_select_for_detaild_bits : 1;   /* bit location 24 */
		__IOM uint32_t lock_clic_mth : 1;                             /* bit location 25 */
		__IOM uint32_t lock_sec_ctrl_region : 1;                      /* bit location 26 */
    } b;
    __IOM uint32_t d;
}SEC_LOCK_CTRL0_REG_Typedef;

/* Random Instruction Insertion control */
typedef union {
    struct {
		__IOM uint32_t RIICN : 24;                                /* bit location 23:0 */
        __IM  uint32_t RSV0 : 6;                                  /* bit location 29:24 */
		__IOM uint32_t RIICM_enable : 1;                          /* bit location 30 */
		__IOM uint32_t RIR_count_enable : 1;                      /* bit location 31 */
    } b;
    __IOM uint32_t d;
}SEC_RANDOM_INSTRUCT_INSERT_REG_Typedef;

/* Threshold Value 0 */
typedef union {
    struct {
		__IOM uint32_t RII_threshold : 8;                         /* bit location 7:0 */
    __IOM uint32_t ALU2_TR_WBCK_threshold : 8;                /* bit location 15:8 */
		__IOM uint32_t DIV_DISTB_threshold : 8;                   /* bit location 23:16 */
		__IOM uint32_t MUL_DISTB_threshold : 8;                   /* bit location 31:24 */
    } b;
    __IOM uint32_t d;
}SEC_THRESHOLD_VALUE0_REG_Typedef;

/* Threshold Value 1 */
typedef union {
    struct {
		__IOM uint32_t ALU1_DISTB_threshold : 8;                  /* bit location 7:0 */
        __IOM uint32_t ALU2_DISTB_threshold : 8;                  /* bit location 15:8 */
		__IOM uint32_t ALU1_FLUSH_threshold : 8;                  /* bit location 23:16 */
		__IOM uint32_t ALU2_FLUSH_threshold : 8;                  /* bit location 31:24 */
    } b;
    __IOM uint32_t d;
}SEC_THRESHOLD_VALUE1_REG_Typedef;

/* Vector Remap Base for mtvt */
typedef union {
    struct {
		__IOM uint32_t vector_table_remap_base : 30;              /* bit location 31:2 */
    } b;
    __IOM uint32_t d;
}SEC_VECTOR_REMAP_BASE_FOR_MTVT_REG_Typedef;

/* Vector Remap Base for mtvec */
typedef union {
    struct {
		__IOM uint32_t common_entry_remap_base : 30;              /* bit location 31:2 */
    } b;
    __IOM uint32_t d;
}SEC_VECTOR_REMAP_BASE_FOR_MTVEC_REG_Typedef;

/* Vector Remap Base for stvt */
typedef union {
    struct {
		__IOM uint32_t common_entry_remap_base : 30;              /* bit location 31:2 */
    } b;
    __IOM uint32_t d;
}SEC_VECTOR_REMAP_BASE_FOR_STVT_REG_Typedef;

/* Vector Remap Base for stvec */
typedef union {
    struct {
		__IOM uint32_t common_entry_remap_base : 30;              /* bit location 31:2 */
    } b;
    __IOM uint32_t d;
}SEC_VECTOR_REMAP_BASE_FOR_STVEC_REG_Typedef;

/* Key Clear Return PC */
typedef union {
    struct {
		__IOM uint32_t Return_PC : 30;                            /* bit location 31:2 */
    } b;
    __IOM uint32_t d;
}SEC_KEY_CLEAR_RETURN_PC_REG_Typedef;

/* Auth Control */
typedef union {
    struct {
        __IOM uint32_t authenticated : 1;                      /* bit location 0 */
        __IOM uint32_t authbusy : 1;                           /* bit location 1 */
        __IOM uint32_t auth_irq : 1;                           /* bit location 2 */
        __IOM uint32_t debug_disable : 1;                      /* bit location 3 */
		__IOM uint32_t debug_autolock : 1;                     /* bit location 4 */
		__IOM uint32_t auth_irqmask : 1;                       /* bit location 5 */
		__IM  uint32_t RSV0 : 10;                              /* bit location 15:6 */
		__IOM uint32_t Password : 16;                          /* bit location 31:16 */
    } b;
    __IOM uint32_t d;
}SEC_AUTH_CTRL_REG_Typedef;

/* BBOX x Start */
typedef union {
    struct {
        __IOM uint32_t bbox_en : 1;                            /* bit location 0 */
        __IOM uint32_t bbox_lock : 1;                          /* bit location 1 */
		__IM  uint32_t RSV0 : 3;                               /* bit location 4:2 */
		__IOM uint32_t bbox_start : 27;                        /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_BBOXx_START_REG_Typedef;

/* BBOX x End */
typedef union {
    struct {
		__IM  uint32_t RSV0 : 5;                               /* bit location 4:2 */
		__IOM uint32_t bbox_end : 27;                          /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_BBOXx_END_REG_Typedef;

/* MPU Region x Configuration */
typedef union {
    struct {
		__IOM uint32_t mpu_ren : 1;                            /* bit location 0 */
		__IOM uint32_t mpu_rx : 1;                             /* bit location 1 */
		__IOM uint32_t mpu_rr : 1;                             /* bit location 2 */
		__IOM uint32_t mpu_rw : 1;                             /* bit location 3 */
		__IM  uint32_t RSV0 : 4;                               /* bit location 7:4 */
		__IOM uint32_t mpu_rbind : 8;                          /* bit location 15:8 */
		__IM  uint32_t RSV1 : 12;                              /* bit location 27:16 */
		__IOM uint32_t mpu_rlock : 4;                          /* bit location 31:28 */
    } b;
    __IOM uint32_t d;
}SEC_MPU_REGIONx_CONFIG_REG_Typedef;

/* MPU Region x Start */
typedef union {
    struct {
		__IM  uint32_t RSV0 : 5;                               /* bit location 4:0 */
		__IOM uint32_t mpu_rstart : 27;                        /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_MPU_REGIONx_START_REG_Typedef;

/* MPU Region x End */
typedef union {
    struct {
		__IM  uint32_t RSV0 : 5;                               /* bit location 4:0 */
		__IOM uint32_t mpu_rend : 27;                        /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_MPU_REGIONx_END_REG_Typedef;


/* Security machine mode stack overflow check top */
typedef union {
    struct {
		__IOM  uint32_t stack_check_enable : 1;                 /* bit location 0 */
    __IM  uint32_t RSV0 : 4;                               /* bit location 4:1 */
		__IOM uint32_t top_addr : 27;                          /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_MACHINE_M_STACK_OVER_CHK_TOP_REG_Typedef;

/* Security machine mode stack overflow check buttom */
typedef union {
    struct {
    __IM  uint32_t RSV0 : 5;                               /* bit location 4:0 */
		__IOM uint32_t buttom_addr : 27;                       /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_MACHINE_M_STACK_OVER_CHK_BUTTOM_REG_Typedef;

/* Security supervisor mode stack overflow check top */
typedef union {
    struct {
		__IOM  uint32_t stack_check_enable : 1;                 /* bit location 0 */
    __IM  uint32_t RSV0 : 4;                               /* bit location 4:1 */
		__IOM uint32_t top_addr : 27;                          /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_SUPERVISOR_M_STACK_OVER_CHK_TOP_REG_Typedef;

/* Security supervisor mode stack overflow check buttom */
typedef union {
    struct {
    __IM  uint32_t RSV0 : 5;                               /* bit location 4:0 */
		__IOM uint32_t buttom_addr : 27;                       /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_SUPERVISOR_M_STACK_OVER_CHK_BUTTOM_REG_Typedef;

/* Security user mode stack overflow check top */
typedef union {
    struct {
		__IOM  uint32_t stack_check_enable : 1;                 /* bit location 0 */
    __IM  uint32_t RSV0 : 4;                               /* bit location 4:1 */
		__IOM uint32_t top_addr : 27;                          /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_USER_M_STACK_OVER_CHK_TOP_REG_Typedef;

/* Security user mode stack overflow check buttom */
typedef union {
    struct {
    __IM  uint32_t RSV0 : 5;                               /* bit location 4:0 */
		__IOM uint32_t buttom_addr : 27;                       /* bit location 31:5 */
    } b;
    __IOM uint32_t d;
}SEC_USER_M_STACK_OVER_CHK_BUTTOM_REG_Typedef;


/* Request Type*/
enum RII_REQTYPE{RequestValue0 = 0,RequestValue1};
/* Mapping Rule*/
enum RII_MAP{RII_REQ = 0,ALU2_TR_WBCK_REQ,DIV_DISTB_REQ,MUL_DISTB_REQ,ALU1_DISTB_REQ,ALU2_DISTB_REQ,ALU1_FLUSH_REQ,ALU2_FLUSH_REQ};
/*RII Typedfe*/
typedef struct{
  enum RII_REQTYPE RequestType;
  enum RII_MAP     RequestMap;
  uint8_t          RIICNENABLE;
  uint32_t          RIICNNUM;
  uint8_t         threshold;
}RII_Typedef;


// Security IRQ Enable Reg
#define RF_PARITY_ERR_INT         BIT(0)
#define PIPE_PARITY_ERR_INT       BIT(1)
#define CSR_PARITY_ERR_INT        BIT(2)
#define CLIC_PARITY_ERR_INT       BIT(3)
#define PMP_PARITY_ERR_INT        BIT(4)
#define SPMP_PARITY_ERR_INT       BIT(5)
#define CSR_LOCK_ERR_INT          BIT(6)
#define CLIC_LOCK_ERR_INT         BIT(7)
#define TIMER_LOCK_ERR_INT        BIT(8)
#define PMP_LOCK_ERR_INT          BIT(9)
#define SPMP_LOCK_ERR_INT         BIT(10)
#define PPI_LOCK_ERR_INT(regval)  (BITS(11,26) & ((uint32_t)(regval) << 11))
#define BBOX_LOCK_ERR_INT         BIT(27)
#define MPU_LOCK_ERR_INT          BIT(28)
#define SEC_CTRL_LOCK_ERR_INT     BIT(31)

// Security IRQ Enable 1 Reg
#define MACHINE_M_STACK_OVER_INT      BIT(2)
#define SUPERVISOR_M_STACK_OVER_INT   BIT(3)
#define USER_M_STACK_OVER_INT         BIT(4)

// Security Status Reg
#define CSR_LOCK_ERR_FLG          BIT(6)
#define CLIC_LOCK_ERR_FLG         BIT(7)
#define TIMER_LOCK_ERR_FLG        BIT(8)
#define PMP_LOCK_ERR_FLG          BIT(9)
#define SPMP_LOCK_ERR_FLG         BIT(10)
#define BBOX_LOCK_ERR_FLG         BIT(27)
#define MPU_LOCK_ERR_FLG          BIT(28)
#define SEC_CTRL_LOCK_ERR_FLG     BIT(31)

// Security Status1 Reg
#define MACHINE_M_STACK_OVER_ERR_FLG      BIT(2)
#define SUPERVISOR_M_STACK_OVER_ERR_FLG   BIT(3)
#define USER_M_STACK_OVER_ERR_FLG         BIT(4)

// Security Status Reg
#define LOCK_CSR                  BIT(0)
#define LOCK_TIMER                BIT(1)
#define LOCK_CLIC_CFG             BIT(2)
#define LOCK_CLIC_MTH             BIT(25)
#define LOCK_SEC_CTRL_REGION      BIT(26)

#define __SPMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __SPMP_ENTRY_NUM           8                    /*!< Set to 8 or 16, the number of PMP entries */

#define SEC_GET_STATUS(__SEC__,__FLAG__) (((__SEC__)->Sec_Status & (__FLAG__)) == (__FLAG__))
#define SEC_CLEAR_STATUS(__SEC__,__FLAG__) (((__SEC__)->Sec_Status |= (__FLAG__) ))
#define SEC_GET_STATUS1(__SEC__,__FLAG__) (((__SEC__)->Security_Status1 & (__FLAG__)) == (__FLAG__))
#define SEC_CLEAR_STATUS1(__SEC__,__FLAG__) (((__SEC__)->Security_Status1 |= (__FLAG__) ))
#define MPU_CFG_LOCK_ENABLE(num, val)   (REG16(SEC_CTRL_BASE+0x160+num*12 + 1) = (val << 4))


void CSRMsecurityEnable(uint32_t data);
void lock_enable(SEC_CTRL_TypeDef *pSecCtrl, uint32_t lockType);
void Sec_IRQ_Enable(SEC_CTRL_TypeDef *pSecCtrl, uint32_t intType, uint8_t Num);
void RII_Insert_Enable(RII_Typedef re,int RSx);
void debug_bbox_lock_and_en_config(uint8_t bbox_num, uint8_t box_lock, uint8_t box_en);
uint32_t read_bbox_start_addr(uint8_t bbox_num);
uint32_t read_bbox_end_addr(uint8_t bbox_num);
void write_bbox_start_addr(uint8_t bbox_num, uint32_t start_addr_val);
void write_bbox_end_addr(uint8_t bbox_num, uint32_t end_addr_val);
void write_mpu_r_end_addr(uint8_t mpu_rnum, uint32_t end_addr_val);
uint32_t read_mpu_r_end_addr(uint8_t mpu_rnum);
void write_mpu_r_start_addr(uint8_t mpu_rnum, uint32_t start_addr_val);
uint32_t read_mpu_r_start_addr(uint8_t mpu_rnum);
uint32_t read_mpu_r_configuration(uint8_t mpu_rnum);
void config_mpu_r_configuration(uint8_t mpu_rnum, uint8_t mpu_rlock, uint8_t mpu_rbind, uint8_t mpu_rw, uint8_t mpu_rr, uint8_t mpu_rx, uint8_t mpu_ren);
uint8_t __get_SPMPxCFG(uint32_t idx);
void __set_SPMPxCFG(uint32_t idx, uint8_t spmpxcfg);
rv_csr_t __get_SPMPCFGx(uint32_t idx);
void __set_SPMPCFGx(uint32_t idx, rv_csr_t spmpcfg);
rv_csr_t __get_SPMPADDRx(uint32_t idx);
void __set_SPMPADDRx(uint32_t idx, rv_csr_t spmpaddr);
