/*
 * Copyright (C) 2016 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _MT6759_VPU_REG_H_
#define _MT6759_VPU_REG_H_

#include <sync_write.h>

/* common macro definitions */
#define F_REG(base, offset)     (*((volatile unsigned int *) (base + offset)))
#define F_VAL(val, msb, lsb)    ((msb - lsb > 30) ? val : (((val)&((1<<(msb-lsb+1))-1))<<lsb))
#define F_MSK(msb, lsb)         F_VAL(0xffffffff, msb, lsb)
#define F_BIT_SET(bit)          (1<<(bit))
#define F_BIT_VAL(val, bit)     ((!!(val))<<(bit))

#define VPU_SET_BIT(reg, bit)   ((*(volatile unsigned int *)(reg)) |= (unsigned int)(1 << (bit)))
#define VPU_CLR_BIT(reg, bit)   ((*(volatile unsigned int *)(reg)) &= ~((unsigned int)(1 << (bit))))

static inline unsigned int vpu_read_reg32(unsigned long vpu_base, unsigned int offset)
{
	return ioread32((void *) (vpu_base + offset));
}

static inline void vpu_write_reg32(unsigned long vpu_base, unsigned int offset, unsigned int val)
{
	mt_reg_sync_writel(val, (void *) (vpu_base + offset));
}


/* Spare Register - Enum */
enum {
	VPU_CMD_DO_EXIT         = 0x00,
	VPU_CMD_DO_LOADER       = 0x01,
	VPU_CMD_DO_DECRYPT      = 0x02,
	VPU_CMD_DO_PASS1_DL     = 0x10,
	VPU_CMD_DO_PASS2_DL     = 0x11,
	VPU_CMD_DO_D2D          = 0x22,
	VPU_CMD_SET_DEBUG       = 0x40,
	VPU_CMD_GET_SWVER       = 0x81,
	VPU_CMD_GET_ALGO        = 0x82,

	/* Extend for test */
	VPU_CMD_EXT_BUSY        = 0xF0
};

enum {
	VPU_STATE_NOT_READY     = 0x00,
	VPU_STATE_READY         = 0x01,
	VPU_STATE_IDLE          = 0x02,
	VPU_STATE_BUSY          = 0x04,
	VPU_STATE_ERROR         = 0x08,
	VPU_STATE_TERMINATED    = 0x10
};


enum vpu_reg {
	REG_RESET,
	REG_EVENT_TRIG,
	REG_INT_EN,
	REG_DONE_ST,
	REG_CTRL,
	REG_XTENSA_INT,
	REG_CTL_XTENSA_INT,
	REG_INT_MASK,
	REG_TOP_SPARE,
	REG_AXI_DEFAULT0,
	REG_AXI_DEFAULT1,
	REG_AXI_DEFAULT2,
	REG_AXI_DEFAULT3,
	REG_CABGEN_CTRL,
	REG_XTENSA_INFO00,
	REG_XTENSA_INFO01,
	REG_XTENSA_INFO02,
	REG_XTENSA_INFO03,
	REG_XTENSA_INFO04,
	REG_XTENSA_INFO05,
	REG_XTENSA_INFO06,
	REG_XTENSA_INFO07,
	REG_XTENSA_INFO08,
	REG_XTENSA_INFO09,
	REG_XTENSA_INFO10,
	REG_XTENSA_INFO11,
	REG_XTENSA_INFO12,
	REG_XTENSA_INFO13,
	REG_XTENSA_INFO14,
	REG_XTENSA_INFO15,
	REG_XTENSA_INFO16,
	REG_XTENSA_INFO17,
	REG_XTENSA_INFO18,
	REG_XTENSA_INFO19,
	REG_XTENSA_INFO20,
	REG_XTENSA_INFO21,
	REG_XTENSA_INFO22,
	REG_XTENSA_INFO23,
	REG_XTENSA_INFO24,
	REG_XTENSA_INFO25,
	REG_XTENSA_INFO26,
	REG_XTENSA_INFO27,
	REG_XTENSA_INFO28,
	REG_XTENSA_INFO29,
	REG_XTENSA_INFO30,
	REG_XTENSA_INFO31,
	REG_DEBUG_INFO00,
	REG_DEBUG_INFO01,
	REG_DEBUG_INFO02,
	REG_DEBUG_INFO03,
	REG_DEBUG_INFO04,
	REG_DEBUG_INFO05,
	REG_DEBUG_INFO06,
	REG_DEBUG_INFO07,
	REG_DEBUG_INFO08,
	VPU_NUM_REGS
};

