/* -*- mode: c -*- */

/*
 * Simple command interface.
 *
 * Bjoren Davis, March 25, 2022
 */

#if !defined(__CMDIF_H)
#define __CMDIF_H

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */

#define CMDIF_MAXCMDLINE 80 /* maximum number of characters in a command line */
#define CMDIF_MAXNARGS 8          /* maximum number of arguments */

#define CMDIF_STATE_IDLE    0       /* done nothing...need to prompt */
#define CMDIF_STATE_LISTEN  1       /* listening for characters */
#define CMDIF_STATE_BUSY    2       /* command is being dispatched */

/* Forward reference: */
struct _cmdif;

/*
 * A command dispatch table entry.
 */
typedef struct _cmdifdisp {
    const char      *cmdname;
    const char      *briefhelp;     /* one line description */
    const char      *fullhelp;      /* full help, given with 'help cmdname' */
    void           (*funcp)(struct _cmdif *cmdifp, int argc, char **argv);
} cmdifdisp_t;

/*
 * Read only information pertaining to the command interface.
 */
typedef struct _cmdifro {
    const char          *prompt;
    const char          *tophelp;           /* string with top-level help */
    const cmdifdisp_t   *disptable;
    int                  disptablelen;
} cmdifro_t;

/*
 * Read/write state pertaining to the command interface.
 */
typedef struct _cmdif {
    const cmdifro_t *rop;                   /* pointer to read-only info */
    Stream          *outstreamp;            /* pointer to output stream */
    dpc_t            dpc;
    char             line[CMDIF_MAXCMDLINE+1];  /* Add space for final NUL */
    unsigned char    len;
    unsigned char    state;
} cmdif_t;

/* -------------------------------------------------------------------------- */
/* Global protototypes */

/* cmdif.c: */
extern void cmdif_init(cmdif_t *cmdifp, const cmdifro_t *cmdifrop,
                       Stream *outstreamp);
extern void cmdif_takechar(cmdif_t *cmdifp, int inchar);
extern void cmdif_prompt(cmdif_t *cmdifp);
extern void cmdif_cmd_help(cmdif_t *cmdifp, int argc, char **argv);
extern void cmdif_refresh(cmdif_t *cmdifp);

#endif      /* !defined(__CMDIF_H) */
