/* -*- mode: c -*- */
#include <Arduino.h>
#include <Stream.h>
#include "basetypes.h"
#include "dpc.h"
#include "cmdif.h"

/*
 * Simple command interface.
 *
 * Bjoren Davis, March 25, 2022
 */

/* -------------------------------------------------------------------------- */
/* Static functions */

static const cmdifdisp_t *cmdif_lookup(cmdif_t *cmdifp, const char *cmdname);
static void cmdif_callback(dpc_t *dpcp);

/* -------------------------------------------------------------------------- */
/* Global functions */

/*
 * Initialize the command interface state.  'outstreamp' points to the
 *  Stream object that will be used to output text (e.g., prompts,
 *  echoes).  It is usually '&Serial'.
 */
void
cmdif_init(cmdif_t *cmdifp, const cmdifro_t *cmdifrop, Stream *outstreamp)
{
    cmdifp->rop = cmdifrop;
    cmdifp->state = CMDIF_STATE_IDLE;
    cmdifp->len = 0;
    cmdifp->outstreamp = outstreamp;
    LISTLINK_RESET(&(cmdifp->dpc.linkage));
    cmdifp->dpc.funcp = &cmdif_callback;
    cmdif_prompt(cmdifp);
}

/*
 * Accept a character.  This does the echoing and
 *  other line discipline.
 *  inchar >= 0
 */
void
cmdif_takechar(cmdif_t *cmdifp, int inchar)
{
    if (inchar == '\r') {           /* ^M */
        /* We're ready to dispatch the command */
        cmdifp->state = CMDIF_STATE_BUSY;
        cmdifp->outstreamp->write("\r\n");
        dpc_sched(&(cmdifp->dpc));
    }
    else if (inchar == '\b') {      /* ^H */
        if (cmdifp->len > 0) {
            cmdifp->outstreamp->write("\b \b");
            cmdifp->len--;
        }
    }
    else if (inchar == 21) {        /* ^U */
        /* Nuke the entire line */
        while (cmdifp->len > 0) {
            cmdifp->outstreamp->write("\b \b");
            cmdifp->len--;
        }
    }
    else if (cmdifp->len < CMDIF_MAXCMDLINE) {
        /* Accept the character into the string  */
        cmdifp->outstreamp->write((char) inchar);
        cmdifp->line[cmdifp->len++] = (char) inchar;
    }
    else {
        /* Command line is too long.  Beep. */
        cmdifp->outstreamp->write((char) 7);
    }
}


/*
 * Print out a command prompt.
 */
void
cmdif_prompt(cmdif_t *cmdifp)
{
    cmdifp->outstreamp->write(cmdifp->rop->prompt);
    cmdifp->state = CMDIF_STATE_LISTEN;
}


/*
 * Provide help.
 */
void
cmdif_cmd_help(cmdif_t *cmdifp, int argc, char **argv)
{
    int                  idx;
    Stream              *outstreamp;
    const cmdifro_t     *rop;
    const cmdifdisp_t   *cmdentryp;

    rop = cmdifp->rop;
    outstreamp = cmdifp->outstreamp;

    if (argc <= 1) {
        /* Just give a list of all commands and their one-liners */
        outstreamp->print(rop->tophelp);
        outstreamp->println();

        outstreamp->println(F("\r\n\nAvailable commands:\r\n"));

        idx = rop->disptablelen;
        cmdentryp = rop->disptable;
        while (idx != 0) {
            outstreamp->print(cmdentryp->cmdname);
            outstreamp->print("\t");
            outstreamp->print(cmdentryp->briefhelp);
            outstreamp->println();
            cmdentryp++;
            idx--;
        }
    }
    else {
        cmdentryp = cmdif_lookup(cmdifp, argv[1]);
        if (cmdentryp == NULL) {
            outstreamp->print(F("Unknown command: "));
            outstreamp->println(argv[1]);
        }
        else if (cmdentryp->fullhelp != NULL) {
            outstreamp->println(cmdentryp->fullhelp);
        }
        else {
            outstreamp->print(F("No further help available for command "));
            outstreamp->println(argv[1]);
        }
    }
}

void
cmdif_refresh(cmdif_t *cmdifp)
{
    int      idx;
    Stream              *outstreamp;
    const cmdifro_t     *rop;

    rop = cmdifp->rop;
    outstreamp = cmdifp->outstreamp;

    if (cmdifp->state == CMDIF_STATE_LISTEN) {
        outstreamp->print("\r\n");
        outstreamp->print(rop->prompt);
        for (idx = 0; idx < cmdifp->len; idx++) {
            outstreamp->print(cmdifp->line[idx]);
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Static functions */


/*
 * Given a command name, find the command dispatch table
 *  entry, if it exists.  Returns NULL if it does not.
 */

static const cmdifdisp_t *
cmdif_lookup(cmdif_t *cmdifp, const char *cmdname)
{
    int                  idx;
    const cmdifdisp_t   *cmdentryp;
    const cmdifro_t     *rop;

    rop = cmdifp->rop;
    idx = rop->disptablelen;
    cmdentryp = rop->disptable;
    while (idx != 0) {
        if (strcmp(cmdname, cmdentryp->cmdname) == 0) {
            return cmdentryp;
        }
        cmdentryp++;
        idx--;
    }
    return NULL;
}

/*
 * The callback function to dispatch an actual
 *  command.
 */
static void
cmdif_callback(dpc_t *dpcp)
{
    cmdif_t     *cmdifp;
    char        *argv[CMDIF_MAXNARGS];
    int          argc, idx;
    void       (*funcp)(cmdif_t *cmdifp, int argc, char **argv);
    int          instr;     /* actually a boolean */
    const cmdifdisp_t *cmdentryp;
    Stream              *outstreamp;

    cmdifp = FROMWITHIN(dpcp, cmdif_t, dpc);
    outstreamp = cmdifp->outstreamp;

    /*
     * We've accumulated a line.  Parse it and dispatch it.
     */
    /*
     * First, create our argv.
     */
    argc = 0;
    instr = 0;
    idx = 0;

    while (idx < (int) cmdifp->len) {
        if (isWhitespace(cmdifp->line[idx])) {
            if (instr != 0) {
                /* We were in an argument. */
                cmdifp->line[idx] = '\0';
            }
            instr = 0;
        }
        else {
            if (instr == 0 && argc < CMDIF_MAXNARGS) {
                /* We were not in an argument */
                argv[argc++] = &(cmdifp->line[idx]);
            }
            instr = 1;
        }
        idx++;
    }
    /* NUL out the final character */
    cmdifp->line[idx] = '\0';

    /*
     * Did we not actually have a command?
     */
    if (argc > 0) {
        /*
         * Now try to find the matching command handler.
         */
        cmdentryp = cmdif_lookup(cmdifp, argv[0]);

        if (cmdentryp == NULL) {
            outstreamp->print(F("Unknown command: "));
            outstreamp->println(argv[0]);
        }
        else {
            funcp = cmdentryp->funcp;
            (*funcp)(cmdifp, argc, argv);
        }
    }
    cmdifp->len = 0;

    /* Prompt for a new command */
    cmdif_prompt(cmdifp);
}

