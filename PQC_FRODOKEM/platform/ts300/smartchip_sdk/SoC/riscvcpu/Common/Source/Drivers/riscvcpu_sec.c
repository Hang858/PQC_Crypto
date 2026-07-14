#include "riscvcpu_sec.h"
#include <stdio.h>
#include <time.h>
#include "malloc.h"




/**
 * \brief Enable CSR MSECURITY register
 * \param data: enable the corresponding field
 */

void CSRMsecurityEnable(uint32_t data)
{
      __RV_CSR_SET(MSECURITY,SECKRY|data);
}


/**
 * \brief enable normal lock
 *
 * \param pSecCtrl: point to sec_ctrl reg struct
 * \param lockType: lock type
 *    \arg LOCK_CSR: lock CSR
 *    \arg LOCK_TIMER: lock Timer
 *    \arg LOCK_CLIC_CFG: lock CLIC cfg
 *    \arg LOCK_CLIC_MTH: lock CLIC mth
 *    \arg LOCK_SEC_CTRL_REGION: lock sec_ctrl region
 */
void lock_enable(SEC_CTRL_TypeDef *pSecCtrl, uint32_t lockType)
{
    SEC_LOCK_CTRL0_REG_Typedef sec_lock_ctrl0;

    sec_lock_ctrl0.d = pSecCtrl->Lock_Ctrl0;
    sec_lock_ctrl0.d |= lockType;
    pSecCtrl->Lock_Ctrl0 = sec_lock_ctrl0.d;
}

/**
 * \brief enable lock interrupt
 *
 * \param pSecCtrl: point to sec_ctrl reg struct
 * \param intType: interrupt type
 *    \arg CSR_LOCK_ERR_INT: CSR lock error interrupt
 *    \arg CLIC_LOCK_ERR_INT: CLIC lock error interrupt
 *    \arg TIMER_LOCK_ERR_INT: Timer lock error interrupt
 *    \arg PMP_LOCK_ERR_INT: PMP lock error interrupt
 *    \arg SPMP_LOCK_ERR_INT: sPMP lock error interrupt
 *    \arg SEC_CTRL_LOCK_ERR_INT: sec_ctrl lock error interrupt
 *    \arg BBOX_LOCK_ERR_INT : bbox lock error interrupt
 *    \arg MPU_LOCK_ERR_INT : MPU lock error interrupt
 * \param Num: 0-Security IRQ Enable register; 1-Security IRQ Enable 1 register
 */
void Sec_IRQ_Enable(SEC_CTRL_TypeDef *pSecCtrl, uint32_t intType, uint8_t Num)
{
    if (Num == 0) {
        pSecCtrl->Security_IRQ |= intType;
    }else if (Num == 1) {
        pSecCtrl->Security_IRQ1 |= intType;
    }
}

/**
 * \brief enable/disable bbox_x lock, enable/disable bbox_x debug
 *
 * \param bbox_num: select bbox number from 0 to 7
 * \param box_lock: set 1 to lock, set 0 to unlock
 * \param box_en: set 1 to enable, set 0 to disable
 */
void debug_bbox_lock_and_en_config(uint8_t bbox_num, uint8_t box_lock, uint8_t box_en)
{
    SEC_BBOXx_START_REG_Typedef sec_bbox_start;
    if(bbox_num == 0) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_0_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_0_Start = sec_bbox_start.d;
    }else if(bbox_num == 1) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_1_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_1_Start = sec_bbox_start.d;
    }else if(bbox_num == 2) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_2_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_2_Start = sec_bbox_start.d;
    }else if(bbox_num == 3) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_3_Start;
        sec_bbox_start.b.bbox_lock = 1;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_3_Start = sec_bbox_start.d;
    }else if(bbox_num == 4) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_4_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_4_Start = sec_bbox_start.d;
    }else if(bbox_num == 5) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_5_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_5_Start = sec_bbox_start.d;
    }else if(bbox_num == 6) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_6_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_6_Start = sec_bbox_start.d;
    }else if(bbox_num == 7) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_7_Start;
        sec_bbox_start.b.bbox_lock = box_lock;
        sec_bbox_start.b.bbox_en = box_en;
        SEC_CTRL->BBOX.BBOX_7_Start = sec_bbox_start.d;
    }
}

