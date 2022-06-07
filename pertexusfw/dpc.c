/* -*- mode: c -*- */
/*
 * Deferred procedure calls.
 *
 * Bjoren Davis, March 26, 2022
 */

#include <Arduino.h>
#include "basetypes.h"
#include "dpc.h"

/* -------------------------------------------------------------------------- */
/* Static read/write data */

static listlink_t dpc_list = LISTLINK_EMPTYINIT(&dpc_list);


/* -------------------------------------------------------------------------- */
/* Global functions */

/*
 * Schedule the given DPC function.  Interrupt handlers should
 *  call dpc_sched_fromint(), below.
 */
void
dpc_sched(dpc_t *dpcp)
{
    noInterrupts();     /* implicitly does a memory barrier as well */
    listlink_lastput(&dpc_list, &(dpcp->linkage));
    interrupts();
}

/*
 * Schedule the given DPC function.  This is meant to be called
 *  from the interrupt context with interrupts *off*.
 */
void
dpc_sched_fromint(dpc_t *dpcp)
{
    listlink_lastput(&dpc_list, &(dpcp->linkage));
}
    

/*
 * Dispatch a single DPC from the beginning of the list.
 *
 * Note that this function briefly disables interrupts
 *  so that it may modify the DPC list without risk of
 *  corruption.
 *
 * Returns TRUE if it ran a DPC, FALSE otherwise.
 */
bool_t
dpc_dispatch(void)
{
    dpc_t       *dpcp;
    listlink_t  *itemp;

    noInterrupts();     /* implicitly does a memory barrier as well */
    itemp = listlink_firstget(&dpc_list);
    interrupts();

    if (itemp == NULL) {
        return FALSE;
    }

    dpcp = FROMWITHIN(itemp, dpc_t, linkage);
    (*dpcp->funcp)(dpcp);

    return TRUE;
}

/* -------------------------------------------------------------------------- */
/* Static functions */