enum vpu_reg_field {
	FLD_SHARE_SRAM_CONFIG,
	FLD_DCACHE_SRAM_EN,
	FLD_APB_PADDR_REG,
	FLD_IPU_DM_RST,
	FLD_APB_MODE,
	FLD_IPU_OCD_HALT_ON_RST,
	FLD_IPU_D_RST,
	FLD_IPU_B_RST,
	FLD_IPU_HW_RST,
	FLD_GCE_EVENT3,
	FLD_GCE_EVENT2,
	FLD_GCE_EVENT1,
	FLD_GCE_EVENT0,
	FLD_IPU_INT_EN_RESERVED,
	FLD_P_WAIT_MODE,
	FLD_BREAK_IN_ACK,
	FLD_BREAK_OUT,
	FLD_XOCD_MODE,
	FLD_P_DEBUG_ENABLE,
	FLD_STROBE,
	FLD_PARB_CLK_EN,
	FLD_PBCLK_EN,
	FLD_RUN_STALL,
	FLD_TRIG_IN_IDMA,
	FLD_BREAK_OUT_ACK,
	FLD_BREAK_IN,
	FLD_STAT_VECTOR_SEL,
	FLD_TIE2APB_CLK_EN,
	FLD_BUS_PIF_GATED,
	FLD_PRID,
	FLD_NMI,
	FLD_INT_RSV_XTENSA,
	FLD_INT_XTENSA,
	FLD_INT_CTL_XTENSA15,
	FLD_INT_CTL_XTENSA14,
	FLD_INT_CTL_XTENSA13,
	FLD_INT_CTL_XTENSA12,
	FLD_INT_CTL_XTENSA11,
	FLD_INT_CTL_XTENSA10,
	FLD_INT_CTL_XTENSA09,
	FLD_INT_CTL_XTENSA08,
	FLD_INT_CTL_XTENSA07,
	FLD_INT_CTL_XTENSA06,
	FLD_INT_CTL_XTENSA05,
	FLD_INT_CTL_XTENSA04,
	FLD_INT_CTL_XTENSA03,
	FLD_INT_CTL_XTENSA02,
	FLD_INT_CTL_XTENSA01,
	FLD_INT_CTL_XTENSA00,
	FLD_APMCU_INT_MASK,
	FLD_CTL_XTENSA_INT_MASK,
	FLD_IPU_SPARE,
	FLD_AR_USER_CORE_8_4,
	FLD_AW_USER_CORE_8_4,
	FLD_AR_USER_IDMA_8_4,
	FLD_AW_USER_IDMA_8_4,
	FLD_AR_DOMAIN_4_3,
	FLD_AW_DOMAIN_4_3,
	FLD_CACHE_S_ADDR_HPART,
	FLD_MM1_S_ADDR_HPART,
	FLD_AR_DOMAIN_APB,
	FLD_AR_FLUSH_APB,
	FLD_AW_DOMAIN_APB,
	FLD_AW_FLUSH_APB,
	FLD_AR_FLUSH_CORE,
	FLD_AW_FLUSH_CORE,
	FLD_AR_FLUSH_IDMA,
	FLD_AW_FLUSH_IDMA,
	FLD_AR_DOMAIN_CAM,
	FLD_AR_FLUSH_CAM,
	FLD_AW_DOMAIN_CAM,
	FLD_AW_FLUSH_CAM,
	FLD_AR_DOMAIN_IMG,
	FLD_AR_FLUSH_IMG,
	FLD_AW_DOMAIN_IMG,
	FLD_AW_FLUSH_IMG,
	FLD_AR_DOMAIN_CORE,
	FLD_AW_DOMAIN_CORE,
	FLD_SPNIDEN,
	FLD_SPIDEN,
	FLD_NIDEN,
	FLD_DBG_EN,
	FLD_AR_DOMAIN_IDMA,
	FLD_AW_DOMAIN_IDMA,
	FLD_AR_PROT_1_CORE,
	FLD_AW_PROT_1_CORE,
	FLD_AR_PROT_1_IDMA,
	FLD_AW_PROT_1_IDMA,
	FLD_M_1_OUTSTAND_EXTEND_ON,
	FLD_M_1_QOS_ON,
	FLD_M_0_OUTSTAND_EXTEND_ON,
	FLD_M_0_QOS_ON,
	FLD_SI_1_WAY_INTERLEAVE_EN,
	FLD_SI_1_OUTSTAND_DISABLE,
	FLD_SI_1_CTRL_BYPASS,
	FLD_SI_1_WAY_EN,
	FLD_SI_0_WAY_INTERLEAVE_EN,
	FLD_SI_0_OUTSTAND_DISABLE,
	FLD_SI_0_CTRL_BYPASS,
	FLD_SI_0_WAY_EN,
	FLD_XTENSA_INFO0,
	FLD_XTENSA_INFO1,
	FLD_XTENSA_INFO2,
	FLD_XTENSA_INFO3,
	FLD_XTENSA_INFO4,
	FLD_XTENSA_INFO5,
	FLD_XTENSA_INFO6,
	FLD_XTENSA_INFO7,
	FLD_XTENSA_INFO8,
	FLD_XTENSA_INFO9,
	FLD_XTENSA_INFO10,
	FLD_XTENSA_INFO11,
	FLD_XTENSA_INFO12,
	FLD_XTENSA_INFO13,
	FLD_XTENSA_INFO14,
	FLD_XTENSA_INFO15,
	FLD_XTENSA_INFO16,
	FLD_XTENSA_INFO17,
	FLD_XTENSA_INFO18,
	FLD_XTENSA_INFO19,
	FLD_XTENSA_INFO20,
	FLD_XTENSA_INFO21,
	FLD_XTENSA_INFO22,
	FLD_XTENSA_INFO23,
	FLD_XTENSA_INFO24,
	FLD_XTENSA_INFO25,
	FLD_XTENSA_INFO26,
	FLD_XTENSA_INFO27,
	FLD_XTENSA_INFO28,
	FLD_XTENSA_INFO29,
	FLD_XTENSA_INFO30,
	FLD_XTENSA_INFO31,
	FLD_IPU_DBG_INFO00,
	FLD_IPU_DBG_INFO01_RESERVED,
	FLD_IPU_DBG_INFO01,
	FLD_IPU_DBG_INFO02,
	FLD_IPU_DBG_INFO03,
	FLD_IPU_DBG_INFO04,
	FLD_IPU_DBG_INFO05,
	FLD_IPU_DBG_INFO06_RESERVED,
	FLD_IPU_DBG_INFO06,
	FLD_IPU_DBG_INFO07,
	FLD_IPU_DBG_INFO08,
	VPU_NUM_REG_FIELDS
};


struct vpu_reg_desc {
	enum vpu_reg reg;
	char *name;
	uint32_t offset;
	uint8_t size;
};


struct vpu_reg_field_desc {
	enum vpu_reg reg;
	enum vpu_reg_field field;
	char *name;
	uint8_t msb;
	uint8_t lsb;
};

extern struct vpu_reg_desc g_vpu_reg_descs[VPU_NUM_REGS];
extern struct vpu_reg_field_desc g_vpu_reg_field_descs[VPU_NUM_REG_FIELDS];

uint32_t vpu_read_field(enum vpu_reg_field f);

void vpu_write_field(enum vpu_reg_field f, uint32_t v);

#endif