/**
 * \brief read the addr start of bbox_x
 *
 * \param bbox_num: select bbox number from 0 to 7
 *
 * \retval bbox_x start addr, 32-bytes align
 */
uint32_t read_bbox_start_addr(uint8_t bbox_num)
{
    SEC_BBOXx_START_REG_Typedef sec_bbox_start;
    if(bbox_num == 0) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_0_Start;
    }else if(bbox_num == 1) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_1_Start;
    }else if(bbox_num == 2) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_2_Start;
    }else if(bbox_num == 3) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_3_Start;
    }else if(bbox_num == 4) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_4_Start;
    }else if(bbox_num == 5) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_5_Start;
    }else if(bbox_num == 6) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_6_Start;
    }else if(bbox_num == 7) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_7_Start;
    }else {
        return -1;
    }
    return sec_bbox_start.b.bbox_start << 5;
}

/**
 * \brief write the start addr of bbox_x
 *
 * \param bbox_num: select bbox number from 0 to 7
 * \param start_addr_val: bbox start addr, 32-bytes align
 */
void write_bbox_start_addr(uint8_t bbox_num, uint32_t start_addr_val)
{
    SEC_BBOXx_START_REG_Typedef sec_bbox_start;
    if(bbox_num == 0) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_0_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_0_Start = sec_bbox_start.d;
    }else if(bbox_num == 1) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_1_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_1_Start = sec_bbox_start.d;
    }else if(bbox_num == 2) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_2_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_2_Start = sec_bbox_start.d;
    }else if(bbox_num == 3) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_3_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_3_Start = sec_bbox_start.d;
    }else if(bbox_num == 4) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_4_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_4_Start = sec_bbox_start.d;
    }else if(bbox_num == 5) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_5_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_5_Start = sec_bbox_start.d;
    }else if(bbox_num == 6) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_6_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_6_Start = sec_bbox_start.d;
    }else if(bbox_num == 7) {
        sec_bbox_start.d = SEC_CTRL->BBOX.BBOX_7_Start;
        sec_bbox_start.b.bbox_start = start_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_7_Start = sec_bbox_start.d;
    }
}

/**
 * \brief read the end addr of bbox_x
 *
 * \param bbox_num: select bbox number from 0 to 7
 *
 * \retval bbox_x end addr value, 32-bytes align
 */
uint32_t read_bbox_end_addr(uint8_t bbox_num)
{
    SEC_BBOXx_END_REG_Typedef sec_bbox_end;
    if(bbox_num == 0) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_0_End;
    }else if(bbox_num == 1) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_1_End;
    }else if(bbox_num == 2) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_2_End;
    }else if(bbox_num == 3) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_3_End;
    }else if(bbox_num == 4) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_4_End;
    }else if(bbox_num == 5) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_5_End;
    }else if(bbox_num == 6) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_6_End;
    }else if(bbox_num == 7) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_7_End;
    }else {
        return -1;
    }
    return sec_bbox_end.b.bbox_end << 5;
}

/**
 * \brief write the end addr of bbox_x
 *
 * \param bbox_num: select bbox number from 0 to 7
 * \param end_addr_val: bbox end addr, 32-bytes align
 */
