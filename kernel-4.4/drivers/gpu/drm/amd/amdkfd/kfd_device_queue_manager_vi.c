/*
 * Copyright 2014 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "kfd_device_queue_manager.h"
#include "gca/gfx_8_0_enum.h"
#include "gca/gfx_8_0_sh_mask.h"
#include "gca/gfx_8_0_enum.h"
#include "oss/oss_3_0_sh_mask.h"

static bool set_cache_memory_policy_vi(struct device_queue_manager *dqm,
				   struct qcm_process_device *qpd,
				   enum cache_policy default_policy,
				   enum cache_policy alternate_policy,
				   void __user *alternate_aperture_base,
				   uint64_t alternate_aperture_size);
static int register_process_vi(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd);
static int initialize_cpsch_vi(struct device_queue_manager *dqm);
static void init_sdma_vm(struct device_queue_manager *dqm, struct queue *q,
				struct qcm_process_device *qpd);

void device_queue_manager_init_vi(struct device_queue_manager_asic_ops *ops)
{
	ops->set_cache_memory_policy = set_cache_memory_policy_vi;
	ops->register_process = register_process_vi;
	ops->initialize = initialize_cpsch_vi;
	ops->init_sdma_vm = init_sdma_vm;
}

static uint32_t compute_sh_mem_bases_64bit(unsigned int top_address_nybble)
{


	BUG_ON((top_address_nybble & 1) || top_address_nybble > 0xE ||
		top_address_nybble == 0);

	return top_address_nybble << 12 |
			(top_address_nybble << 12) <<
			SH_MEM_BASES__SHARED_BASE__SHIFT;
}

static bool set_cache_memory_policy_vi(struct device_queue_manager *dqm,
				   struct qcm_process_device *qpd,
				   enum cache_policy default_policy,
				   enum cache_policy alternate_policy,
				   void __user *alternate_aperture_base,
				   uint64_t alternate_aperture_size)
{
	uint32_t default_mtype;
	uint32_t ape1_mtype;

	default_mtype = (default_policy == cache_policy_coherent) ?
			MTYPE_CC :
			MTYPE_NC;

	ape1_mtype = (alternate_policy == cache_policy_coherent) ?
			MTYPE_CC :
			MTYPE_NC;

	qpd->sh_mem_config = (qpd->sh_mem_config &
			SH_MEM_CONFIG__ADDRESS_MODE_MASK) |
		SH_MEM_ALIGNMENT_MODE_UNALIGNED <<
				SH_MEM_CONFIG__ALIGNMENT_MODE__SHIFT |
		default_mtype << SH_MEM_CONFIG__DEFAULT_MTYPE__SHIFT |
		ape1_mtype << SH_MEM_CONFIG__APE1_MTYPE__SHIFT |
		SH_MEM_CONFIG__PRIVATE_ATC_MASK;

	return true;
}

static int register_process_vi(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd)
{
	struct kfd_process_device *pdd;
	unsigned int temp;

	BUG_ON(!dqm || !qpd);

	pdd = qpd_to_pdd(qpd);

	/* check if sh_mem_config register already configured */
	if (qpd->sh_mem_config == 0) {
		qpd->sh_mem_config =
			SH_MEM_ALIGNMENT_MODE_UNALIGNED <<
				SH_MEM_CONFIG__ALIGNMENT_MODE__SHIFT |
			MTYPE_CC << SH_MEM_CONFIG__DEFAULT_MTYPE__SHIFT |
			MTYPE_CC << SH_MEM_CONFIG__APE1_MTYPE__SHIFT |
			SH_MEM_CONFIG__PRIVATE_ATC_MASK;

		qpd->sh_mem_ape1_limit = 0;
		qpd->sh_mem_ape1_base = 0;
	}

	if (qpd->pqm->process->is_32bit_user_mode) {
		temp = get_sh_mem_bases_32(pdd);
		qpd->sh_mem_bases = temp << SH_MEM_BASES__SHARED_BASE__SHIFT;
		qpd->sh_mem_config |= SH_MEM_ADDRESS_MODE_HSA32 <<
					SH_MEM_CONFIG__ADDRESS_MODE__SHIFT;
	} else {
		temp = get_sh_mem_bases_nybble_64(pdd);
		qpd->sh_mem_bases = compute_sh_mem_bases_64bit(temp);
		qpd->sh_mem_config |= SH_MEM_ADDRESS_MODE_HSA64 <<
			SH_MEM_CONFIG__ADDRESS_MODE__SHIFT;
	}

	pr_debug("kfd: is32bit process: %d sh_mem_bases nybble: 0x%X and register 0x%X\n",
		qpd->pqm->process->is_32bit_user_mode, temp, qpd->sh_mem_bases);

	return 0;
}

static void init_sdma_vm(struct device_queue_manager *dqm, struct queue *q,
				struct qcm_process_device *qpd)
{
	uint32_t value = (1 << SDMA0_RLC0_VIRTUAL_ADDR__ATC__SHIFT);

	if (q->process->is_32bit_user_mode)
		value |= (1 << SDMA0_RLC0_VIRTUAL_ADDR__PTR32__SHIFT) |
				get_sh_mem_bases_32(qpd_to_pdd(qpd));
	else
		value |= ((get_sh_mem_bases_nybble_64(qpd_to_pdd(qpd))) <<
				SDMA0_RLC0_VIRTUAL_ADDR__SHARED_BASE__SHIFT) &
				SDMA0_RLC0_VIRTUAL_ADDR__SHARED_BASE_MASK;

	q->properties.sdma_vm_addr = value;
}

static int initialize_cpsch_vi(struct device_queue_manager *dqm)
{
	return 0;
}
