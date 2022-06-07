/* -*- mode: c -*- */

/*
 * Base types for my firmware.
 *
 * Bjoren Davis, March 26, 2022
 */

#if !defined(__BASETYPES_H)
#define __BASETYPES_H

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

/* forward reference: */
struct listlink;

/*
 * A list or an item on a list.  An empty list is represented
 *  as one of these where both prevp and nextp point back to the
 *  list itself.
 */
typedef struct listlink {
    struct listlink     *prevp; /* for list: pointer to last element */
    struct listlink     *nextp; /* for list: pointer to first element */
} listlink_t;

#define LISTLINK_EMPTYINIT(llp)     { (llp), (llp) }
#define LISTLINK_RESET(llp)         ((llp)->prevp = (llp)->nextp = (llp))

#define NELEM(x)        (sizeof((x)) / sizeof((x)[0]))
#define OFFSETOF(t, m)  ((unsigned int) &(((t *) 0)->m))
#define FROMWITHIN(op, t, m) ((t *) ((unsigned char *) (op)             \
                                     - ((unsigned int) &(((t *) 0)->m))))

#define UNUSED(x)       ((void) (x))

#define MIN3(x, y, z)   ((x) < (y) ?                                    \
                                 ((x) < (z) ? (x)                       \
                                            : (z))                      \
                               : ((y) < (z) ? (y)                       \
                                            : (z)))
#define MIN(x, y)       ((x) < (y) ? (x) : (y))

#if defined(TRUE)
#   undef TRUE
#endif
#if defined(FALSE)
#   undef FALSE
#endif
#define TRUE        (0 == 0)
#define FALSE       (!TRUE)

#define BITSPERBYTE     8

typedef int8_t bool_t;

/* -------------------------------------------------------------------------- */
/* Inline functions */

/*
 * Pull the first item off the front of a list.  Returns NULL if
 *  there are no items on the list.
 */
static inline listlink_t *
listlink_firstget(listlink_t *listp)
{
    listlink_t      *itemp, *newfirstp;

    itemp = listp->nextp;
    if (itemp == listp) {
        /* The list is empty */
        return NULL;
    }

    /* Remove this element from the list */
    newfirstp = itemp->nextp;
    newfirstp->prevp = listp;
    listp->nextp = newfirstp;


    /* To be safe, isolate this element */
    itemp->nextp = itemp->prevp = itemp;

    return itemp;
}

/*
 * Add an item to the end of a list.
 */
static inline void
listlink_lastput(listlink_t *listp, listlink_t *itemp)
{
    listlink_t      *oldlastp;

    oldlastp = listp->prevp;    /* the current last element in the list */

    itemp->nextp = listp;       /* make this the last element on the list */
    itemp->prevp = oldlastp;

    oldlastp->nextp = itemp;
    listp->prevp = itemp;
}



/* -------------------------------------------------------------------------- */
/* Global protototypes */


#endif      /* !defined(__BASETYPES_H) */