void write_bbox_end_addr(uint8_t bbox_num, uint32_t end_addr_val)
{
    SEC_BBOXx_END_REG_Typedef sec_bbox_end;
    if(bbox_num == 0) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_0_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_0_End = sec_bbox_end.d;
    }else if(bbox_num == 1) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_1_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_1_End = sec_bbox_end.d;
    }else if(bbox_num == 2) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_2_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_2_End = sec_bbox_end.d;
    }else if(bbox_num == 3) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_3_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_3_End = sec_bbox_end.d;
    }else if(bbox_num == 4) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_4_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_4_End = sec_bbox_end.d;
    }else if(bbox_num == 5) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_5_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_5_End = sec_bbox_end.d;
    }else if(bbox_num == 6) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_6_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_6_End = sec_bbox_end.d;
    }else if(bbox_num == 7) {
        sec_bbox_end.d = SEC_CTRL->BBOX.BBOX_7_End;
        sec_bbox_end.b.bbox_end = end_addr_val >> 5;
        SEC_CTRL->BBOX.BBOX_7_End = sec_bbox_end.d;
    }
}

/**
 * \brief configure mup region configuration
 *
 * \param mpu_rnum: select mpu from 0 to 7
 * \param mpu_rlock: set val is not 0x5 to lock
 * \param mpu_rbind: select the binding region num
 * \param mpu_rw: set 1 to enable write authority
 * \param mpu_rr: set 1 to enable read authority
 * \param mpu_rx: set 1 to enable execute authority
 * \param mpu_ren: set 1 to enable MPU region
 */
void config_mpu_r_configuration(uint8_t mpu_rnum, uint8_t mpu_rlock, uint8_t mpu_rbind, uint8_t mpu_rw, uint8_t mpu_rr, uint8_t mpu_rx, uint8_t mpu_ren)
{
    SEC_MPU_REGIONx_CONFIG_REG_Typedef sec_mpu_region_config;
    if(mpu_rnum == 0) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region0_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region0_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 1) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region1_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region1_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 2) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region2_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region2_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 3) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region3_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region3_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 4) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region4_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region4_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 5) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region5_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region5_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 6) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region6_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region6_Cfg = sec_mpu_region_config.d;
    }else if(mpu_rnum == 7) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region7_Cfg;
        sec_mpu_region_config.b.mpu_rlock = mpu_rlock;
        sec_mpu_region_config.b.mpu_rbind = mpu_rbind;
        sec_mpu_region_config.b.mpu_rw = mpu_rw;
        sec_mpu_region_config.b.mpu_rr = mpu_rr;
        sec_mpu_region_config.b.mpu_rx = mpu_rx;
        sec_mpu_region_config.b.mpu_ren = mpu_ren;
        SEC_CTRL->MPU_Region.MPU_Region7_Cfg = sec_mpu_region_config.d;
    }
}

/**
 * \brief read the configuration reg val of MPU region_x
 *
 * \param mpu_rnum: select region number from 0 to 7
 *
 * \retval MPU region_x configuration reg value
 */
uint32_t read_mpu_r_configuration(uint8_t mpu_rnum)
{
    SEC_MPU_REGIONx_CONFIG_REG_Typedef sec_mpu_region_config;
    if(mpu_rnum == 0) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region0_Cfg;
    }else if(mpu_rnum == 1) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region1_Cfg;
    }else if(mpu_rnum == 2) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region2_Cfg;
    }else if(mpu_rnum == 3) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region3_Cfg;
    }else if(mpu_rnum == 4) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region4_Cfg;
    }else if(mpu_rnum == 5) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region5_Cfg;
    }else if(mpu_rnum == 6) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region6_Cfg;
    }else if(mpu_rnum == 7) {
        sec_mpu_region_config.d = SEC_CTRL->MPU_Region.MPU_Region7_Cfg;
    }else {
        return -1;
    }
    return sec_mpu_region_config.d;
}

/**
 * \brief read the start addr of MPU region_x
 *
 * \param mpu_rnum: select region number from 0 to 7
 *
 * \retval MPU region_x start addr value, 32-bytes align, LSB4:0 is 0
 */
