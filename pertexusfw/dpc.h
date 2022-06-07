/* -*- mode: c -*- */

/*
 * Deferred procedure calls.
 *
 * Bjoren Davis, March 26, 2022
 */

#if !defined(__DPC_H)
#define __DPC_H

#if defined(__cplusplus)
extern "C" {
#endif      /* defined(__cplusplus) */
    
/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

/* forward reference: */
struct dpc;

typedef struct dpc {
    listlink_t        linkage;
    void            (*funcp)(struct dpc *dpcp);
} dpc_t;

/* -------------------------------------------------------------------------- */
/* Global protototypes */

/* dpc.c: */
extern void dpc_sched(dpc_t *dpcp);
extern bool_t dpc_dispatch(void);

#if defined(__cplusplus)
}
#endif      /* defined(__cplusplus) */

#endif      /* !defined(__DPC_H) */
