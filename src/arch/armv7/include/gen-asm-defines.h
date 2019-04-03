/* SPDX-License-Identifier: BSD-2-Clause */
/* Copyright (c) 2018, Linaro Limited */

#ifndef GEN_ASM_DEFINES_H
#define GEN_ASM_DEFINES_H

#define DEFINES void __defines(void); void __defines(void)

#define DEFINE(def, val) \
	asm volatile("\n==>" #def " %0 " #val : : "i" (val))

DEFINE(SM_NSEC_CTX_R0, offsetof(struct sm_nsec_ctx, r0));
DEFINE(SM_NSEC_CTX_R8, offsetof(struct sm_nsec_ctx, r8));
DEFINE(SM_SEC_CTX_R0, offsetof(struct sm_sec_ctx, r0));
DEFINE(SM_SEC_CTX_MON_LR, offsetof(struct sm_sec_ctx, mon_lr));
DEFINE(SM_CTX_SEC_SIZE, sizeof(struct sm_sec_ctx));
DEFINE(SM_CTX_SIZE, sizeof(struct sm_ctx));
DEFINE(SM_CTX_NSEC, offsetof(struct sm_ctx, nsec));
DEFINE(SM_CTX_SEC, offsetof(struct sm_ctx, sec));

DEFINE(THREAD_VECTOR_TABLE_FIQ_ENTRY,
       offsetof(struct thread_vector_table, fiq_entry));

DEFINE(THREAD_SVC_REG_R0, offsetof(struct thread_svc_regs, r0));
DEFINE(THREAD_SVC_REG_R5, offsetof(struct thread_svc_regs, r5));
DEFINE(THREAD_SVC_REG_R6, offsetof(struct thread_svc_regs, r6));

/* struct thread_core_local */
DEFINE(THREAD_CORE_LOCAL_R0, offsetof(struct thread_core_local, r[0]));
DEFINE(THREAD_CORE_LOCAL_SM_PM_CTX_PHYS,
       offsetof(struct thread_core_local, sm_pm_ctx_phys));
DEFINE(THREAD_CORE_LOCAL_SIZE, sizeof(struct thread_core_local));

DEFINE(SM_PM_CTX_SIZE, sizeof(struct sm_pm_ctx));

/* struct thread_core_local */
DEFINE(THREAD_CORE_LOCAL_TMP_STACK_VA_END,
	offsetof(struct thread_core_local, tmp_stack_va_end));
DEFINE(THREAD_CORE_LOCAL_CURR_THREAD,
	offsetof(struct thread_core_local, curr_thread));
DEFINE(THREAD_CORE_LOCAL_FLAGS,
	offsetof(struct thread_core_local, flags));
DEFINE(THREAD_CORE_LOCAL_ABT_STACK_VA_END,
	offsetof(struct thread_core_local, abt_stack_va_end));

#endif /*GEN_ASM_DEFINES_H*/