uint32_t read_mpu_r_start_addr(uint8_t mpu_rnum)
{
    SEC_MPU_REGIONx_START_REG_Typedef sec_mpu_region_start;
    if(mpu_rnum == 0) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region0_Start;
    }else if(mpu_rnum == 1) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region1_Start;
    }else if(mpu_rnum == 2) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region2_Start;
    }else if(mpu_rnum == 3) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region3_Start;
    }else if(mpu_rnum == 4) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region4_Start;
    }else if(mpu_rnum == 5) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region5_Start;
    }else if(mpu_rnum == 6) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region6_Start;
    }else if(mpu_rnum == 7) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region7_Start;
    }else {
        return -1;
    }
    return sec_mpu_region_start.b.mpu_rstart << 5;
}

/**
 * \brief write the start addr of MPU region_x
 *
 * \param mpu_rnum: select region number from 0 to 7
 * \param start_addr_val: MPU region_x start addr value, 32-bytes align, LSB4:0 is 0
 */
void write_mpu_r_start_addr(uint8_t mpu_rnum, uint32_t start_addr_val)
{
    SEC_MPU_REGIONx_START_REG_Typedef sec_mpu_region_start;
    if(mpu_rnum == 0) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region0_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region0_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 1) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region1_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region1_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 2) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region2_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region2_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 3) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region3_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region3_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 4) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region4_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region4_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 5) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region5_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region5_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 6) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region6_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region6_Start = sec_mpu_region_start.d;
    }else if(mpu_rnum == 7) {
        sec_mpu_region_start.d = SEC_CTRL->MPU_Region.MPU_Region7_Start;
        sec_mpu_region_start.b.mpu_rstart = start_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region7_Start = sec_mpu_region_start.d;
    }
}

/**
 * \brief read the end addr of MPU region_x
 *
 * \param mpu_rnum: select region number from 0 to 7
 *
 * \retval MPU region_x end addr value, 32-bytes align, LSB4:0 is 0
 */
uint32_t read_mpu_r_end_addr(uint8_t mpu_rnum)
{
    SEC_MPU_REGIONx_END_REG_Typedef sec_mpu_region_end;
    if(mpu_rnum == 0) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region0_End;
    }else if(mpu_rnum == 1) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region1_End;
    }else if(mpu_rnum == 2) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region2_End;
    }else if(mpu_rnum == 3) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region3_End;
    }else if(mpu_rnum == 4) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region4_End;
    }else if(mpu_rnum == 5) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region5_End;
    }else if(mpu_rnum == 6) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region6_End;
    }else if(mpu_rnum == 7) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region7_End;
    }else {
        return -1;
    }
    return sec_mpu_region_end.b.mpu_rend << 5;
}

/**
 * \brief write the end addr of MPU region_x
 *
 * \param mpu_rnum: select region number from 0 to 7
 * \param end_addr_val: MPU region_x end addr value, 32-bytes align, LSB4:0 is 0
 */
void write_mpu_r_end_addr(uint8_t mpu_rnum, uint32_t end_addr_val)
{
    SEC_MPU_REGIONx_END_REG_Typedef sec_mpu_region_end;
    if(mpu_rnum == 0) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region0_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region0_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 1) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region1_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region1_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 2) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region2_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region2_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 3) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region3_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region3_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 4) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region4_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region4_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 5) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region5_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region5_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 6) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region6_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region6_End = sec_mpu_region_end.d;
    }else if(mpu_rnum == 7) {
        sec_mpu_region_end.d = SEC_CTRL->MPU_Region.MPU_Region7_End;
        sec_mpu_region_end.b.mpu_rend = end_addr_val >> 5;
        SEC_CTRL->MPU_Region.MPU_Region7_End = sec_mpu_region_end.d;
    }
}

/**
 * \brief Random Instruction control Cfg
 *
 * \param rii_re: RII_Typedef to Cfg RRI
 * \param RSx   : point to RS1 or RS2
 *    \arg 0: ignore
 *    \arg 1: RS1
 *    \arg 2: RS2
 */

void RII_Insert_Enable(RII_Typedef rii_re,int RSx)
{
    SEC_CTRL->RRI_Counter = 0;

    SEC_RANDOM_INSTRUCT_INSERT_REG_Typedef rii;
    rii.d = SEC_CTRL->RII_ctrl;
    rii.b.RIR_count_enable =1;
    rii.b.RIICM_enable = rii_re.RIICNENABLE;
    rii.b.RIICN =rii_re.RIICNNUM;
    SEC_CTRL->RII_ctrl= rii.d;

    //printf("Begin counter num :%d\r\n",SEC_CTRL->RRI_Counter);
    //printf("Begin  RIICN num : %d\r\n", rii.b.RIICN );
    //printf("Begin  RIICM value :%d\r\n", rii.b.RIICM_enable);

    SEC_CTRL->Random_Load_Base = 0x80000000;
    if ((rii_re.RequestMap == ALU1_FLUSH_REQ) || (rii_re.RequestMap == ALU2_FLUSH_REQ)) {
        //RII & UIT enable
        __RV_CSR_SET(MSECURITY,SECKRY|BITS(2,3));
    }else {
        //RII enable
        __RV_CSR_SET(MSECURITY,SECKRY|BIT(2));
    }

    if(rii_re.RequestType == RequestValue0)
    {

          SEC_CTRL_REG_Typedef sec_ctrl;
          sec_ctrl.d = SEC_CTRL->Sec_Control;
          sec_ctrl.b.Request_value0_index= rii_re.RequestMap;
          SEC_CTRL->Sec_Control=sec_ctrl.d;

          sec_ctrl.d = SEC_CTRL->Sec_Control;
          if( rii_re.RequestMap != RII_REQ)
               sec_ctrl.d |= BIT(15 + rii_re.RequestMap);
          SEC_CTRL->Sec_Control = sec_ctrl.d;


          SEC_THRESHOLD_VALUE0_REG_Typedef threshold_val;
          threshold_val.d = SEC_CTRL->Threshold_value0;
          threshold_val.d =  rii_re.threshold << 8*rii_re.RequestMap;
          SEC_CTRL->Threshold_value0 = threshold_val.d ;

          SEC_CTRL->Request_value0= rand();
          if(rii_re.RequestMap == RII_REQ)
          {

               SEC_CTRL->rand_seed_value.RII = rand();
          }
          else if(rii_re.RequestMap == ALU2_TR_WBCK_REQ)
          {

               SEC_CTRL->rand_seed_value.ALU2_TR_WBCK_RES = rand();
          }
          else if(rii_re.RequestMap == DIV_DISTB_REQ)
          {

            if(RSx == 1)
                SEC_CTRL->rand_seed_value.DIV_DISTB_RS1 = rand();
            else if(RSx == 2)
                SEC_CTRL->rand_seed_value.DIV_DISTB_RS2 = rand();

          }
          else if(rii_re.RequestMap == MUL_DISTB_REQ)
          {

            if(RSx == 1)
                SEC_CTRL->rand_seed_value.MUL_DISTB_RS1 = rand();
            else if(RSx == 2)
                SEC_CTRL->rand_seed_value.MUL_DISTB_RS2 = rand();
          }
          else{

          }
    }
    else if(rii_re.RequestType == RequestValue1){


          SEC_CTRL_REG_Typedef sec_ctrl;
          sec_ctrl.d = SEC_CTRL->Sec_Control;
          sec_ctrl.b.Request_value1_index= rii_re.RequestMap - 4;
          SEC_CTRL->Sec_Control=sec_ctrl.d;

          sec_ctrl.d = SEC_CTRL->Sec_Control;
          sec_ctrl.d |= BIT(19 + rii_re.RequestMap - 4);
          SEC_CTRL->Sec_Control = sec_ctrl.d;

          SEC_THRESHOLD_VALUE1_REG_Typedef threshold_val;
          threshold_val.d = SEC_CTRL->Threshold_value1;
          threshold_val.d =  rii_re.threshold << 8*(rii_re.RequestMap - 4);
          SEC_CTRL->Threshold_value1 = threshold_val.d ;

          SEC_CTRL->Request_value1= rand();

          if(rii_re.RequestMap == ALU1_DISTB_REQ)
          {

            if(RSx == 1)
                SEC_CTRL->rand_seed_value.ALU1_DISTB_RS1 = rand();
            else if(RSx == 2)
                SEC_CTRL->rand_seed_value.ALU1_DISTB_RS2 = rand();
          }
          else if(rii_re.RequestMap == ALU2_DISTB_REQ){

            if(RSx == 1)
                SEC_CTRL->rand_seed_value.ALU2_DISTB_RS1 = rand();
            else if(RSx == 2)
                SEC_CTRL->rand_seed_value.ALU2_DISTB_RS2 = rand();
          }
          else{

          }
    }
}

/**
 * \brief   Get 8bit SPMPxCFG Register by SPMP entry index
 * \details Return the content of the SPMPxCFG Register.
 * \param [in]    idx    SPMP region index(0-15)
 * \return               SPMPxCFG Register value
 */
uint8_t __get_SPMPxCFG(uint32_t idx)
{
    rv_csr_t spmpcfg = 0;

    if (idx >= __SPMP_ENTRY_NUM) return 0;
#if __RISCV_XLEN == 32
    if (idx < 4) {
        spmpcfg = __RV_CSR_READ(CSR_SPMPCFG0);
    } else if ((idx >=4) && (idx < 8)) {
        idx -= 4;
        spmpcfg = __RV_CSR_READ(CSR_SPMPCFG1);
    } else if ((idx >=8) && (idx < 12)) {
        idx -= 8;
        spmpcfg = __RV_CSR_READ(CSR_SPMPCFG2);
    } else {
        idx -= 12;
        spmpcfg = __RV_CSR_READ(CSR_SPMPCFG3);
    }

    idx = idx << 3;
    return (uint8_t)((spmpcfg>>idx) & 0xFF);
#elif __RISCV_XLEN == 64
    if (idx < 8) {
        spmpcfg = __RV_CSR_READ(CSR_SPMPCFG0);
    } else {
        idx -= 8;
        spmpcfg = __RV_CSR_READ(CSR_SPMPCFG2);
    }
    idx = idx << 3;
    return (uint8_t)((spmpcfg>>idx) & 0xFF);
#else
    // TODO Add RV128 Handling
    return 0;
#endif
}

/**
 * \brief   Set 8bit SPMPxCFG by spmp entry index
 * \details Set the given spmpxcfg value to the SPMPxCFG Register.
 * \param [in]    idx      SPMPx region index(0-15)
 * \param [in]    spmpxcfg  SPMPxCFG register value to set
 */
void __set_SPMPxCFG(uint32_t idx, uint8_t spmpxcfg)
{
    rv_csr_t spmpcfgx = 0;
    if (idx >= __SPMP_ENTRY_NUM) return;

#if __RISCV_XLEN == 32
    if (idx < 4) {
        spmpcfgx = __RV_CSR_READ(CSR_SPMPCFG0);
        idx = idx << 3;
        spmpcfgx = (spmpcfgx & ~(0xFFUL << idx)) | ((rv_csr_t)spmpxcfg << idx);
        __RV_CSR_WRITE(CSR_SPMPCFG0, spmpcfgx);
    } else if ((idx >=4) && (idx < 8)) {
        idx -= 4;
        spmpcfgx = __RV_CSR_READ(CSR_SPMPCFG1);
        idx = idx << 3;
        spmpcfgx = (spmpcfgx & ~(0xFFUL << idx)) | ((rv_csr_t)spmpxcfg << idx);
        __RV_CSR_WRITE(CSR_SPMPCFG1, spmpcfgx);
    } else if ((idx >=8) && (idx < 12)) {
        idx -= 8;
        spmpcfgx = __RV_CSR_READ(CSR_SPMPCFG2);
        idx = idx << 3;
        spmpcfgx = (spmpcfgx & ~(0xFFUL << idx)) | ((rv_csr_t)spmpxcfg << idx);
        __RV_CSR_WRITE(CSR_SPMPCFG2, spmpcfgx);
    } else {
        idx -= 12;
        spmpcfgx = __RV_CSR_READ(CSR_SPMPCFG3);
        idx = idx << 3;
        spmpcfgx = (spmpcfgx & ~(0xFFUL << idx)) | ((rv_csr_t)spmpxcfg << idx);
        __RV_CSR_WRITE(CSR_SPMPCFG3, spmpcfgx);
    }
#elif __RISCV_XLEN == 64
    if (idx < 8) {
        spmpcfgx = __RV_CSR_READ(CSR_SPMPCFG0);
        idx = idx << 3;
        spmpcfgx = (spmpcfgx & ~(0xFFULL << idx)) | ((rv_csr_t)spmpxcfg << idx);
        __RV_CSR_WRITE(CSR_SPMPCFG0, spmpcfgx);
    } else {
        idx -= 8;
        spmpcfgx = __RV_CSR_READ(CSR_SPMPCFG2);
        idx = idx << 3;
        spmpcfgx = (spmpcfgx & ~(0xFFULL << idx)) | ((rv_csr_t)spmpxcfg << idx);
        __RV_CSR_WRITE(CSR_SPMPCFG2, spmpcfgx);
    }
#else
    // TODO Add RV128 Handling
#endif
}

/**
 * \brief   Get SPMPCFGx Register by index
 * \details Return the content of the SPMPCFGx Register.
 * \param [in]    idx    SPMPCFG CSR index(0-3)
 * \return               SPMPCFGx Register value
 * \remark
 * - For RV64, only idx = 0 and idx = 2 is allowed.
 *   spmpcfg0 and spmpcfg2 hold the configurations
 *   for the 16 SPMP entries, spmpcfg1 and spmpcfg3 are illegal
 * - For RV32, spmpcfg0–spmpcfg3, hold the configurations
 *   spmp0cfg–spmp15cfg for the 16 SPMP entries
 */
rv_csr_t __get_SPMPCFGx(uint32_t idx)
{
    switch (idx) {
        case 0: return __RV_CSR_READ(CSR_SPMPCFG0);
        case 1: return __RV_CSR_READ(CSR_SPMPCFG1);
        case 2: return __RV_CSR_READ(CSR_SPMPCFG2);
        case 3: return __RV_CSR_READ(CSR_SPMPCFG3);
        default: return 0;
    }
}

/**
 * \brief   Set SPMPCFGx by index
 * \details Write the given value to the SPMPCFGx Register.
 * \param [in]    idx      SPMPCFG CSR index(0-3)
 * \param [in]    spmpcfg   SPMPCFGx Register value to set
 * \remark
 * - For RV64, only idx = 0 and idx = 2 is allowed.
 *   spmpcfg0 and spmpcfg2 hold the configurations
 *   for the 16 SPMP entries, spmpcfg1 and spmpcfg3 are illegal
 * - For RV32, spmpcfg0–spmpcfg3, hold the configurations
 *   spmp0cfg–spmp15cfg for the 16 SPMP entries
 */
void __set_SPMPCFGx(uint32_t idx, rv_csr_t spmpcfg)
{
    switch (idx) {
        case 0: __RV_CSR_WRITE(CSR_SPMPCFG0, spmpcfg); break;
        case 1: __RV_CSR_WRITE(CSR_SPMPCFG1, spmpcfg); break;
        case 2: __RV_CSR_WRITE(CSR_SPMPCFG2, spmpcfg); break;
        case 3: __RV_CSR_WRITE(CSR_SPMPCFG3, spmpcfg); break;
        default: return;
    }
}

/**
 * \brief   Get SPMPADDRx Register by index
 * \details Return the content of the SPMPADDRx Register.
 * \param [in]    idx    SPMP region index(0-15)
 * \return               SPMPADDRx Register value
 */
rv_csr_t __get_SPMPADDRx(uint32_t idx)
{
    switch (idx) {
        case 0: return __RV_CSR_READ(CSR_SPMPADDR0);
        case 1: return __RV_CSR_READ(CSR_SPMPADDR1);
        case 2: return __RV_CSR_READ(CSR_SPMPADDR2);
        case 3: return __RV_CSR_READ(CSR_SPMPADDR3);
        case 4: return __RV_CSR_READ(CSR_SPMPADDR4);
        case 5: return __RV_CSR_READ(CSR_SPMPADDR5);
        case 6: return __RV_CSR_READ(CSR_SPMPADDR6);
        case 7: return __RV_CSR_READ(CSR_SPMPADDR7);
        case 8: return __RV_CSR_READ(CSR_SPMPADDR8);
        case 9: return __RV_CSR_READ(CSR_SPMPADDR9);
        case 10: return __RV_CSR_READ(CSR_SPMPADDR10);
        case 11: return __RV_CSR_READ(CSR_SPMPADDR11);
        case 12: return __RV_CSR_READ(CSR_SPMPADDR12);
        case 13: return __RV_CSR_READ(CSR_SPMPADDR13);
        case 14: return __RV_CSR_READ(CSR_SPMPADDR14);
        case 15: return __RV_CSR_READ(CSR_SPMPADDR15);
        default: return 0;
    }
}

/**
 * \brief   Set SPMPADDRx by index
 * \details Write the given value to the SPMPADDRx Register.
 * \param [in]    idx      SPMP region index(0-15)
 * \param [in]    spmpaddr  SPMPADDRx Register value to set
 */
void __set_SPMPADDRx(uint32_t idx, rv_csr_t spmpaddr)
{
    switch (idx) {
        case 0: __RV_CSR_WRITE(CSR_SPMPADDR0, spmpaddr); break;
        case 1: __RV_CSR_WRITE(CSR_SPMPADDR1, spmpaddr); break;
        case 2: __RV_CSR_WRITE(CSR_SPMPADDR2, spmpaddr); break;
        case 3: __RV_CSR_WRITE(CSR_SPMPADDR3, spmpaddr); break;
        case 4: __RV_CSR_WRITE(CSR_SPMPADDR4, spmpaddr); break;
        case 5: __RV_CSR_WRITE(CSR_SPMPADDR5, spmpaddr); break;
        case 6: __RV_CSR_WRITE(CSR_SPMPADDR6, spmpaddr); break;
        case 7: __RV_CSR_WRITE(CSR_SPMPADDR7, spmpaddr); break;
        case 8: __RV_CSR_WRITE(CSR_SPMPADDR8, spmpaddr); break;
        case 9: __RV_CSR_WRITE(CSR_SPMPADDR9, spmpaddr); break;
        case 10: __RV_CSR_WRITE(CSR_SPMPADDR10, spmpaddr); break;
        case 11: __RV_CSR_WRITE(CSR_SPMPADDR11, spmpaddr); break;
        case 12: __RV_CSR_WRITE(CSR_SPMPADDR12, spmpaddr); break;
        case 13: __RV_CSR_WRITE(CSR_SPMPADDR13, spmpaddr); break;
        case 14: __RV_CSR_WRITE(CSR_SPMPADDR14, spmpaddr); break;
        case 15: __RV_CSR_WRITE(CSR_SPMPADDR15, spmpaddr); break;
        default: return;
    }
}

