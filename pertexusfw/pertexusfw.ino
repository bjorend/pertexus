/* -*- mode: c -*- */

/*
 * pertexus -- A Pertex tape interface excerciser and nexus (via
 *  USB and/or Ethernet iSCSI).
 *
 * This software allows a Teensy 4.1 microcontroller
 *  (https://www.pjrc.com/store/teensy41.html) to speak to a
 *  magnetic tape drive via the Pertec interface.  It is designed
 *  around the pertexus rev A board (bdb004, revision A1, taped out
 *  May 22, 2022).
 *
 * References:
 *  [1] Technical Service Manual for Qualstar Phase Encoded
 *      Streaming Tape Transports
 *      P/N 500150 Rev. B 5/14/90
 *      Qualstar Corporation, 9621 Irondale Avenue, Chatsworth, CA  91311
 *      https://bitsavers.org/pdf/qualstar/500150B_1052serviceMan.pdf
 *  [2] Pertec 9-Track Tape Interface
 *      Information courtesty [sic] of John Wilson
 *      https://sydex.com/pertec.html
 *  [3] i.MX RT1060 Processor Reference Manual
 *      Document Number: IMXRT1060RM
 *      Rev. 3, 07/2021
 *      NXP Semiconductors
 *      https://www.pjrc.com/teensy/IMXRT1060RM_rev3.pdf
 *  [4] SIMH Magtape Representation and Handling
 *      Bob Supnik, 30 Aug 06
 *      http://simh.trailing-edge.com/docs/simh_magtape.pdf
 *  [5] Schematic for Teensy 4.1
 *      https://www.pjrc.com/teensy/schematic41.png
 *  [6] Pinouts for Teensy 4.1
 *      Front: https://www.pjrc.com/teensy/card11a_rev4_web.pdf
 *      Back: https://www.pjrc.com/teensy/card11b_rev4_web.pdf
 *  [7] PCA9535; PCA9535C
 *      16-bit I2C-bus and SMBus, low power I/O port with interrupt
 *      Product data sheet
 *      Rev. 6 - 7 November 2017
 *      Document identifier: PCA9535_PCA9535C
 *      NXP Semiconductors
 *      https://www.nxp.com/docs/en/data-sheet/PCA9535_PCA9535C.pdf
 *
 * Bjoren Davis, June 4, 2022
 */

#define OPT_INTCOUNTS

#include <Wire.h> //include Wire.h library
#include <SD.h>
#include <TimeLib.h>
#if defined(OPT_ETHERNET)
#   include <NativeEthernet.h>
#endif
#include "basetypes.h"
#include "dpc.h"
#include "cmdif.h"

/* -------------------------------------------------------------------------- */
/* General macros */

/* Compile-time assertion: */
#define _ASSERT_CT3(x, ln)                                              \
    typedef int _assert_ct_failed_on_line_ ## ln [(x) ? 1 : -1] __attribute__ ((unused))
#define _ASSERT_CT2(x, ln)  _ASSERT_CT3(x, ln)
#define ASSERT_CT(x)        _ASSERT_CT2(x, __LINE__)

/* GPIOs directly connected to the CPU (via level shifters) */
/* Tuple is (a, b, c, d, e, f, g, h):
 *  a = Signal group:
 *      ------------------
 *      Group A signals.  These are controller inputs/peripheral outputs.
 *      0 = Group A0.  Group A signal implemented on a direct,
 *          level-shifted I/O.
 *      1 = Group A1.  Group A signal implemented on a port expander.
 *      ------------------
 *      Group B signals.  These are peripheral inputs/controller outputs.
 *      2 = Group B0.  Group B signal implemented on a direct,
 *          level-shifted I/O.
 *      3 = Group B1.  Group B signal implemented on a port expander.
 *      ------------------
 *      Group C signals.  These signals have implementation-specific meanings
 *          and directions.
 *      4 = Group C0.  Group C signal implemented on a direct,
 *          level-shifted I/O. (not currently implemented)
 *      5 = Group C1.  Group C signal implemented on a port expander.
 *      ------------------
 *      Group D signals.  Direction and output control, I2C.  These
 *      are all direct I/Os.
 *      6 = Group D
 *
 *  b = for direct, level-shifted: GPIO set number (1..9)
 *      for port expander: port expander word number (0 or 1)
 *  c = for direct, level-shifted: GPIO number within set (0..31)
 *      for port expander: bit within word (0..7)
 *  d = Arduino pin number.  -1 for port expander signals
 *  e = Teensy 4.1 external connector name (string)
 *  f = Teensy 4.1 external connector pin number
 *  g = Pertec interface connector (1 or 2; 0 if not brought out)
 *  h = Pertec interface pin number (0..49)
 */
#define GROUPA0         0
#define GROUPA1         1
#define GROUPB0         2
#define GROUPB1         3
#define GROUPC0         4
#define GROUPC1         5
#define GROUPD          6

#define GP_IRP        (GROUPA0, 6,  2,  1, "J5",  3, 2,  1)  /* AD_B0_02 */
#define GP_IFMK       (GROUPA0, 6,  3,  0, "J5",  2, 2, 14)  /* AD_B0_03 */
#define GP_LPI2C4_SCL (GROUPD,  6, 12, 24, "J5", 16, 0,  0)  /* AD_B0_12 */
#define GP_LPI2C4_SDA (GROUPD,  6, 13, 25, "J5", 17, 0,  0)  /* AD_B0_13 */
#define GP_IR7        (GROUPA0, 6, 16, 19, "J4",  8, 2,  8)  /* AD_B1_00 */
#define GP_IR6        (GROUPA0, 6, 17, 18, "J4",  9, 2, 10)  /* AD_B1_01 */
#define GP_IR5        (GROUPA0, 6, 18, 14, "J4", 13, 2, 20)  /* AD_B1_02 */
#define GP_IR4        (GROUPA0, 6, 19, 15, "J4", 12, 2,  6)  /* AD_B1_03 */
#define GP_IR3        (GROUPA0, 6, 20, 40, "J4", 17, 1, 50)  /* AD_B1_04 */
#define GP_IR2        (GROUPA0, 6, 21, 41, "J4", 16, 1, 48)  /* AD_B1_05 */
#define GP_IR1        (GROUPA0, 6, 22, 17, "J4", 10, 2,  3)  /* AD_B1_06 */
#define GP_IR0        (GROUPA0, 6, 23, 16, "J4", 11, 2,  2)  /* AD_B1_07 */
#define GP_IW7        (GROUPB0, 6, 24, 22, "J4",  5, 1, 24)  /* AD_B1_08 */
#define GP_IW6        (GROUPB0, 6, 25, 23, "J4",  4, 1, 28)  /* AD_B1_09 */
#define GP_IW5        (GROUPB0, 6, 26, 20, "J4",  7, 1, 32)  /* AD_B1_10 */
#define GP_IW4        (GROUPB0, 6, 27, 21, "J4",  6, 1,  6)  /* AD_B1_11 */
#define GP_IW3        (GROUPB0, 6, 28, 38, "J4", 19, 1, 26)  /* AD_B1_12 */
#define GP_IW2        (GROUPB0, 6, 29, 39, "J4", 18, 1, 30)  /* AD_B1_13 */
#define GP_IW1        (GROUPB0, 6, 30, 26, "J5", 18, 1, 12)  /* AD_B1_14 */
#define GP_IW0        (GROUPB0, 6, 31, 27, "J5", 19, 1, 10)  /* AD_B1_15 */
#define GP_IWRT       (GROUPB0, 7,  0, 10, "J5", 12, 1, 34)  /* B0_00 */
#define GP_IERASE     (GROUPB0, 7,  1, 12, "J5", 14, 1, 40)  /* B0_01 */
#define GP_IEDIT      (GROUPB0, 7,  2, 11, "J5", 13, 1, 38)  /* B0_02 */
#define GP_ILWD       (GROUPB0, 7,  3, 13, "J4", 14, 1,  4)  /* B0_03 */
#define GP_IGO        (GROUPB0, 7, 10,  6, "J5",  8, 1,  8)  /* B0_10 */
#define GP_IREV       (GROUPB0, 7, 11,  9, "J5", 11, 1, 18)  /* B0_11 */
#define GP_IWFM       (GROUPB0, 7, 12, 32, "J5", 24, 1, 42)  /* B0_12 */
#define GP_IREW       (GROUPB0, 7, 16,  8, "J5", 10, 1, 20)  /* B1_00 */
#define GP_IWP        (GROUPB0, 7, 17,  7, "J5",  9, 1, 22)  /* B1_01 */
#define GP_IRSTR      (GROUPA0, 7, 18, 36, "J4", 21, 2, 34)  /* B1_02 */
#define GP_IWSTR      (GROUPA0, 7, 19, 37, "J4", 20, 2, 36)  /* B1_03 */
#define GP_IHER       (GROUPA0, 7, 28, 35, "J4", 22, 2, 12)  /* B1_12 */
#define GP_ICER       (GROUPA0, 7, 29, 34, "J4", 23, 2, 42)  /* B1_13 */
#define GP_DIRB       (GROUPD,  8, 18, 28, "J5", 20, 0,  0)  /* EMC_32 */
#define GP_IEOT       (GROUPA0, 8, 22, 31, "J5", 23, 2, 22)  /* EMC_36 */
#define GP_IOFL       (GROUPB0, 8, 23, 30, "J5", 22, 2, 24)  /* EMC_37 */
#define GP_IRDY       (GROUPA0, 9,  4,  2, "J5",  4, 2, 28)  /* EMC_04 */
#define GP_DIRA       (GROUPD,  9,  5,  3, "J5",  5, 0,  0)  /* EMC_05 */
#define GP_IDBY       (GROUPA0, 9,  6,  4, "J5",  6, 2, 38)  /* EMC_06 */
#define GP_IFBY       (GROUPA0, 9,  7, 33, "J4", 24, 1,  2)  /* EMC_07 */
#define GP_LPI2C4_INT (GROUPD,  9,  8,  5, "J5",  7, 0,  0)  /* EMC_08 */
#define GP_OEA        (GROUPD,  9, 31, 29, "J5", 21, 0,  0)  /* EMC_31 */
#define GP_IDENT      (GROUPA1, 0,  0, -1, NULL,  0, 2, 16)  /* IOExpander:0 */
#define GP_IRWD       (GROUPA1, 0,  1, -1, NULL,  0, 2, 30)  /* IOExpander:1 */
#define GP_ISPEED     (GROUPA1, 0,  2, -1, NULL,  0, 2, 40)  /* IOExpander:2 */
#define GP_INRZ       (GROUPA1, 0,  3, -1, NULL,  0, 2, 26)  /* IOExpander:3 */
#define GP_ITAD0      (GROUPB1, 0,  4, -1, NULL,  0, 1, 46)  /* IOExpander:4 */
#define GP_IFEN       (GROUPB1, 0,  5, -1, NULL,  0, 2, 18)  /* IOExpander:5 */
#define GP_ITAD1      (GROUPB1, 0,  6, -1, NULL,  0, 2, 46)  /* IOExpander:6 */
#define GP_IFAD       (GROUPB1, 0,  7, -1, NULL,  0, 2, 48)  /* IOExpander:7 */
#define GP_IDEN       (GROUPB1, 1,  0, -1, NULL,  0, 2, 50)  /* IOExpander:8 */
#define GP_ISGL       (GROUPC1, 1,  1, -1, NULL,  0, 1, 14)  /* IOExpander:9 */
#define GP_ILOL       (GROUPC1, 1,  2, -1, NULL,  0, 1, 16)  /* IOExpander:10 */
#define GP_IRTH2      (GROUPC1, 1,  3, -1, NULL,  0, 1, 36)  /* IOExpander:11 */
#define GP_IRTH1      (GROUPC1, 1,  4, -1, NULL,  0, 1, 44)  /* IOExpander:12 */
#define GP_IONL       (GROUPA1, 1,  5, -1, NULL,  0, 2, 44)  /* IOExpander:13 */
#define GP_ILDP       (GROUPA1, 1,  6, -1, NULL,  0, 2,  4)  /* IOExpander:14 */
#define GP_IFPT       (GROUPA1, 1,  7, -1, NULL,  0, 2, 32)  /* IOExpander:15 */

#define GP_SETLOW       6       /* lowest "set" number of GPIOs in use */
#define GP_SETHIGH      9       /* lowest "set" number of GPIOs in use */

/*
 * GPIOs are controlled by a set of 11 registers (DR, GDIR, PSR, ICR1, ICR2,
 *  IMR, ISR, EDGE_SEL, DR_SET, DR_CLEAR, and DR_TOGGLE).  These registers
 *  appear at fixed offsets from a base address.  This
 *  macro calculates that base address.
 */
#define GP_REGPTR(setnum, off)                                          \
    ({                                                                  \
        ((volatile uint32_t *)                                          \
            (((setnum) * 0x4000)                                        \
             + (((setnum) < 5) ? 0x401b4000                             \
                                : ((setnum) == 5 ? 0x400ac000           \
                                                  : 0x41fe8000))        \
             + (off)));                                                 \
    })

#define GP_REG_DR_OFF           0x00
#define GP_REG_GDIR_OFF         0x04
#define GP_REG_PSR_OFF          0x08
#define GP_REG_ICR1_OFF         0x0c
#define GP_REG_ICR2_OFF         0x10
#define GP_REG_IMR_OFF          0x14
#define GP_REG_ISR_OFF          0x18
#define GP_REG_EDGE_SEL_OFF     0x1c
#define GP_REG_DR_SET_OFF       0x84
#define GP_REG_DR_CLEAR_OFF     0x88
#define GP_REG_DR_TOGGLE_OFF    0x8c


#define _GP_DR_REG(a, b, c, d, e, f, g, h)                              \
    GP_REGPTR(b, GP_REG_DR_OFF)
#define GP_DR_REG(gp) _GP_DR_REG gp
#define _GP_GDIR_REG(a, b, c, d, e, f, g, h)                            \
    GP_REGPTR(b, GP_REG_GDIR_OFF)
#define GP_GDIR_REG(gp) _GP_GDIR_REG gp
#define _GP_PSR_REG(a, b, c, d, e, f, g, h)                             \
    GP_REGPTR(b, GP_REG_PSR_OFF)
#define GP_PSR_REG(gp)  _GP_PSR_REG gp
#define _GP_ICR1_REG(a, b, c, d, e, f, g, h)                            \
    GP_REGPTR(b, GP_REG_ICR1_OFF)
#define GP_ICR1_REG(gp) _GP_ICR1_REG gp
#define _GP_ICR2_REG(a, b, c, d, e, f, g, h)                            \
    GP_REGPTR(b, GP_REG_ICR2_OFF)
#define GP_ICR2_REG(gp) _GP_ICR2_REG gp
#define _GP_IMR_REG(a, b, c, d, e, f, g, h)                             \
    GP_REGPTR(b, GP_REG_IMR_OFF)
#define GP_IMR_REG(gp)  _GP_IMR_REG gp
#define _GP_ISR_REG(a, b, c, d, e, f, g, h)                             \
    GP_REGPTR(b, GP_REG_ISR_OFF)
#define GP_ISR_REG(gp)  _GP_ISR_REG gp
#define _GP_EDGE_SEL_REG(a, b, c, d, e, f, g, h)                        \
    GP_REGPTR(b, GP_REG_EDGE_SEL_OFF)
#define GP_EDGE_SEL_REG(gp) _GP_EDGE_SEL_REG gp
#define _GP_DR_SET_REG(a, b, c, d, e, f, g, h)                          \
    GP_REGPTR(b, GP_REG_DR_SET_OFF)
#define GP_DR_SET_REG(gp)   _GP_DR_SET_REG gp
#define _GP_DR_CLEAR_REG(a, b, c, d, e, f, g, h)                        \
    GP_REGPTR(b, GP_REG_DR_CLEAR_OFF)
#define GP_DR_CLEAR_REG(gp) _GP_DR_CLEAR_REG gp
#define _GP_DR_TOGGLE_REG(a, b, c, d, e, f, g, h)                       \
    GP_REGPTR(b, GP_REG_DR_TOGGLE_OFF)
#define GP_DR_TOGGLE_REG(gp)    _GP_DR_TOGGLE_REG gp

#define _GP_GROUP(a, b, c, d, e, f, g, h)       (a)
#define GP_GROUP(gp)  _GP_GROUP gp
#define _GP_SETNUM(a, b, c, d, e, f, g, h)      (b)
#define GP_SETNUM(gp)  _GP_SETNUM gp
#define _GP_BITNUM(a, b, c, d, e, f, g, h)      (c)
#define GP_BITNUM(gp)  _GP_BITNUM gp
#define _GP_PINNUM(a, b, c, d, e, f, g, h)      (d)
#define GP_PINNUM(gp)  _GP_PINNUM gp
#define _GP_TCON(a, b, c, d, e, f, g, h)        (e)
#define GP_TCON(gp)  _GP_TCON gp
#define _GP_TPIN(a, b, c, d, e, f, g, h)        (f)
#define GP_TPIN(gp)  _GP_TPIN gp
#define _GP_PERCON(a, b, c, d, e, f, g, h)      (g)
#define GP_PERCON(gp)  _GP_PERCON gp
#define _GP_PERPIN(a, b, c, d, e, f, g, h)      (g)
#define GP_PERPIN(gp)  _GP_PERPIN gp

#define _GP_TUPLE(a, b, c, d, e, f, g, h)                               \
    (a), (b), (c), (d), (e), (f), (g), (h)
#define GP_TUPLE(gp)        _GP_TUPLE gp

#define GP_MASK(gp)         (1 << GP_BITNUM(gp))

/* Write a value to a port expander GPIO .  Caller needs to have made
 *  sure this is really a port-expansion GPIO. */
#define _GP_PORTEXP_WRITE(pstatep, setnum, bitnum, val)                 \
    do {                                                                \
        uint8_t *_cachedregp, _cachedval;                               \
                                                                        \
        _cachedregp = &((pstatep)->pexpregs[PCA9535_CMD_OUTP0           \
                                            + (setnum)]);               \
        _cachedval = *_cachedregp;                                      \
        /* Port expander GPIO */                                        \
        if ((val) != 0) {                                               \
            /* Turning bit on */                                        \
            _cachedval |= (1 << (bitnum));                              \
        }                                                               \
        else {                                                          \
            /* Turning bit off */                                       \
            _cachedval  &= ~(1 << (bitnum));                            \
        }                                                               \
        portexp_write((pstatep),                                        \
                      PCA9535_CMD_OUTP0 + (setnum), _cachedval);        \
        *_cachedregp = _cachedval;                                      \
    } while (FALSE)

/* Write a value to a traditional GPIO */
#define _GP_TRADGPIO_WRITE(setnum, bitnum, val)                          \
    do {                                                                \
        if ((val) != 0) {                                               \
            /* Turning bit on */                                        \
            *GP_REGPTR((setnum), GP_REG_DR_SET_OFF) = (1 << (bitnum));  \
        }                                                               \
        else {                                                          \
            /* Turning bit off */                                       \
            *GP_REGPTR((setnum), GP_REG_DR_CLEAR_OFF) = (1 << (bitnum)); \
        }                                                               \
    } while (FALSE)


/*
 * General purpose GPIO writer.  Will work for either
 *  traditional GPIOs or GPIOs on the port expander.
 */
#define GP_WRITE(pstatep, gp, val)                                      \
    do {                                                                \
        if (((_GP_GROUP gp) & 1) != 0) {                                \
            /* This is expander GPIO */                                 \
            _GP_PORTEXP_WRITE((pstatep),                                \
                              _GP_SETNUM gp, _GP_BITNUM gp, (val));     \
        }                                                               \
        else {                                                          \
            /* This is a traditional GPIO */                            \
            _GP_TRADGPIO_WRITE(_GP_SETNUM gp, _GP_BITNUM gp, (val));    \
        }                                                               \
    } while (FALSE)

/* Read a value from a port expander GPIO.  Caller must have made sure
 *  this is a port expander GPIO. */
#define _GP_PORTEXP_READ(pstatep, setnum, bitnum)                       \
    (((portexp_read((pstatep), (setnum))                                \
       & (1 << (bitnum))) != 0) ? HIGH : LOW)

/* Read a value from a traditional GPIO.  Caller must ensure that this is
 * a traditional GPIO. */
#define _GP_TRADGPIO_READ(setnum, bitnum)                               \
    (*GP_REGPTR((setnum), GP_REG_DR_OFF) & (1 << (bitnum)))

/*
 * General purpose GPIO reader.  Will work for both traditional and
 *  port expander GPIOs.
 */
#define GP_READ(pstatep, gp)                                            \
    ({                                                                  \
        uint32_t        _val;                                           \
        if (((_GP_GROUP gp) & 1) != 0) {                                \
            _val = _GP_PORTEXP_READ((pstatep),                          \
                                    _GP_SETNUM gp, _GP_BITNUM gp);      \
        }                                                               \
        else {                                                          \
            _val = _GP_TRADGPIO_READ(_GP_SETNUM gp, _GP_BITNUM gp);     \
        }                                                               \
        _val;                                                           \
    })

/*
 * Pertec command bits.  These were carefully selected to exactly
 *  match the GPIO masks.
 */
#define PCMD_IWFM   GP_MASK(GP_IWFM)
#define PCMD_IERASE GP_MASK(GP_IERASE)
#define PCMD_IEDIT  GP_MASK(GP_IEDIT)
#define PCMD_IWRT   GP_MASK(GP_IWRT)
#define PCMD_IREV   GP_MASK(GP_IREV)

/* Assert that all these command bits are in the same word as IGO */
ASSERT_CT(GP_SETNUM(GP_IWRT) == GP_SETNUM(GP_IGO));
ASSERT_CT(GP_SETNUM(GP_IERASE) == GP_SETNUM(GP_IGO));
ASSERT_CT(GP_SETNUM(GP_IWFM) == GP_SETNUM(GP_IGO));
ASSERT_CT(GP_SETNUM(GP_IEDIT) == GP_SETNUM(GP_IGO));
ASSERT_CT(GP_SETNUM(GP_IREV) == GP_SETNUM(GP_IGO));


#define GP_MAXINREGS      4       /* number of input registers */

#define TAPEBUFSIZ      32768
#define TAPENRECS       16           /* must be a power of two */


/* Maximum tape length (used to limit some operation times) */
#define MAXTAPELEN      3600        /* in feet */
#define INPERFT         12          /* in inches/foot */
#define REWINDSPEED     100         /* in inches/second */
#define PLAYSPEED       25          /* in inches/second */
#define INVMICRO        1000000     /* number of micro-X per X */

/* Define a string of name n with value s which will be in cheap memory */
#define CONSTSTRDEF(n, s)                                               \
    const char n[] PROGMEM = s

/*
 * Read a value atomically.  This is important for multi-byte values.
 */
#define ATOMIC_READ(val)                                                \
    ({                                                                  \
        typeof(val) __tmp;                                              \
        noInterrupts();                                                 \
        __tmp = (val);                                                  \
        interrupts();                                                   \
        __tmp;                                                          \
    })

#define ATOMIC_WRITE(lval, rval)                                        \
    ({                                                                  \
        noInterrupts();                                                 \
        (lval) = (rval);                                                \
        interrupts();                                                   \
        (rval);                                                         \
    })

/*
 * Swap the value in a variable with a new value, atomically.
 *  Returns the old value.
 */
#define ATOMIC_SWAP(lval, rval)                                         \
    ({                                                                  \
        typeof(lval) __tmp;                                             \
        noInterrupts();                                                 \
        __tmp = (lval);                                                 \
        (lval) = (rval);                                                \
        interrupts();                                                   \
        __tmp;                                                          \
    })

/*
 * Perform a nonatomic increment, wrapped to the given modulus.
 *  modulus must be a power of 2
 */
#define NONATOMIC_INCR1_MOD(val, modulus)                               \
    (val) = ((val) + 1) & ((modulus)-1)

/*
 * Given a ring buffer of size 2**n with read and write pointers,
 *  calculate the available size, which will be between 0 and
 *  2**n-1 (because empty is when the read and write indices have
 *  the same value).
 */
#define RINGPTR_FREE(wrptr, rdptr, ringsize)                            \
    (((uint16_t) ((rdptr) - (wrptr) - 1)) & ((ringsize)-1))

/*
 * Given the same information, calculate the amount of
 *  *used* space in the ring.
 */
#define RINGPTR_USED(wrptr, rdptr, ringsize)                            \
    (((uint16_t) ((wrptr) - (rdptr))) & ((ringsize)-1))

/*
 * Determine if a ring is fully empty.
 */
#define RING_ISEMPTY(wridx, rdidx, ringsize)                            \
    ((wridx) == (rdidx))

/* Same thing, but with pointers */
#define RINGPTR_ISEMPTY(wrptr, rdptr, ringsize)                         \
    ((wrptr) == (rdptr))


/*
 * Determine if a ring is completely full
 */
#define RING_ISFULL(wridx, rdidx, ringsize)                             \
    ((((wridx) + 1 - (rdidx)) & ((ringsize)-1)) == 0)

/* Same thing, but with pointers */
#define RINGPTR_ISFULL(wrptr, rdptr, ringsize)                          \
    ((((uint16_t) ((wrptr) + 1 - (rdptr))) & ((ringsize)-1)) == 0)

/*
 * Given a ring pointer, figure out how many elements
 *  there are between the pointer and the end of the ring.
 *  Returns a uint16_t.
 */
#define RINGPTR_TOEND(rptr, ringbase, ringsize)                         \
    (uint16_t) ((ringbase) + (ringsize) - (rptr))

/*
 * Wrap a pointer in a ring buffer.  This assumes the
 *  pointer has already been incremented.
 */
#if 1
#define RINGPTR_WRAP(ptr, ringbase, ringsize)                           \
    do {                                                                \
        if ((ptr) >= ((ringbase) + (ringsize))) {                       \
            (ptr) -= (ringsize);                                        \
        }                                                               \
    } while (0 != 0)
#elif 0
#define RINGPTR_WRAP(ptr, ringbase, ringsize)                           \
    do {                                                                \
        if ((ptr) == ((ringbase) + (ringsize))) {                       \
            (ptr) = (ringbase);                                         \
        }                                                               \
    } while (0 != 0)
#else
#define RINGPTR_WRAP(ptr, ringbase, ringsize)                           \
    (ptr) = ((ringbase) + ((uint16_t) ((ptr) - (ringbase)) & ((ringsize)-1)))
#endif

/* External interrupt stuff */
    /*
     * Interrupt sense control values. Reference: [3]
     *  p. 110 Section 15.2.1 External Interrupts, Register Description,
     *  EICRA - External Interrupt Control Register A, Table 15-1.
     *  "Interrupt Sense Control"
     */
#define ISC_LOW     0
#define ISC_ANYEDGE 1
#define ISC_FALLING 2
#define ISC_RISING  3

#define ISCBITS(intnum, isc)   (((uint16_t) (isc)) << (2 * (intnum)))
#define EICR_SET(fullval)                                               \
    EICRA = (fullval) & 0xff;                                           \
    EICRB = ((fullval) >> 8) & 0xff

/* Pin change interrupt things */
/* bit for mask in PCICR and PCIFR */
#define PCIXR_MASK_PCINTNUM(pcintnum)    (1 << ((pcintnum)>>3))
/* PC mask register for the given pcintnum */
#define PCMSK_PCINTNUM(pcintnum)    (&PCMSK0 + ((pcintnum)>>3))
#define PCMSK_MASK_PCINTNUM(pcintnum)   (1 << ((pcintnum & 7)))

#define HEXDIGIT(x)     ((char) (((x) & 0xf) < 10 ? (((x) & 0xf) + '0') \
                                 : ((((x) & 0xf) - 10) + 'A')))

/*
 * Port expander definitions.  Reference: [7]
 */
#define PCA9535_ADDR        0x20    /* I2C address */
#define PCA9535_CMD_INP0    0x00    /* input port 0 */
#define PCA9535_CMD_INP1    0x01    /* input port 1 */
#define PCA9535_CMD_OUTP0   0x02    /* output port 0 */
#define PCA9535_CMD_OUTP1   0x03    /* output port 1 */
#define PCA9535_CMD_INV0    0x04    /* (write to) polarity inversion port 0 */
#define PCA9535_CMD_INV1    0x05    /* (write to) polarity inversion port 1 */
#define PCA9535_CMD_CFG0    0x06    /* (write to) configuration port 0 */
#define PCA9535_CMD_CFG1    0x07    /* (write to) configuration port 1 */

#define PCA9535_NCMDS       8       /* total number of "commands" */

#define PCA9535_NPORTS      2

/* Temporary filename length (fits in 8.3) */
#define TMPFNLEN        13

/* -------------------------------------------------------------------------- */
/* New types and type-specific macros */


//typedef __uint24 recsize_t;
typedef uint32_t recsize_t;

/*
 * The strings here are arrays and not pointers to force the
 *  strings into PROGMEM space.  Stupid compiler.
 */
typedef struct _gpinfo {
    const char          ifname[10];
    const char          ifdescrip[34];
    uint8_t             group;
    uint8_t             setnum;
    uint8_t             bitnum;
    int8_t              pinnum;
    const char         *tcon;
    uint8_t             tpin;
    uint8_t             percon;
    uint8_t             perpin;
} gpinfo_t;

#define GPINFO_ENTRY(ifname, ifdescrip, gp)                             \
    { (ifname), (ifdescrip), _GP_TUPLE gp }


/*
 * We gang together input GPIOs by register so that we can do
 *  quick reading/XORing.
 */
typedef struct _gpinreg {
    volatile uint8_t        *pinregp;
    const gpinfo_t          *bits[BITSPERBYTE];
    uint8_t                  mask;
} gpinreg_t;

/*
 * A single tape record, for either writing or reading.
 *  Note that these are live
 */
typedef struct _tape_rec {
    recsize_t        len;       /* length, in bytes.  For reading
                                 *  this value is only written at the end.
                                 *  For writing this value is counted
                                 *  down.  For writing, a value of 0
                                 *  indicates a filemark. */
    uint8_t          flags;
    /* NOTE: this works out to a 32-bit value that is compatible
     *  with the TAP file format */
#define TAPE_REC_FLAGS_ERR      0x80      /* for receivers only  */
} tape_rec_t;


#define FILEBLKSIZ        512       /* size of a single block to be
                                     *  read/written from/to a file
                                     *  on the SD card.  This must
                                     *  be a power of two.
                                     */


typedef struct _pertexus_state {
    volatile uint8_t         opstate;
    /* common states */
#define PERTEXUS_STATE_OPSTATE_IDLE           0
#define PERTEXUS_STATE_OPSTATE_ERRWAIT        1
#define PERTEXUS_STATE_OPSTATE_ERROR          2
#define PERTEXUS_STATE_OPSTATE_EOT            3
    /* write states */
#define PERTEXUS_STATE_OPSTATE_WRECSTART      4
#define PERTEXUS_STATE_OPSTATE_WINREC         5
#define PERTEXUS_STATE_OPSTATE_WRECDONE       6
#define PERTEXUS_STATE_OPSTATE_WFMWAIT        7
    /* read states */
#define PERTEXUS_STATE_OPSTATE_RPAUSED        8
#define PERTEXUS_STATE_OPSTATE_RSTART         9
#define PERTEXUS_STATE_OPSTATE_RINREC         10
#define PERTEXUS_STATE_OPSTATE_RFMSEEN        11
#define PERTEXUS_STATE_OPSTATE_ROVERRUN       12

    uint8_t          fmcount;       /* consecutive filemark count */
    uint8_t          driveaddr;
    volatile uint8_t         rdrec, wrrec;
    uint8_t        * volatile rdptr, * volatile wrptr;
    uint8_t          harderrcount;  /* hard errors seen in this record */
    recsize_t         recbytes;      /* count down for transmit, count
                                     *  up for receive */
#if defined(OPT_INTCOUNTS)
    struct {
        unsigned long        icer;
        unsigned long        iher;
        unsigned long        iwstr;
        unsigned long        irstr;
        unsigned long        idby;
        unsigned long        ifmk;
        unsigned long        ieot;
/*        unsigned long        portexp; */
    } intcounts;
#endif      /* defined(OPT_INTCOUNTS) */
    int             filesleft;
    unsigned int    filesdone;
    unsigned int    recsdone;
    uint32_t        startcmd;
    tape_rec_t      taperecs[TAPENRECS];
    uint8_t         buf[TAPEBUFSIZ];
    uint8_t         wrnextbyte;
    bool_t          wrlwd;
    cmdif_t         cmdif;
    gpinreg_t       gpinregs[GP_MAXINREGS];
    unsigned int    ninregs;
    uint8_t         pexpregs[PCA9535_NCMDS];
    uint8_t         gpinvals[GP_MAXINREGS];
    bool_t          sdcardinit;
    TwoWire        *portexp;
    SDClass         *sdp;
} pertexus_state_t;

/* Interrupt configurations: each of these bits indicates that the
 *  given interrupt should be enabled */
#define INTCFG_IFMK     0x00000001
#define INTCFG_ICER     0x00000002
#define INTCFG_IHER     0x00000004
#define INTCFG_IWSTR    0x00000008
#define INTCFG_IRSTR    0x00000010
#define INTCFG_IEOT     0x00000020
/* #define INTCFG_PORTEXP  0x00000040 */      /* LPI2C4_INT port expander int */
#define INTCFG_IDBY     0x00000080

/* Internal readonly data mapping these interrupts to GPIO set
 *  numbers and bitmasks */
typedef struct _pertexus_intcfgmap {
    uint32_t         intcfg;        /* interrupt configuration bit */
    uint32_t         bitmask;
    unsigned int     setnum;
} pertexus_intcfgmap_t;

/* Interrupt configurations for different modes of operation: */
#define INTCFG_IDLE     (INTCFG_IFMK | INTCFG_ICER                      \
                         | INTCFG_IHER | INTCFG_IWSTR                   \
                         | INTCFG_IRSTR | INTCFG_IEOT                   \
                         /* | INTCFG_PORTEXP */ | INTCFG_IDBY)
#define INTCFG_READING  (INTCFG_IFMK /* | INTCFG_ICER */                \
                         | INTCFG_IHER /* | INTCFG_IWSTR */             \
                         | INTCFG_IRSTR | INTCFG_IEOT                   \
                         /* | INTCFG_PORTEXP  */| INTCFG_IDBY)
#define INTCFG_WRITING  (INTCFG_IFMK /* | INTCFG_ICER */                \
                         | INTCFG_IHER | INTCFG_IWSTR                   \
                         /* | INTCFG_IRSTR */ | INTCFG_IEOT             \
                         /* | INTCFG_PORTEXP */ | INTCFG_IDBY)
#define INTCFG_ALLOFF  0


/* -------------------------------------------------------------------------- */
/* Static prototypes */

/* Functions that must run very quickly */
static FASTRUN inline void writerecstart(pertexus_state_t *pstatep);
static inline void readrecstart(pertexus_state_t *pstatep);
static inline void readrecfinish(pertexus_state_t *pstatep, bool_t eof);
static FASTRUN void ifreset(pertexus_state_t *pstatep);
static FASTRUN void pcmd(pertexus_state_t *pstatep, uint32_t cmdbits);
static FASTRUN void irstr_isr(void);
static FASTRUN void iwstr_isr(void);
static FASTRUN void idby_isr(void);
static FASTRUN void ifmk_isr(void);
static FASTRUN void icer_isr(void);
static FASTRUN void iher_isr(void);
static FASTRUN void ieot_isr(void);
#if 0
static FASTRUN void portexp_isr(void);
#endif

/* Medium-speed functions */
static void interrupts_init(pertexus_state_t *pstatep);
static void interrupts_set(pertexus_state_t *pstatep, uint32_t intcfg);
static unsigned int portexp_read(pertexus_state_t *pstatep, int portnum);
static void portexp_write(pertexus_state_t *pstatep, unsigned int bytenum,
                          unsigned int val);
static void portexp_writeall(pertexus_state_t *pstatep, unsigned int bytenum,
                             unsigned int val);
static bool_t bitwait(volatile uint32_t *regp, uint32_t bitmask,
                      bool_t bitset, Stream *instreamp, unsigned long timeout);
static unsigned int atohex(const char *strp);
static void cmd_echo(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_peek(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_poke(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_date(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_reset(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_stats(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_fsf(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_testwrite(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_write(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_read(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_nop(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_rewind(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_unload(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_density(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_testread(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_gpio(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_sd(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_erase(cmdif_t *cmdifp, int argc, char **argv);

#if 0
static void cmd_cmd(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_dump(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_play(cmdif_t *cmdifp, int argc, char **argv);
static void cmd_tapetest(cmdif_t *cmdifp, int argc, char **argv);
static void gp_inreads(uint8_t *valp);
static bool_t gp_inchanges(const uint8_t *valp,
                           const uint8_t *prevp);
static void gp_inprint(Stream *outstreamp,
                       const uint8_t *valp, const uint8_t *prevp);
#endif


/* Slow, rarely-called functions that exist in and run from flash */
static FLASHMEM void gpios_init(pertexus_state_t *pstatep);
static FLASHMEM void gpios_fini(pertexus_state_t *pstatep);
static FLASHMEM void debug_dumpstate(pertexus_state_t *pstatep,
                                     Print *outstreamp);
static FLASHMEM bool_t sdcard_init(pertexus_state_t *pstatep, Print *streamp);
static FLASHMEM void printDirectory(File dir, int numSpaces,
                                    Print *outstreamp);
static FLASHMEM void printSpaces(int num, Print *outstreamp);
static FLASHMEM void printTime(const DateTimeFields tm, Print *outstreamp,
                               bool_t printwday, bool_t printsecs);
static FLASHMEM void printpad(Print *streamp, const char *strp, int padlen);
static FLASHMEM void hexprint(Print *outstream, unsigned int val, int ndigits);
static FLASHMEM void hexprint32(Print *outstream, uint32_t val, int ndigits);
static FLASHMEM time_t getTeensy3Time(void);
static FLASHMEM void pca9535_init(pertexus_state_t *pstatep);
static FLASHMEM bool_t tmpfn_gen(FS *fsp, char *bufp);
static FLASHMEM bool_t tapfilerev(pertexus_state_t *pstatep, Print *outprintp,
                                  File *writefile, File *tmpfile,
                                  uint32_t lastfooteroff);
static FLASHMEM bool_t tapfilefinalize(pertexus_state_t *pstatep,
                                       Print *outprintp, File *writefile,
                                       uint32_t lastfooteroff);
static FLASHMEM void drivesel(pertexus_state_t *pstatep);
static FLASHMEM void drivedesel(pertexus_state_t *pstatep);

/* -------------------------------------------------------------------------- */
/* Local constant data */

/*
 * Yuck.  In order to get our strings out of RAM and into
 *  PROGMEM we have to do this.
 */

CONSTSTRDEF(_cmd_echo_name, "echo");
static CONSTSTRDEF(_cmd_echo_brief, "echo back command arguments");

static CONSTSTRDEF(_cmd_date_name, "date");
static CONSTSTRDEF(_cmd_date_brief, "read or change the current date/time");

static CONSTSTRDEF(_cmd_fsf_name, "fsf");
static CONSTSTRDEF(_cmd_fsf_brief,
                    "skip ahead some number of filenarks");
static CONSTSTRDEF(_cmd_fsf_full,
    "Usage: fsf [number-of-filemarks]\r\n"
    "\r\n"
    "Where number-of-filemarks is the number of filemarks to\r\n"
    "skip forward.  If it is not specified it defaults to 1.\r\n");

static CONSTSTRDEF(_cmd_help_name, "help");
static CONSTSTRDEF(_cmd_help_brief, "provide command help");
static CONSTSTRDEF(_cmd_help_full,
    "Usage: help [command-name]\r\n"
    "\r\n"
    "Using 'help' by itself will print a list of\r\n"
    "all possible commands");

static CONSTSTRDEF(_cmd_reset_name, "reset");
static CONSTSTRDEF(_cmd_reset_brief, "reset the Pertec interface");

static CONSTSTRDEF(_cmd_rewind_name, "rewind");
static CONSTSTRDEF(_cmd_rewind_brief, "rewind the tape");

static CONSTSTRDEF(_cmd_unload_name, "unload");
static CONSTSTRDEF(_cmd_unload_brief, "rewind and unload the tape");

static CONSTSTRDEF(_cmd_stats_name, "stats");
static CONSTSTRDEF(_cmd_stats_brief, "print out statistics");

static CONSTSTRDEF(_cmd_testread_name, "testread");
static CONSTSTRDEF(_cmd_testread_brief, "read test records of data");

static CONSTSTRDEF(_cmd_testwrite_name, "testwrite");
static CONSTSTRDEF(_cmd_testwrite_brief, "write test records of data");

static CONSTSTRDEF(_cmd_write_name, "write");
static CONSTSTRDEF(_cmd_write_brief, "write a tape image to tape");

static CONSTSTRDEF(_cmd_read_name, "read");
static CONSTSTRDEF(_cmd_read_brief, "read a tape image from tape");

static CONSTSTRDEF(_cmd_nop_name, "nop");
static CONSTSTRDEF(_cmd_nop_brief, "do a NO-OP");

static CONSTSTRDEF(_cmd_peek_name, "peek");
static CONSTSTRDEF(_cmd_peek_brief, "peek a byte");

static CONSTSTRDEF(_cmd_poke_name, "poke");
static CONSTSTRDEF(_cmd_poke_brief, "poke a byte");

static CONSTSTRDEF(_cmd_cmd_name, "cmd");
static CONSTSTRDEF(_cmd_cmd_brief, "do an aribtrary IGO command");

static CONSTSTRDEF(_cmd_density_name, "density");
static CONSTSTRDEF(_cmd_density_brief, "select the drive's density");

static CONSTSTRDEF(_cmd_gpio_name, "gpio");
static CONSTSTRDEF(_cmd_gpio_brief, "diddle GPIOs to confirm wiring");

static CONSTSTRDEF(_cmd_dump_name, "dump");
static CONSTSTRDEF(_cmd_dump_brief,
                    "dump out previously read tape data");

static CONSTSTRDEF(_cmd_play_name, "play");
static CONSTSTRDEF(_cmd_play_brief,
                    "do a series of file skips to reach end of tape");

static CONSTSTRDEF(_cmd_erase_name, "erase");
static CONSTSTRDEF(_cmd_erase_brief, "securely erase an entire tape");

static CONSTSTRDEF(_cmd_sd_name, "sd");
static CONSTSTRDEF(_cmd_sd_brief, "do SD commands");

static CONSTSTRDEF(_cmd_tapetest_name, "tapetest");
static CONSTSTRDEF(_cmd_tapetest_brief, "destructively test the medium");

/*
 * The command dispatch table.
 */
static const cmdifdisp_t cmdifdisp[] PROGMEM = {
    { &(_cmd_echo_name[0]), &(_cmd_echo_brief[0]), NULL, &cmd_echo },
    { &(_cmd_date_name[0]), &(_cmd_date_brief[0]), NULL, &cmd_date },
    { &(_cmd_gpio_name[0]), &(_cmd_gpio_brief[0]), NULL, &cmd_gpio },
    { &(_cmd_sd_name[0]), &(_cmd_sd_brief[0]), NULL, &cmd_sd },
    { &(_cmd_reset_name[0]), &(_cmd_reset_brief[0]), NULL, &cmd_reset },
    { &(_cmd_testread_name[0]), &(_cmd_testread_brief[0]),
      NULL, &cmd_testread },
    { &(_cmd_fsf_name[0]), &(_cmd_fsf_brief[0]),
      &(_cmd_fsf_full[0]), &cmd_fsf },
    { &(_cmd_rewind_name[0]), &(_cmd_rewind_brief[0]), NULL, &cmd_rewind },
    { &(_cmd_unload_name[0]), &(_cmd_unload_brief[0]), NULL, &cmd_unload },
    { &(_cmd_nop_name[0]), &(_cmd_nop_brief[0]), NULL, &cmd_nop },
    { &(_cmd_density_name[0]), &(_cmd_density_brief[0]), NULL, &cmd_density },
    { &(_cmd_stats_name[0]), &(_cmd_stats_brief[0]), NULL, &cmd_stats },
    { &(_cmd_help_name[0]), &(_cmd_help_brief[0]),
      &(_cmd_help_full[0]), &cmdif_cmd_help },
    { &(_cmd_testwrite_name[0]), &(_cmd_testwrite_brief[0]),
      NULL, &cmd_testwrite },
    { &(_cmd_write_name[0]), &(_cmd_write_brief[0]), NULL, &cmd_write },
    { &(_cmd_read_name[0]), &(_cmd_read_brief[0]), NULL, &cmd_read },
    { &(_cmd_erase_name[0]), &(_cmd_erase_brief[0]), NULL, &cmd_erase },
    { &(_cmd_peek_name[0]), &(_cmd_peek_brief[0]), NULL, &cmd_peek },
    { &(_cmd_poke_name[0]), &(_cmd_poke_brief[0]), NULL, &cmd_poke },
#if 0
    { &(_cmd_cmd_name[0]), &(_cmd_cmd_brief[0]), NULL, &cmd_cmd },
    { &(_cmd_dump_name[0]), &(_cmd_dump_brief[0]), NULL, &cmd_dump },
    { &(_cmd_play_name[0]), &(_cmd_play_brief[0]), NULL, &cmd_play },
    { &(_cmd_tapetest_name[0]), &(_cmd_tapetest_brief[0]),
      NULL, &cmd_tapetest },
#endif
};

/*
 * The readonly command interface data.
 */
static CONSTSTRDEF(_cmd_prompt, "PERTEXUS> ");
static CONSTSTRDEF(_cmd_description,
    "This is the Pertec Magtape Interface Exerciser and Nexus (PERTEXUS)\r\n"
    "\r\n"
    "It provides a simple mechanism to directly control a\r\n"
    "magnetic tape drive attached to an Teensy 4.1\r\n"
   "via a Pertec-type interface (two 50-pin cables)");

static const cmdifro_t cmdifro PROGMEM = {
    &(_cmd_prompt[0]),
    &(_cmd_description[0]),
    &(cmdifdisp[0]),
    NELEM(cmdifdisp)
};

/*
 * GPIO information.
 */


static const gpinfo_t gpinfos[] PROGMEM = {
    GPINFO_ENTRY("IW7",      "Write data bit 7 (LSB)", GP_IW7),
    GPINFO_ENTRY("IW6",      "Write data bit 6 (LSB)", GP_IW6),
    GPINFO_ENTRY("IW5",      "Write data bit 5 (LSB)", GP_IW5),
    GPINFO_ENTRY("IW4",      "Write data bit 4 (LSB)", GP_IW4),
    GPINFO_ENTRY("IW3",      "Write data bit 3 (LSB)", GP_IW3),
    GPINFO_ENTRY("IW2",      "Write data bit 2 (LSB)", GP_IW2),
    GPINFO_ENTRY("IW1",      "Write data bit 1 (LSB)", GP_IW1),
    GPINFO_ENTRY("IW0",      "Write data bit 0 (LSB)", GP_IW0),
    GPINFO_ENTRY("IGO",      "Go pulse (command strobe)", GP_IGO),
    GPINFO_ENTRY("IWRT",     "Write", GP_IWRT),
    GPINFO_ENTRY("IERASE",   "Erase", GP_IERASE),
    GPINFO_ENTRY("IWFM",     "Write filemark", GP_IWFM),
    GPINFO_ENTRY("IEDIT",    "Edit", GP_IEDIT),
    GPINFO_ENTRY("IREV",     "Reverse", GP_IREV),
    GPINFO_ENTRY("ILWD",     "Last word; terminates write", GP_ILWD),
    GPINFO_ENTRY("ISGL",     "*Selected drive fault", GP_ISGL),
    GPINFO_ENTRY("ILOL",     "*Load on line", GP_ILOL),
    GPINFO_ENTRY("IREW",     "Rewind", GP_IREW),
    GPINFO_ENTRY("IWP",      "Write data parity", GP_IWP),
    GPINFO_ENTRY("IRTH2",    "*Write density select 2", GP_IRTH2),
    GPINFO_ENTRY("IRTH1",    "*Write density select 1", GP_IRTH1),
    GPINFO_ENTRY("ICER",     "Correctable error", GP_ICER),
    GPINFO_ENTRY("IHER",     "Hard error", GP_IHER),
    GPINFO_ENTRY("IWSTR",    "Write strobe", GP_IWSTR),
    GPINFO_ENTRY("IRSTR",    "Read strobe", GP_IRSTR),
    GPINFO_ENTRY("IDENT",    "Identifiation burst", GP_IDENT),
    GPINFO_ENTRY("IFMK",     "Filemark", GP_IFMK),
    GPINFO_ENTRY("IR7",      "Read data bit 7 (LSB)", GP_IR7),
    GPINFO_ENTRY("IR6",      "Read data bit 6", GP_IR6),
    GPINFO_ENTRY("IR5",      "Read data bit 5", GP_IR5),
    GPINFO_ENTRY("IR4",      "Read data bit 4", GP_IR4),
    GPINFO_ENTRY("IR3",      "Read data bit 3", GP_IR3),
    GPINFO_ENTRY("IR2",      "Read data bit 2", GP_IR2),
    GPINFO_ENTRY("IR1",      "Read data bit 1", GP_IR1),
    GPINFO_ENTRY("IR0",      "Read data bit 0 (MSB)", GP_IR0),
    GPINFO_ENTRY("IFBY",     "Formatter busy", GP_IFBY),
    GPINFO_ENTRY("IRP",      "Read data parity", GP_IRP),
    GPINFO_ENTRY("ILDP",     "Load point", GP_ILDP),
    GPINFO_ENTRY("IEOT",     "End of tape", GP_IEOT),
    GPINFO_ENTRY("INRZ",     "3200 BPI selected", GP_INRZ),
    GPINFO_ENTRY("IRDY",     "Ready", GP_IRDY),
    GPINFO_ENTRY("IRWD",     "Rewinding", GP_IRWD),
    GPINFO_ENTRY("IFPT",     "File protected", GP_IFPT),
    GPINFO_ENTRY("IDBY",     "Data busy", GP_IDBY),
    GPINFO_ENTRY("ISPEED",   "Early EOT indicator", GP_ISPEED),
    GPINFO_ENTRY("IONL",     "Online", GP_IONL),
    GPINFO_ENTRY("ITAD0",    "Transport address 0 (MSB)", GP_ITAD0),
    GPINFO_ENTRY("IFEN",     "Formatter enable", GP_IFEN),
    GPINFO_ENTRY("IOFL",     "Rewind/unload", GP_IOFL),
    GPINFO_ENTRY("ITAD1",    "Transport address 1 (LSB)", GP_ITAD1),
    GPINFO_ENTRY("IFAD",     "Formatter address", GP_IFAD),
    GPINFO_ENTRY("IDEN",     "*Speed select", GP_IDEN),
};

static const char daysofweek[] PROGMEM = {
    "SunMonTueWedThuFriSat"
};
static const char monthnames[] PROGMEM = {
    "JanFebMarAprMayJunJulAugSepOctNovDec"
};

/* Interrupt configuration mapping */
static const pertexus_intcfgmap_t intcfgmap[] PROGMEM = {
    { INTCFG_IFMK, GP_MASK(GP_IFMK), GP_SETNUM(GP_IFMK) },
    { INTCFG_ICER, GP_MASK(GP_ICER), GP_SETNUM(GP_ICER) },
    { INTCFG_IHER, GP_MASK(GP_IHER), GP_SETNUM(GP_IHER) },
    { INTCFG_IWSTR, GP_MASK(GP_IWSTR), GP_SETNUM(GP_IWSTR) },
    { INTCFG_IRSTR, GP_MASK(GP_IRSTR), GP_SETNUM(GP_IRSTR) },
    { INTCFG_IEOT, GP_MASK(GP_IEOT), GP_SETNUM(GP_IEOT) },
/*    { INTCFG_PORTEXP, GP_MASK(GP_LPI2C4_INT), GP_SETNUM(GP_LPI2C4_INT) }, */
    { INTCFG_IDBY, GP_MASK(GP_IDBY), GP_SETNUM(GP_IDBY) }
};

/* -------------------------------------------------------------------------- */
/* Local read/write data */

static pertexus_state_t pertexus_state;

/* -------------------------------------------------------------------------- */
/* Global read/write data */

/* -------------------------------------------------------------------------- */
/* Global functions */

#if defined(OPT_ETHERNET)
byte ethernet_macaddr[] = { 0xde, 0xad, 0xbe, 0xef, 0xab, 0xba };
#endif

void
setup(void)
{
    /* set the Time library to use Teensy 4.1's RTC to keep time */
    setSyncProvider(&getTeensy3Time);

    pca9535_init(&pertexus_state);

    pertexus_state.driveaddr = 0;

    gpios_init(&pertexus_state);
    interrupts_init(&pertexus_state);

    Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
    while (!Serial) {
        ; // Waiting for Serial Monitor
    }

    Serial.println(F(__FILE__));
    Serial.println(F("Edited " __TIMESTAMP__));
    Serial.println(F("Compiled " __DATE__ " "  __TIME__));
    Serial.println(F("Starting up..."));
    cmdif_init(&(pertexus_state.cmdif), &cmdifro, &Serial);


#if defined(OPT_ETHERNET)
    if (Ethernet.begin(ethernet_macaddr) == 0) {
        Serial.println(F("Failed to configure Ethernet using DHCP"));
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
        }
        else if (Ethernet.linkStatus() == LinkOFF) {
            Serial.println(F("Ethernet cable is not connected."));
        }
        // no point in carrying on, so do nothing forevermore:
        while (true) {
            delay(1);
        }
    }
#endif

}


void
loop(void)
{
    int      inchar;

    if (pertexus_state.cmdif.state == CMDIF_STATE_LISTEN
        && (inchar = Serial.read()) >= 0) {
        cmdif_takechar(&(pertexus_state.cmdif), inchar);
    }
    (void) dpc_dispatch();
}


/* -------------------------------------------------------------------------- */
/* Static functions (FASTRUN).  These are functions that must run quickly. */

/*
 * Initialize a tape record write, either a record with
 *  data or a filemark.  This function
 *  should be called when IDBY is deasserted.
 * This function should be called either from
 *  the interrupt context or if the state is IDLE
 *  (so no interrupt would affect state anyway).
 */
static inline void
writerecstart(pertexus_state_t *pstatep)
{
    uint8_t         rdrec;
    tape_rec_t      *trp;

    rdrec = pstatep->rdrec;
    if (RING_ISEMPTY(pstatep->wrrec, rdrec, TAPENRECS)) {
        /* No new records to get */
        pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;
    }
    else {
        trp = &(pstatep->taperecs[rdrec]);

        if (trp->len == 0) {
            /* We're writing a filemark */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_WFMWAIT;
            pcmd(pstatep, PCMD_IWRT | PCMD_IWFM);
        }
        else if (RINGPTR_ISEMPTY(pstatep->wrptr, pstatep->rdptr, TAPEBUFSIZ)) {
            /* We don't have any data, so we must go to idle */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;
        }
        else {
            pstatep->recbytes = trp->len;
            /* We have at least one byte to send, so prime the pump */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_WRECSTART;
            pcmd(pstatep, PCMD_IWRT);
        }
    }
}

/*
 * (Possibly) start reading data, but only if
 *  we have some space for both data and records.
 *
 * This function is called either from idle (when no
 *  interrupts are coming because we're in PERTEXUS_STATE_OPSTATE_RPAUSED)
 *  or from readrecfinish() from the IDBY interrupt handler
 *  when we're in PERTEXUS_STATE_OPSTATE_RINREC or
 *  PERTEXUS_STATE_OPSTATE_RFMSEEN.
 */
static inline void
readrecstart(pertexus_state_t *pstatep)
{
    /*
     * We have space and records available to receive into.
     *  Start reading.
     */
    pstatep->recbytes = 0;
    pstatep->harderrcount = 0;
    pstatep->opstate = PERTEXUS_STATE_OPSTATE_RSTART;
    pcmd(pstatep, pstatep->startcmd);
}

/*
 * After seeing IDBY go deasserted after a READ command, clean
 *  up and move on.
 *
 * This function is called from the IDBY interrupt only
 *  when we're in PERTEXUS_STATE_OPSTATE_RINREC or
 *  PERTEXUS_STATE_OPSTATE_RFMSEEN.
 */
static inline void
readrecfinish(pertexus_state_t *pstatep, bool_t eof)
{
    uint8_t          wrrec;

    /*
     * Retire the current frame.  Note that we are *guaranteed*
     *  to have a record slot to write to because we would not
     *  have started the read without one.
     */
    /* Note: recbytes will be 0 when we just read a filemark */
    wrrec = pstatep->wrrec;
    pstatep->taperecs[wrrec].len = pstatep->recbytes;
    pstatep->taperecs[wrrec].flags = (pstatep->harderrcount != 0
                                      ? TAPE_REC_FLAGS_ERR
                                      : 0);

    NONATOMIC_INCR1_MOD(wrrec, TAPENRECS);
    pstatep->wrrec = wrrec;

    if (eof && pstatep->filesleft > 0) {
        pstatep->filesleft--;
    }


    /*
     * Have we seen consecutive filemarks?
     */
    if (eof
        && (pstatep->filesleft == 0
            || (pstatep->filesleft == -2 && pstatep->fmcount > 1))) {

        /*
         * We've seen an EOF (IFMK) and we've either reached our
         *  terminal file count or we've seen a double filemark.
         *
         * filesleft      fmcount       action
         * >0             1 or 2        continue.  We're counting down
         *                              a certain number of tape files.
         *                              We will keep going right over
         *                              double EOFs.  The only other
         *                              terminating event is EOT.
         * 0              1 or 2        finish.  We've been counting down
         *                              a certain number of tape files and
         *                              we've reached the last.
         * -1             1 or 2        continue.  We're set up to keep
         *                              reading forever.  Only EOT will
         *                              stop the reading.
         * -2             1             continue.  This means we will
         *                              stop on double-EOF.
         * -2             2             finish
         *
         */
        pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;
    }
    else {
        /*
         * Time to start reading another record.
         *  NOTE: If we are reading in reverse, we really don't
         *   want to start a new read if ILDP is asserted.
         *   Sadly, ILDP is on the port expander, and to read it
         *   takes time, so we would rather do that at idle level.
         *  So, if we're reading in reverse we look at the LPI2C4_INT
         *   line.  If it's asserted (low) then we'll demur and
         *   read the port expander later in the main loop.  If
         *   ILDP is not asserted then we can restart the read in
         *   that loop.
         */
        if (!RING_ISFULL(wrrec, pstatep->rdrec, TAPENRECS)
            && !RINGPTR_ISFULL(pstatep->wrptr, pstatep->rdptr, TAPEBUFSIZ)
            && ((pstatep->startcmd & PCMD_IREV) == 0
                || GP_READ(pstatep, GP_LPI2C4_INT) != 0)) {
            readrecstart(pstatep);
        }
        else {
            /* Go back to the paused state */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_RPAUSED;
        }
    }
}


/*
 * Reset the Pertec interface by deasserting (setting) IFEN and
 *  then reasserting it.
 */
static void
ifreset(pertexus_state_t *pstatep)
{
    /* Reset the interface */
    GP_WRITE(pstatep, GP_IFEN, 1);
    delayMicroseconds(5);
    GP_WRITE(pstatep, GP_IFEN, 0);
}

/*
 * Start a Pertec command.  The command byte is *logical*,
 *  so a 1 in a bit is actually sent as a low signal on the line.
 */
static void
pcmd(pertexus_state_t *pstatep, uint32_t cmdbits)
{
    uint32_t                 mask;

    /*
     * First, write the command byte as is, with IGO low.
     */
    mask = (GP_MASK(GP_IWRT) | GP_MASK(GP_IERASE)
            | GP_MASK(GP_IWFM) | GP_MASK(GP_IEDIT)
            | GP_MASK(GP_IREV));


    /* "set" and "clear" here are inverted from normal use because
     *  these are active-low signals */
#if 0
    Serial.print(F("Writing 0x"));
    hexprint(&Serial, (cmdbits & mask) ^ mask, 2);
    Serial.print(F(" to *0x"));
    hexprint32(&Serial, (uint32_t) GP_DR_SET_REG(GP_IGO), 8);
    Serial.print(F(" (DR_SET) and 0x"));
    hexprint(&Serial, (cmdbits & mask), 2);
    Serial.print(F(" to *0x"));
    hexprint32(&Serial, (uint32_t) GP_DR_CLEAR_REG(GP_IGO), 8);
    Serial.println(F(" (DR_CLEAR)"));
#endif

    *GP_DR_SET_REG(GP_IGO) = (cmdbits & mask) ^ mask;
    *GP_DR_CLEAR_REG(GP_IGO) = (cmdbits & mask);

    /* Wait at least 1 microsecond */
    delayMicroseconds(5);

    /* Now lower IGO */
#if 0
    Serial.print(F("Writing 0x"));
    hexprint(&Serial, GP_MASK(GP_IGO), 2);
    Serial.println(F(" to DR_CLEAR (clearing IGO)"));
#endif
    *GP_DR_CLEAR_REG(GP_IGO) = GP_MASK(GP_IGO);

    /* Wait at least 1 microsecond */
    /* !!! bkd 2022apr04 -- delayMicroseconds() is *way* off, hence
     *  the long period
     *  */
    delayMicroseconds(10);

#if 0
    /* Set the IGO line back high.  This transition causes the action
     *  to start. */
    Serial.print(F("Writing 0x"));
    hexprint(&Serial, GP_MASK(GP_IGO), 2);
    Serial.println(F(" to DR_SET (setting IGO)"));
#endif
    *GP_DR_SET_REG(GP_IGO) = GP_MASK(GP_IGO);

    /* Wait at least 1 microsecond */
    delayMicroseconds(5);

    /* Set everything back up again */
    *GP_DR_SET_REG(GP_IGO) = mask;
}

/*
 * Interrupt handlers.
 */

static void
irstr_isr(void)
{
    uint8_t         *wrptr;
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_RINREC) {
        wrptr = pstatep->wrptr;
        /*
         * Is there any space?
         */
        if (!RINGPTR_ISFULL(wrptr, pstatep->rdptr, TAPEBUFSIZ)) {
            /* Yes.  Stow the datum away */
            *wrptr++ = ~(*GP_DR_REG(GP_IR7) >> GP_BITNUM(GP_IR7));
            RINGPTR_WRAP(wrptr, &(pstatep->buf[0]), TAPEBUFSIZ);
            pstatep->wrptr = wrptr;
            pstatep->recbytes++;
        }
        else {
            /* No space.  We have problems. */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_ROVERRUN;
        }
        /* In either case, we reset the consecutive filemark count */
        pstatep->fmcount = 0;
    }

#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.irstr++;
#endif
}

/* IWSTR interrupt */
/*
 * This is used to write the tape.  The tape drive toggles
 *  IWSTR to elicit data.
 *
 *                    updated by
 *              this func     interruptable code
 * data ring    rdptr         wrptr
 */

/*
 * The tape drive asserts (lowers) IWSTR and then, 1.6 microseconds
 *  later, it deasserts (raises) it.  I believe it is on the deassertion
 *  transition that it samples IWn (and ILWD).
 * We had been attempting to set IWn with this interrupt handler,
 *  triggered by the asserting (downgoing) transition.
 * To be optimal, what
 *  I'm doing is having the interrupt happen on the *upgoing*
 *  transition, and at that time we put the *next* value onto
 *  the wire.  There is inherently enough latency that I believe
 *  this will not cause a problem.
 */


static void
iwstr_isr(void)
{
    uint8_t         *rdptr;
    uint32_t                 regval;
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    /* Put up the new value */
    regval = *GP_DR_REG(GP_IW7);
    regval &= ~(0xff << GP_BITNUM(GP_IW7));
    regval |= pstatep->wrnextbyte << GP_BITNUM(GP_IW7);
    *GP_DR_REG(GP_IW7) = regval;

    /* Possibly assert the last word bit */
    if (pstatep->wrlwd) {
        GP_WRITE(pstatep, GP_ILWD, 0);
    }


    /* Do we have a current record we're writing? */
    if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_WINREC) {
        /*
         * Have we already written the final value?
         */
        if (pstatep->recbytes == 0) {
            pstatep->wrnextbyte = ~0;
            pstatep->wrlwd = FALSE;

            /*
             * We're actually expecting one more IWSTR
             *  deassertion interrupt,
             *  but we won't have anything to do there except
             *  write the above values on the wire.
             */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_WRECDONE;
        }
        else {
            rdptr = pstatep->rdptr;
            /* Do we have data available for the next byte? */
            if (!RINGPTR_ISEMPTY(pstatep->wrptr, rdptr, TAPEBUFSIZ)) {
                pstatep->wrnextbyte = ~(*rdptr++);
                RINGPTR_WRAP(rdptr, &(pstatep->buf[0]), TAPEBUFSIZ);
                pstatep->rdptr = rdptr;

                if (--pstatep->recbytes == 0) {
                    /* Also assert ILWD on last byte */
                    pstatep->wrlwd = TRUE;
                }
            }
            else {
                /* oops...we're out of data; we'll effectively
                 *  re-write the previous byte but we'll end
                 *  the whole thing here. */
                pstatep->wrlwd = TRUE;
                pstatep->opstate = PERTEXUS_STATE_OPSTATE_ERRWAIT;
            }
        }
    }

#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.iwstr++;
#endif
}


/* IDBY interrupt */
static void
idby_isr(void)
{
    bool_t           idby_asserted;
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    idby_asserted = GP_READ(pstatep, GP_IDBY) == 0;

    switch (pstatep->opstate) {

    case PERTEXUS_STATE_OPSTATE_ERRWAIT:
        /*
         * We ran out of data (underran), so we're just
         *  waiting for the command to complete.
         */
        if (!idby_asserted) {
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_ERROR;
        }
        break;

    /*
     * Write states
     */
    case PERTEXUS_STATE_OPSTATE_WRECSTART:
        /*
         * We did an IWRT waiting for IDBY to go active so
         *  we can put a byte on the wire.  Note that we would
         *  not have done the IWRT command if we had not
         *  had at least one byte of data available to send.
         */
        if (idby_asserted) {
            uint8_t     *rdptr;
            uint32_t         regval;

            rdptr = pstatep->rdptr;
            /* Put the first byte directly onto the wire. */
            regval = *GP_DR_REG(GP_IW7);
            regval &= ~(0xff << GP_BITNUM(GP_IW7));
            regval |= ~(*rdptr++) << GP_BITNUM(GP_IW7);
            *GP_DR_REG(GP_IW7) = regval;

            RINGPTR_WRAP(rdptr, &(pstatep->buf[0]), TAPEBUFSIZ);

            if (--pstatep->recbytes == 0) {
                /* This is a 1 byte record.  Assert ILWD. */
                GP_WRITE(pstatep, GP_ILWD, 0);
                pstatep->wrnextbyte = ~0;       /* float them */
                pstatep->wrlwd = FALSE;         /* leave ILWD alone */

                pstatep->opstate = PERTEXUS_STATE_OPSTATE_WRECDONE;
            }
            else {
                /*
                 * This is a 2 or more byte record.  Set up the
                 *  next data byte in the GPRIOn register.
                 */
                pstatep->wrnextbyte = ~(*rdptr++);
                RINGPTR_WRAP(rdptr, &(pstatep->buf[0]), TAPEBUFSIZ);

                if (--pstatep->recbytes == 0) {
                    /* This is a 2 byte record.  Assert ILWD on the next. */
                    pstatep->wrlwd = TRUE;         /* assert ILWD */
                }
                else {
                    pstatep->wrlwd = FALSE;         /* leave ILWD alone */
                }
                pstatep->opstate = PERTEXUS_STATE_OPSTATE_WINREC;
            }

            pstatep->rdptr = rdptr;
        }
        break;

    case PERTEXUS_STATE_OPSTATE_WRECDONE:
        /*
         * We were waiting for IDBY to become deasserted
         *  after we wrote the final byte of a record.
         */
        if (!idby_asserted) {
            /* Clear the ILWD bit. */
            GP_WRITE(pstatep, GP_ILWD, 1);

            pstatep->recsdone++;

            /* Advance the record */
            NONATOMIC_INCR1_MOD(pstatep->rdrec, TAPENRECS);

            /* Try to start the next record */
            writerecstart(pstatep);
        }
        break;

    case PERTEXUS_STATE_OPSTATE_WFMWAIT:
        /*
         * We were waiting for IDBY to become deasserted
         *  after we wrote a filemark (WFM command).
         */
        if (!idby_asserted) {
            pstatep->filesdone++;

            /* Advance the record */
            NONATOMIC_INCR1_MOD(pstatep->rdrec, TAPENRECS);

            /* Try to start the next record */
            writerecstart(pstatep);
        }
        break;

    /*
     * Read states
     */
    case PERTEXUS_STATE_OPSTATE_RSTART:
        if (idby_asserted) {
            /* Move to RINREC */
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_RINREC;
        }
        break;

    case PERTEXUS_STATE_OPSTATE_RINREC:
    case PERTEXUS_STATE_OPSTATE_RFMSEEN:
        /*
         * IDBY went inactive while we were reading a record.  We
         *  may instead seen a filemark.  In any case, the recovery
         *  action is the same.
         */
        if (!idby_asserted) {
            readrecfinish(pstatep,
                          pstatep->opstate == PERTEXUS_STATE_OPSTATE_RFMSEEN);
        }
        break;

    }

#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.idby++;
#endif
}

/* IFMK interrupt */
static void
ifmk_isr(void)
{
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_RINREC) {
        /* !!! should we make sure that IFMK is indeed asserted? */
        pstatep->fmcount++;

        pstatep->opstate = PERTEXUS_STATE_OPSTATE_RFMSEEN;
    }
#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.ifmk++;
#endif
}

/* ICER interrupt */
static void
icer_isr(void)
{
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.icer++;
#endif
}

/* IHER interrupt */
static void
iher_isr(void)
{
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    pstatep->harderrcount++;
#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.iher++;
#endif
}

/* IEOT interrupt */
static void
ieot_isr(void)
{
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    if (pstatep->opstate != PERTEXUS_STATE_OPSTATE_IDLE) {
        pstatep->opstate = PERTEXUS_STATE_OPSTATE_EOT;
    }

#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.ieot++;
#endif
}

#if 0
/* Port expander interrupt */
static void
portexp_isr(void)
{
    pertexus_state_t        *pstatep;

    pstatep = &pertexus_state;

    pstatep->pexpintpend++;

#if defined(OPT_INTCOUNTS)
    pstatep->intcounts.portexp++;
#endif
}
#endif

/* -------------------------------------------------------------------------- */
/* Static functions (NORMAL).  These are medium-speed functions that
 *  should execute out of RAM */

static void
interrupts_init(pertexus_state_t *pstatep)
{
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_IRSTR)),
                    &irstr_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_IWSTR)),
                    &iwstr_isr, RISING);
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_IDBY)),
                    &idby_isr, CHANGE);
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_IFMK)),
                    &ifmk_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_ICER)),
                    &icer_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_IHER)),
                    &iher_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_IEOT)),
                    &ieot_isr, FALLING);
#if 0
    attachInterrupt(digitalPinToInterrupt(GP_PINNUM(GP_LPI2C4_INT)),
                    &portexp_isr, FALLING);
#endif
}

/*
 * Set up interrupts for a particular operation
 */
static void
interrupts_set(pertexus_state_t *pstatep, uint32_t intcfg)
{
    unsigned int        setnum, intidx;
    uint32_t            imrval, isrval, newimrval;

    /*
     * For each of the GPIO set numbers we're using,
     *  go through all possible interrupt configuration bits
     *  and figure out new masks.
     * Note the optimization: if we aren't turning any interrupts
     *  on for a given GPIO we *don't* bother writing to GPIOnISR.
     * Also, if we are not changing the interrupt mask we also
     *  don't bother writing to GPIOnIMR.
     * This might be over-optimization, but I'd like to avoid
     *  glitching if possible.
     */
    for (setnum = GP_SETLOW; setnum <= GP_SETHIGH; setnum++) {
        imrval = *GP_REGPTR(setnum, GP_REG_IMR_OFF);
        isrval = 0;
        newimrval = imrval;
        for (intidx = 0; intidx < NELEM(intcfgmap); intidx++) {
            if (intcfgmap[intidx].setnum == setnum) {
                if ((intcfgmap[intidx].intcfg & intcfg) != 0) {
                    /* We're turning this interrupt on */
                    newimrval |= intcfgmap[intidx].bitmask;
                    isrval |= intcfgmap[intidx].bitmask;
                }
                else {
                    /* We're turning this interrupt off */
                    newimrval &= ~intcfgmap[intidx].bitmask;
                }
            }
        }
        /* Did we turn on any interrupts that we should clear first? */
        if (isrval != 0) {
            *GP_REGPTR(setnum, GP_REG_ISR_OFF) = isrval;
        }
        /* Did we change the interrupt mask? */
        if (newimrval != imrval) {
            *GP_REGPTR(setnum, GP_REG_IMR_OFF) = newimrval;
        }
    }
}

/*
 * Port expander primitive functions.  Reference: [7]
 */

/*
 * Read from a "port" or from both ports.  There are two
 *  sets of GPIOs in the PCA9535: "port" 0 and "port" 1.
 *  Each port contains 8 GPIOs, and is represented by
 *  3 write-only registers (PCA9535_CMD_OUTPx, PCA9535_CMD_INVx,
 *  PCA9535_CMD_CFGx) and 1 read-only register (PCA9535_CMD_INPx).
 *
 * This function will read from either one or both of the
 *  PCA9535_CMD_INPx registers.
 *
 * A portnum value of 0 or 1 will read exactly one byte from
 *  the PCA9535 -- the corresponding PCA9535_CMD_INPx
 *  register.
 *
 * A portnum value of < 0, however, will read *both* PCA9535_CMD_INPx
 *  registers and return a 16-bit value with PCA9535_CMD_INP0 in
 *  the low bits and PCA9535_CMD_INP1 in the high bits.
 *
 * This is especially useful for dealing with interrupts from
 *  the PCA9535.
 */
static unsigned int
portexp_read(pertexus_state_t *pstatep, int portnum)
{
    uint8_t              readval;
    unsigned int         retval, baseport;

    pstatep->portexp->beginTransmission(PCA9535_ADDR);
    baseport = (portnum <= 0 ? PCA9535_CMD_INP0 : PCA9535_CMD_INP1);
    pstatep->portexp->write(byte(baseport));
    pstatep->portexp->endTransmission();

    pstatep->portexp->requestFrom(PCA9535_ADDR, portnum < 0 ? 2 : 1);

    readval = (unsigned int) pstatep->portexp->read();
    pstatep->pexpregs[baseport] = readval;
    retval = readval;

    if (portnum < 0) {
        readval = (unsigned int) pstatep->portexp->read();
        pstatep->pexpregs[PCA9535_CMD_INP1] = readval;
        retval |= (readval << 8);
    }

    return retval;
}

/* Write a single byte from the particular register */
static void
portexp_write(pertexus_state_t *pstatep, unsigned int bytenum, unsigned int val)
{
    /* Keep the cached value */
    pstatep->pexpregs[bytenum] = (uint8_t) val;

    pstatep->portexp->beginTransmission(PCA9535_ADDR);
    pstatep->portexp->write(byte(bytenum));
    pstatep->portexp->write(byte(val & 0xff));
    pstatep->portexp->endTransmission();
}

/* Write all ports from the given base register with the given value */
static void
portexp_writeall(pertexus_state_t *pstatep, unsigned int bytenum,
                 unsigned int val)
{
    unsigned int         portnum;

    for (portnum = 0; portnum < PCA9535_NPORTS; portnum++) {
        portexp_write(pstatep, bytenum + portnum, val);
    }
}

/*
 * Poll-wait for a bit to be set or clear.
 * If streamp is not NULL this will also poll for a ^C to abort.
 * Returns TRUE if the bit was set or cleared as asked for,
 * FALSE if we terminated for another reason (either ^C or timeout).
 * timeout is in microseconds.  0 means wait forever if necessary.
 */
static bool_t
bitwait(volatile uint32_t *regp, uint32_t bitmask, bool_t bitset,
        Stream *instreamp, unsigned long timeout)
{
    unsigned long        start, curtime;
    uint32_t              regval;
    int                  inchar;

    start = micros();

    while (TRUE) {
        /* Test to see if the bit has achieved the requested state */
        regval = *regp;
        if ((bitset && (regval & bitmask) != 0)
            || (!bitset && (regval & bitmask) == 0)) {
            return TRUE;
        }

        /*
         * Was a key pressed?
         */
        if (instreamp != NULL
            && (inchar = instreamp->read()) == '\03') {
            /*
             * We're aborting due to ^C.  Note that any other keypress
             *  will simply be eaten and thrown away.  Well, we're on
             *  the cheap.
             */
            break;
        }

        /*
         * Has the timeout elapsed?
         */
        if (timeout != 0) {
            curtime = micros();
            if ((curtime - start) >= timeout) {
                break;
            }
        }
    }

    /* We're aborting for one reason or another */
    return FALSE;
}

/*
 * Convert a hexadecimal string to binary.
 */
static unsigned int
atohex(const char *strp)
{
    unsigned int     retval;
    char             ch;

    if (*strp == '0' && (*(strp+1) == 'x'
                         || *(strp+1) == 'X')) {
        /* Skip leading 0x */
        strp += 2;
    }
    retval = 0;
    while (*strp != '\0') {
        ch = *strp;
        if (ch >= '0' && ch <= '9') {
            ch -= '0';
        }
        else if (ch >= 'a' && ch <= 'f') {
            ch -= 'a';
            ch += 10;
        }
        else if (ch >= 'A' && ch <= 'F') {
            ch -= 'A';
            ch += 10;
        }
        else {
            break;
        }
        retval <<= 4;
        retval |= ch;
        strp++;
    }

    return retval;
}

/*
 * Commands:
 */

static void
cmd_echo(cmdif_t *cmdifp, int argc, char **argv)
{
    int      idx;

    for (idx = 1; idx < argc; idx++) {
        cmdifp->outstreamp->print(argv[idx]);
        if ((idx+1) < argc) {
            cmdifp->outstreamp->print(' ');
        }
    }
    cmdifp->outstreamp->print(F("\r\n"));
}

/*
 * Peek a hex address and read the byte therein.
 */
static void
cmd_peek(cmdif_t *cmdifp, int argc, char **argv)
{
    uint32_t             readval;
    unsigned int         regaddr;
    Stream              *outstreamp;

    outstreamp = cmdifp->outstreamp;

    if (argc < 2) {
        outstreamp->print(F("Usage: peek address-in-hex\r\n"));
    }
    else {
        regaddr = atohex(argv[1]);
        hexprint32(outstreamp, regaddr, 8);
        outstreamp->print(F(": "));
        readval = *((volatile uint32_t *) regaddr);
        hexprint32(outstreamp, readval, 8);
        outstreamp->print(F("\r\n"));
    }
}

/*
 * Poke a value to a hex address
 */
static void
cmd_poke(cmdif_t *cmdifp, int argc, char **argv)
{
    unsigned int         regaddr, regval;
    Stream              *outstreamp;

    outstreamp = cmdifp->outstreamp;

    if (argc != 3) {
        outstreamp->print(
            F("Usage: poke address-in-hex value-in-hex\r\n"));
    }
    else {
        regaddr = atohex(argv[1]);
        regval = atohex(argv[2]);
        *((volatile uint32_t *) regaddr) = (uint32_t) regval;
        hexprint32(outstreamp, regaddr, 8);
        outstreamp->print(F(" <- "));
        hexprint32(outstreamp, regval, 8);
        outstreamp->print(F("\r\n"));
    }
}

/*
 * Print out the current date/time.
 */
static void
cmd_date(cmdif_t *cmdifp, int argc, char **argv)
{
    Stream              *outstreamp;
    time_t               nowtime, settime;
    int                  slen, val;
    const char          *errstr;
    char                *sp, *dsp, ntmp[5];
    DateTimeFields       now_tm, set_tm;

    outstreamp = cmdifp->outstreamp;
    errstr = NULL;

    nowtime = now();
    /* Start with the current time information as a default */
    breakTime(nowtime, set_tm);

    if (argc > 1) {
        slen = strlen(argv[1]);
        if (slen >= 8) {
            ntmp[2] = '\0';
            sp = argv[1];

            ntmp[0] = *sp++;
            ntmp[1] = *sp++;
            val = atoi(ntmp);
            if (val < 1 || val > 12) {
                errstr = PSTR("invalid month");
                goto usage;
            }
            set_tm.mon = val - 1;

            ntmp[0] = *sp++;
            ntmp[1] = *sp++;
            val = atoi(ntmp);
            if (val < 1 || val > 31) {
                errstr = PSTR("invalid date of month");
                goto usage;
            }
            set_tm.mday = val;

            ntmp[0] = *sp++;
            ntmp[1] = *sp++;
            val = atoi(ntmp);
            if (val < 0 || val > 23) {
                errstr = PSTR("invalid hour");
                goto usage;
            }
            set_tm.hour = val;

            ntmp[0] = *sp++;
            ntmp[1] = *sp++;
            val = atoi(ntmp);
            if (val < 0 || val > 59) {
                errstr = PSTR("invalid minute");
                goto usage;
            }
            set_tm.min = val;

            slen -= 8;
            if (slen > 0) {
                /* Remainder is [[CC]YY][.ss] */
                dsp = sp;
                /* Find the end or a '.', whichever comes first */
                while (*dsp != '\0' && *dsp != '.') {
                    dsp++;
                }
                if (dsp != sp) {
                    /* Must be one of [[CC]YY] */
                    if ((dsp - sp) == 2) {
                        /* Use the century part of the current year */
                        ntmp[0] = *sp++;
                        ntmp[1] = *sp++;
                        val = atoi(ntmp);

                        /* Borrow the century from the current year */
                        val += (set_tm.year - (set_tm.year%100));

                        if (val < 70 || val > 206) {
                            errstr = PSTR("2 digit year out of range");
                        }

                        set_tm.year = val;
                        slen -= 2;
                    }
                    else if ((dsp - sp) == 4) {
                        ntmp[0] = *sp++;
                        ntmp[1] = *sp++;
                        ntmp[2] = *sp++;
                        ntmp[3] = *sp++;
                        ntmp[4] = '\0';

                        val = atoi(ntmp);

                        if (val < 1970 || val > 2106) {
                            errstr = PSTR("4 digit year out of range");
                        }

                        set_tm.year = val - 1900;
                        slen -= 4;
                    }
                    else {
                        errstr = PSTR("wrong year length");
                        goto usage;
                    }
                }
                if (*sp == '.') {
                    sp++;
                    slen--;
                    if (slen != 2) {
                        errstr = PSTR("seconds must be 2 digits");
                        goto usage;
                    }
                    ntmp[0] = *sp++;
                    ntmp[1] = *sp++;
                    ntmp[2] = '\0';

                    val = atoi(ntmp);

                    if (val < 0 || val > 59) {
                        errstr = PSTR("invalid seconds");
                        goto usage;
                    }

                    set_tm.sec = val;
                }
            }

            set_tm.wday = 7;        /* invalid, on purpose */

            settime = makeTime(set_tm);
            Teensy3Clock.set(settime);
            setTime(settime);
        }
        else {
usage:
            if (errstr != NULL) {
                outstreamp->print(F("date: error: "));
                outstreamp->println(errstr);
            }

            outstreamp->println(F("Usage: date [MMDDhhmm[[CC]YY][.ss]]"));

            return;
        }
    }

    nowtime = now();
    breakTime(nowtime, now_tm);

    printTime(now_tm, outstreamp, TRUE, TRUE);
    outstreamp->println();
}

/* Reset the Pertec interface by toggling IFEN */
static void
cmd_reset(cmdif_t *cmdifp, int argc, char **argv)
{
    pertexus_state_t    *pstatep;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    UNUSED(argc);
    UNUSED(argv);

    ifreset(pstatep);
}

/* Print out statistics */
static void
cmd_stats(cmdif_t *cmdifp, int argc, char **argv)
{
    pertexus_state_t    *pstatep;
    Stream              *outstreamp;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);
    outstreamp = cmdifp->outstreamp;

    UNUSED(argv);

#if defined(OPT_INTCOUNTS)
    outstreamp->print(F("intcounts.icer = "));
    outstreamp->println(pstatep->intcounts.icer);

    outstreamp->print(F("intcounts.iher = "));
    outstreamp->println(pstatep->intcounts.iher);

    outstreamp->print(F("intcounts.iwstr = "));
    outstreamp->println(pstatep->intcounts.iwstr);

    outstreamp->print(F("intcounts.irstr = "));
    outstreamp->println(pstatep->intcounts.irstr);

    outstreamp->print(F("intcounts.idby = "));
    outstreamp->println(pstatep->intcounts.idby);

    outstreamp->print(F("intcounts.ifmk = "));
    outstreamp->println(pstatep->intcounts.ifmk);

    outstreamp->print(F("intcounts.ieot = "));
    outstreamp->println(pstatep->intcounts.ieot);
#if 0
    outstreamp->print(F("intcounts.portexp = "));
    outstreamp->println(pstatep->intcounts.portexp);
#endif
#else
    outstreamp->println(F("Interrupt statistics not being kept"));
#endif

    if (argc > 1) {
        outstreamp->println(F("Counts reset"));
#if defined(OPT_INTCOUNTS)
        memset(&(pstatep->intcounts), 0, sizeof(pstatep->intcounts));
#endif
    }
}

static void
cmd_fsf(cmdif_t *cmdifp, int argc, char **argv)
{
    bool_t               isfirst;
    unsigned int         nsfs;
    pertexus_state_t    *pstatep;
    Stream              *streamp;

    streamp = cmdifp->outstreamp;
    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    if (argc < 2) {
        nsfs = 1;
    }
    else {
        nsfs = atoi(argv[1]);
    }

    drivesel(pstatep);
    isfirst = TRUE;

    while (nsfs != 0) {
        pcmd(pstatep, PCMD_IWFM);
        if (isfirst) {
            /* Wait for IFBY to go active (low) */
            if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), FALSE,
                         streamp, 100)) {
                streamp->println(F("error: IFBY failed to assert"));
                goto fail;
            }
        }
        /* Wait for IDBY to go active (low) */
        if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), FALSE,
                     streamp, 5 * INVMICRO)) {
            streamp->println(F("error: IDBY failed to assert"));
            goto fail;
        }

        /* Wait for IDBY to go inactive (high) */
        if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), TRUE,
                     streamp,
                     ((MAXTAPELEN * INPERFT) / PLAYSPEED) * INVMICRO)) {
            streamp->println(F("error: IDBY failed to deassert"));
            goto fail;
        }

        isfirst = FALSE;
        /* !!! wait for completion */
        nsfs--;
    }
    /* Wait for IFBY to go inactive (high) */
    if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), TRUE,
                 streamp, 10000)) {
        streamp->println(F("error: IFBY failed to deassert"));
        goto fail;
    }

    drivedesel(pstatep);
    return;

fail:
    ifreset(pstatep);
    drivedesel(pstatep);
}


static void
cmd_testwrite(cmdif_t *cmdifp, int argc, char **argv)
{
    unsigned int     nrecsleft, nbytesleft, recsize, nfmk;
    uint32_t         regval;
    int              val;
    uint8_t          writeval;
    pertexus_state_t    *pstatep;
    Stream              *streamp;

    streamp = cmdifp->outstreamp;
    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    recsize = 2048;
    nrecsleft = 100;
    nfmk = 2;

    argc--;
    argv++;
    while (argc != 0) {
        if (**argv == '-') {
            if (strcmp_P(*argv, PSTR("-nrecs")) == 0) {
                if (argc < 2) {
                    streamp->println(
                        F("-nrecs must be followed by a decimal number"));
                    goto usage;
                }
                argc--;
                argv++;
                val = atoi(*argv);
                if (val < 0) {
                    streamp->println(F("invalid value after -nrecs"));
                    goto usage;
                }
                nrecsleft = val;
            }
            else if (strcmp_P(*argv, PSTR("-recsize")) == 0) {
                if (argc < 2) {
                    streamp->println(
                        F("-recsize must be followed by a decimal number"));
                    goto usage;
                }
                argc--;
                argv++;
                val = atoi(*argv);
                if (val <= 0) {
                    streamp->println(F("invalid value after -recsize"));
                    goto usage;
                }
                recsize = val;
            }
            else if (strcmp_P(*argv, PSTR("-nfmk")) == 0) {
                if (argc < 2) {
                    streamp->println(
                        F("-nfmk must be followed by a decimal number"));
                    goto usage;
                }
                argc--;
                argv++;
                val = atoi(*argv);
                if (val < 0) {
                    streamp->println(F("invalid value after -nfmk"));
                    goto usage;
                }
                recsize = val;
            }
            else {
                streamp->print(F("invalid flag "));
                streamp->println(*argv);
                goto usage;
            }
        }
        else {
            streamp->print(F("unknown argument "));
            streamp->println(*argv);
usage:
            streamp->println(
                F("usage: testwrite [-nrecs n] [-recsize s] [-nfmk f]"));
            return;
        }

        argc--;
        argv++;
    }

    drivesel(pstatep);

    /*
     * Check to see if writing is disabled.
     */
    if (GP_READ(pstatep, GP_IFPT) == 0) {
        streamp->println(F("Error: tape is write protected"));
        goto fail1;
    }

    interrupts_set(pstatep, INTCFG_ALLOFF);

    /* Turn off interrupts and go to town. */
    while (nrecsleft != 0) {
        writeval = (nrecsleft-1);   /* don't start with 0 all the time; it's
                                     *  boring */
        nbytesleft = recsize;

        /* Start the writing */
        pcmd(pstatep, PCMD_IWRT);

        if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), FALSE,
                     streamp, 1000)) {
            streamp->println(F("Error: IFBY failed to assert"));
            goto fail2;
        }

        if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), FALSE,
                     streamp, 2UL*1000UL*1000UL)) {
            streamp->println(F("Error: IDBY failed to assert"));
            goto fail3;
        }

        noInterrupts();

        while (nbytesleft > 1) {
            /* Load out the next byte */
            regval = *GP_DR_REG(GP_IW7);
            regval &= ~(0xff << GP_BITNUM(GP_IW7));
            regval |= ~writeval << GP_BITNUM(GP_IW7);
                                        /* Inverted because that's
                                         *  the way the lines are
                                         *  specified. */
            *GP_DR_REG(GP_IW7) = regval;

            writeval--;
            /* Wait to see a low-to-high transition of the strobe */
            while (GP_READ(pstatep, GP_IWSTR) != 0) {
                ;
            }

            while (GP_READ(pstatep, GP_IWSTR) == 0) {
                ;
            }
            nbytesleft--;
        }

        /* Write the final byte and set the ILWD bit */

        *GP_DR_CLEAR_REG(GP_ILWD) = GP_MASK(GP_ILWD);
        regval = *GP_DR_REG(GP_IW7);
        regval &= ~(0xff << GP_BITNUM(GP_IW7));
        regval |= ~writeval << GP_BITNUM(GP_IW7);
        *GP_DR_REG(GP_IW7) = regval;

        /* Wait to see a low-to-high transition of the strobe */
        while (GP_READ(pstatep, GP_IWSTR) != 0) {
            ;
        }

        while (GP_READ(pstatep, GP_IWSTR) == 0) {
            ;
        }


        /* !!! ??? wait to finish */

        nrecsleft--;

        interrupts();

        /* Clear the ILWD bit */
        *GP_DR_SET_REG(GP_ILWD) = GP_MASK(GP_ILWD);
    }

    /* Now write the filemarks */

    while (nfmk != 0) {
        /* First, wait for IDBY to go inactive (high) */
        if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), TRUE,
                     streamp, 2UL*1000UL*1000UL)) {
            streamp->println(F("Error: IDBY failed to deassert"));
            goto fail3;
        }

        pcmd(pstatep, PCMD_IWRT | PCMD_IWFM);

        /* Next, wait for IDBY to go active (high) */
        if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), FALSE,
                     streamp, 2UL*1000UL*1000UL)) {
            streamp->println(F("Error: IDBY failed to assert"));
            goto fail3;
        }

        nfmk--;
    }


    /* And re-activate interrupts */
    drivedesel(pstatep);
    return;

fail3:      /* IDBY failed to assert within a certain amount of time */
    ;

fail2:      /* IFBY failed to assert within a certain amount of time */
    ;

fail1:      /* FPT is asserted */
    drivedesel(pstatep);
}

/*
 * Write a tape image file to tape.
 */
static void
cmd_write(cmdif_t *cmdifp, int argc, char **argv)
{
    File             readfile;
    uint32_t         header, footer;
    recsize_t        recleft;
    bool_t           filedone;
#if 0
    uint8_t          timsk0_saved;
#endif
    uint8_t          pattern, patwriteval, postgap, wrrec;
    uint8_t         *wrptr;
    uint16_t         spaceavail, readsize, patrecsleft;
    int              inchar;
    tape_rec_t      *trp;
    Stream          *streamp;
    pertexus_state_t    *pstatep;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    streamp = cmdifp->outstreamp;

    if (argc < 2) {
        streamp->println(F("usage: write tape-image-file"));
        return;
    }

    if (strcmp_P(argv[1], PSTR("-pattern")) == 0) {
        pattern = TRUE;
        patrecsleft = 100;
    }
    else {
        pattern = FALSE;
        if (!pstatep->sdcardinit) {
            if (!sdcard_init(pstatep, streamp)) {
                streamp->println(F("SD card initialization failed."));
                return;
            }
        }

        readfile = pstatep->sdp->open(argv[1], FILE_READ);

        if (!readfile) {
            streamp->println(F("unable to open file"));
            return;
        }
    }

    drivesel(pstatep);

    /*
     * Check to see if writing is disabled.
     */
    if (GP_READ(pstatep, GP_IFPT) == 0) {
        drivedesel(pstatep);
        streamp->println(F("Error: tape is write protected"));
        readfile.close();
        return;
    }

    /* Initialize our state */
    pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;
    pstatep->rdptr = pstatep->wrptr = &(pstatep->buf[0]);
    pstatep->rdrec = pstatep->wrrec = 0;
    pstatep->filesdone = pstatep->recsdone = 0;
    recleft = 0;        /* avoid compiler warning */
    patwriteval = 0;    /* avoid compiler warning */
    filedone = FALSE;
    trp = NULL;

    interrupts_set(pstatep, INTCFG_WRITING);
    /* turn off timer interrupts */
#if 0
    timsk0_saved = TIMSK0;
    TIMSK0 = 0;
#endif

    /*                    updated by
     *              this func     interrupt
     * data ring    wrptr%        rdptr*
     * record ring  wrrec         rdrec
     *
     * * = must be read with ATOMIC_READ because this is a
     *     multi-byte variable.
     * % = must be written with ATOMIC_WRITE because this is
     *     a multi-byte variable.
     */


    while (pstatep->opstate != PERTEXUS_STATE_OPSTATE_ERROR
           && (!filedone || pstatep->opstate != PERTEXUS_STATE_OPSTATE_IDLE)) {
        /*
         * Are we not in a record and there's space for a new
         *  record?
         */
        if (!filedone && trp == NULL) {
            /* rdrec is safe to read without interrupt protection because
             *  it is a single byte */
            ASSERT_CT(sizeof(pstatep->rdrec) == 1);

            wrrec = pstatep->wrrec;
            if (!RING_ISFULL(wrrec, pstatep->rdrec, TAPENRECS)) {
                if (pattern == 0) {
                    /*
                     * Read in the header.
                     */
                    if (readfile.read(&header, sizeof(uint32_t))
                                                    != sizeof(uint32_t)) {
                        /* We're at the end of file. */
                        filedone = TRUE;
                    }
                    else {
                        /* We're now in a record. */
                        /* streamp->print(F("Starting to load record, wrrec = ")); */
                        /* streamp->print(pstatep->wrrec, DEC); */
                        /* streamp->print(F(", rdrec = ")); */
                        /* streamp->print(pstatep->rdrec, DEC); */
                        /* streamp->print(F(", recsavail = ")); */
                        /* streamp->print(recsavail, DEC); */
                        /* streamp->print(F(", header = 0x")); */
                        /* streamp->print(header, HEX); */
                        /* streamp->println(); */

                        trp = &(pstatep->taperecs[wrrec]);
                        trp->len = recleft = (header & 0x00ffffffUL);

                        if (header == 0) {
                            /* It's a file mark, so we're done with this */
                            trp = NULL;
                        }
                    }
                }
                else {
                    /* We're generating from a pattern */
                    trp = &(pstatep->taperecs[wrrec]);
                    if (patrecsleft == 0) {
                        /* Write a filemark */
                        trp->len = recleft = 0;
                        filedone = TRUE;
                        trp = NULL;
                    }
                    else {
                        /* Set up to write a pattern */
                        patrecsleft--;
                        patwriteval = patrecsleft;
                        trp->len = recleft = 2048;
                    }
                }

                /*
                 * Tricky business here.  Even though we are not
                 *  done with this record (we don't even have
                 *  any data on it, if there are going to be
                 *  data) we advance the wrrec here so that
                 *  the interrupt FSM can know that a record is
                 *  available.
                 * Note that as soon as we do this we cannot change
                 *  anything in the record structure.
                 */
                NONATOMIC_INCR1_MOD(wrrec, TAPENRECS);
                /* No need to use "ATOMIC_WRITE()" because this is a
                 *  single byte */
                pstatep->wrrec = wrrec;

                /*
                 * We want to keep loading data until we
                 *  run out of space before we actually start
                 *  things up, so any action we take that
                 *  consumes more space should be followed by
                 *  an attempt to consume more.
                 */
                if (trp == NULL) {
                    continue;
                }
            }
        }

        /*
         * Do we have space for data?
         */

        if (!filedone && trp != NULL) {
            uint8_t             *rdptr;

            rdptr = ATOMIC_READ(pstatep->rdptr);
            wrptr = pstatep->wrptr;
            spaceavail = RINGPTR_FREE(wrptr, rdptr, TAPEBUFSIZ);

            if (spaceavail != 0) {
                /* We're in a record and we have some space */
                readsize = (uint16_t) MIN(recleft, (recsize_t) spaceavail);
                readsize = MIN(readsize,
                               RINGPTR_TOEND(wrptr, &(pstatep->buf[0]),
                                             TAPEBUFSIZ));

                /* streamp->print(F("Reading ")); */
                /* streamp->print(readsize, DEC); */
                /* streamp->println(F(" bytes of record data")); */
                if (pattern == 0) {
                    /*
                     * Cap the readsize so as to get us back on
                     *  block alignment.  Reading fewer data than
                     *  we can also turns the data around more
                     *  quickly so we're less likely to underrun.
                     */
                    readsize = MIN(readsize,
                                   (~((uint16_t) readfile.position())
                                    & (FILEBLKSIZ-1)) + 1);
                    (void) readfile.read(wrptr, readsize);
                    /* !!! error handling if read fails */
                    wrptr += readsize;
                }
                else {
                    uint16_t         idx;

                    idx = readsize;
                    while (idx-- != 0) {
                        *wrptr++ = patwriteval--;
                    }
                }

                RINGPTR_WRAP(wrptr, &(pstatep->buf[0]), TAPEBUFSIZ);
                ATOMIC_WRITE(pstatep->wrptr, wrptr);
                recleft -= readsize;

                if (recleft == 0) {
                    /* Skip over the record's trailer */
                    if (pattern == 0) {
                        /*
                         * Remember to skip some bytes before
                         *  reading the footer (so as to
                         *  become aligned again).
                         */
                        postgap = -((uint8_t) trp->len) & 1;
                        if (postgap != 0) {
                            (void) readfile.seek(postgap, SeekCur);
                        }
                        (void) readfile.read(&footer, sizeof(uint32_t));
                    }

                    trp = NULL;
                }

                /*
                 * We want to keep loading data until we
                 *  run out of space before we actually start
                 *  things up, so any action we take that
                 *  consumes more space should be followed by
                 *  an attempt to consume more.
                 */
                continue;
            }
        }


        /*
         * Are we at idle?
         */
        if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_IDLE) {
            streamp->println(F("Starting"));
            /* Do we have anything to start with? */
            writerecstart(pstatep);
        }

        if ((inchar = streamp->read()) == '\03') {
            bool_t      mustwait;

            /*
             * We're aborting due to ^C.  Note that any other keypress
             *  will simply be eaten and thrown away.  Well, we're on
             *  the cheap.
             */
            mustwait = FALSE;
            noInterrupts();
            /* Force everything to an end */
            switch (pstatep->opstate) {
            case PERTEXUS_STATE_OPSTATE_IDLE:
            case PERTEXUS_STATE_OPSTATE_ERROR:
                break;

            case PERTEXUS_STATE_OPSTATE_WINREC:
            case PERTEXUS_STATE_OPSTATE_WRECSTART:
            case PERTEXUS_STATE_OPSTATE_WRECDONE:
                /* We've issued an IWRT, and we were waiting for
                 *  IDBY.  So force the record to end and move
                 *  to WRECDONE, and then write a tape mark.  */
                GP_WRITE(pstatep, GP_ILWD, 0);
                pstatep->opstate = PERTEXUS_STATE_OPSTATE_WRECDONE;
                pstatep->rdrec = TAPENRECS-1;
                pstatep->taperecs[0].len = 0;       /* write a tapemark */
                pstatep->wrrec = 1;
                mustwait = TRUE;
                break;

            case PERTEXUS_STATE_OPSTATE_ERRWAIT:
            case PERTEXUS_STATE_OPSTATE_WFMWAIT:
                pstatep->rdrec = pstatep->wrrec = 0;
                mustwait = TRUE;
                break;
            }

            interrupts();

            if (mustwait) {
                while (pstatep->opstate != PERTEXUS_STATE_OPSTATE_IDLE
                       && pstatep->opstate != PERTEXUS_STATE_OPSTATE_ERROR) {
                    ;
                }
            }
            break;
        }
    }

    drivedesel(pstatep);

    if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_ERROR) {
        streamp->println(F("Write ended in error (underrun)"));
        streamp->print(F(" recsdone = "));
        streamp->print(pstatep->recsdone, DEC);
        streamp->print(F(", filesdone = "));
        streamp->println(pstatep->filesdone, DEC);
    }

    readfile.close();

#if 0
    /* turn timer interrupts back on again */
    TIMSK0 = timsk0_saved;
#endif
}


/*
 * Read a tape into a tape image file.
 */
static void
cmd_read(cmdif_t *cmdifp, int argc, char **argv)
{
    File            *writefile;
    File             tmpfile, outfile;
    recsize_t        recsofar, reclen, prevreclen;
    uint32_t         zeroword;
    uint32_t         header, lastfooteroff, nerrs, nbytes;
#if 0
    uint32_t         prealloc;
#endif
    uint8_t          rdrec;
#if 0
    uint8_t          timsk0_saved;
#endif
    uint8_t         *rdptr, *wrptr;
    uint16_t         writesize, lastwritesize;
    int              val;
    bool_t           seenend, rev;
    int              inchar;
    tape_rec_t      *trp;
    Stream          *streamp;
    bool_t           nofile;
    pertexus_state_t    *pstatep;
    char             tmpfn[TMPFNLEN+1];
    enum {
        FILECOUNT,
        ABORT,
        OVERRUN,
        EOT,
        BOT,
    } endwhy;
    const char      *outfn;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);
    streamp = cmdifp->outstreamp;

#if 0
    prealloc = 60UL*1024UL*1024UL;      /* preallocate 60 MiB */
#endif

    argc--;
    argv++;
    outfn = NULL;
    nofile = FALSE;
    seenend = FALSE;
    rev = FALSE;

    while (argc != 0) {
        if (strcmp_P(*argv, PSTR("-")) == 0) {
            if (outfn != NULL || nofile) {
errfn:
                streamp->println(
                    F("only one instance of tape-image-file or '-' allowed"));
                goto usage;
            }
            nofile = TRUE;
        }
        else if (**argv == '-') {
            if (strcmp_P(*argv, PSTR("-eot")) == 0) {
                if (seenend) {
errend:
                    streamp->println(
                        F("only one instance of -eot or -nfiles n allowed"));
                    goto usage;
                }
                seenend = TRUE;
                pstatep->filesleft = -1;
            }
            else if (strcmp_P(*argv, PSTR("-nfiles")) == 0) {
                if (seenend) {
                    goto errend;
                }
                if (argc < 2) {
                    streamp->println(
                        F("-nfiles must be followed by a file count"));
                    goto usage;
                }
                argc--;
                argv++;
                val = atoi(*argv);
                if (val < 1) {
                    streamp->println(
                        F("-nfiles must be followed by a positive integer"));
                    goto usage;
                }
                pstatep->filesleft = val;
                seenend = TRUE;
            }
            else if (strcmp_P(*argv, PSTR("-rev")) == 0) {
                rev = TRUE;
            }
            else {
                streamp->print(F("unknown flag "));
                streamp->println(*argv);
                goto usage;
            }
        }
        else {
            if (outfn != NULL || nofile) {
                goto errfn;
            }
            outfn = *argv;
        }
        argc--;
        argv++;
    }


    if (outfn == NULL && !nofile) {
usage:
        streamp->println(
            F("usage: read [-rev] [-eot|-nfiles n] [-|tape-image-file]"));
        return;
    }

    /* Default to double EOF terminating our read */
    if (!seenend) {
        if (rev) {
            /* By default we read to the beginning of tape, unless
             *  specified otherwise (reading backwards is confusing --
             *  the file mark comes before the data).
             */
            pstatep->filesleft = -1;    /* Read to beginning of tape */
        }
        else {
            pstatep->filesleft = -2;        /* Magic value to indicate
                                             *  we're looking for two
                                             *  consecutive EOFs with
                                             *  no intervening data */
        }
    }

    if (!nofile) {
        if (!pstatep->sdcardinit) {
            if (!sdcard_init(pstatep, streamp)) {
                streamp->println(F("SD card initialization failed."));
                return;
            }
        }

        outfile = pstatep->sdp->open(outfn, FILE_WRITE);

        if (!outfile) {
            streamp->println(F("unable to open file"));
            return;
        }

        if (rev) {
            /* For a reverse read we read first into a temporary
             *  file. */
            if (!tmpfn_gen(pstatep->sdp, &(tmpfn[0]))) {
                streamp->println(F("unable to generate temporary file name"));
                return;
            }
            tmpfile = pstatep->sdp->open(tmpfn, FILE_WRITE);
            if (!tmpfile) {
                streamp->println(F("unable to create temporary file"));
                outfile.close();
                return;
            }
            writefile = &tmpfile;
        }
        else {
            writefile = &outfile;
        }
    }
    else {
        writefile = NULL;
    }

    drivesel(pstatep);

    /* Initialize our state */
    pstatep->opstate = PERTEXUS_STATE_OPSTATE_RPAUSED;
    pstatep->rdptr = pstatep->wrptr = &(pstatep->buf[0]);
    pstatep->rdrec = pstatep->wrrec = 0;
    pstatep->filesdone = pstatep->recsdone = 0;
    pstatep->startcmd = rev ? PCMD_IREV : 0;
    recsofar = 0;
    lastfooteroff = 0;  /* offset past last valid footer */
    zeroword = 0;
    writesize = lastwritesize = 0;      /* avoid compiler warning */
    reclen = 0;     /* eliminate compiler warning */
    prevreclen = 0;

    trp = NULL;

    /* Clear any pending port expander interrupt */
    (void) portexp_read(pstatep, -1);
    interrupts_set(pstatep, INTCFG_READING);
#if 0
    /* turn off timer interrupts */
    timsk0_saved = TIMSK0;
    TIMSK0 = 0;
#endif

    nbytes = nerrs = 0;

    /*
     * By default we finish by going to IDLE in readrecfinish() when
     *  we reach our file count or see 2 consecutive file marks.
     */
    endwhy = FILECOUNT;

    /*                    updated by
     *              this func     interrupt
     * data ring    rdptr%        wrptr*
     * record ring  rdrec         wrrec
     *
     * * = must be read with ATOMIC_READ because this is a
     *     multi-byte variable.
     * % = must be written with ATOMIC_WRITE because this is
     *     a multi-byte variable.
     */

    while (pstatep->opstate != PERTEXUS_STATE_OPSTATE_IDLE
           || !RING_ISEMPTY(pstatep->wrrec, pstatep->rdrec, TAPENREC)) {

        /*
         * Have we accumulated data that we need to write
         *  to the SD card?
         */
        /* We read these values so the compiler doesn't keep
         *  re-fetching values that don't change. */
        wrptr = ATOMIC_READ(pstatep->wrptr);
        rdptr = pstatep->rdptr;
        rdrec = pstatep->rdrec;
        if (!RINGPTR_ISEMPTY(wrptr, rdptr, TAPEBUFSIZE)
            || !RING_ISEMPTY(pstatep->wrrec, rdrec, TAPENREC)) {

            lastwritesize = writesize;

            /*
             * Either the data ring is not empty or the
             *  record ring is not empty.
             */
            if (RING_ISEMPTY(pstatep->wrrec, rdrec, TAPENREC)) {
                /*
                 * The record ring is empty.  This must mean
                 *  we've started receiving data for a record
                 *  but we haven't finished it.
                 */
                trp = NULL;
                writesize = RINGPTR_USED(wrptr, rdptr, TAPEBUFSIZ);
            }
            else {
                /*
                 * We have a finished record (or file mark) in
                 *  the queue.  Note that the length of this
                 *  record is guaranteed to be less than or
                 *  equal to the size of the data in the ring
                 *  buffer.
                 */
                trp = &(pstatep->taperecs[rdrec]);
                reclen = (recsize_t) (trp->len & 0x00ffffff);
                writesize = (uint16_t) (reclen - recsofar);
                /* Also: writesize *may be 0* if this is a filemark */
            }

            /*
             * Now limit the write size so we don't go off
             *  the end of ring buffer.  If there's more
             *  data we'll get it on the next go around.
             */
            writesize = MIN(writesize,
                            RINGPTR_TOEND(rdptr, &(pstatep->buf[0]),
                                          TAPEBUFSIZ));

            if (recsofar == 0) {
                /*
                 * We're starting a record.  Write a dummy
                 *  header and we'll come back and
                 *  fill this in afterwards.
                 */

                if (writefile != NULL
                    && writefile->write(&zeroword, sizeof(uint32_t))
                       != sizeof(uint32_t)) {
                    goto writefail;
                }
            }

            if (writesize > 0) {
                if (writefile != NULL) {
                    /*
                     * Limit the write size to whatever would get us
                     *  back to block alignment.  Because the SD card
                     *  code is working in blocks anyway this is the
                     *  fastest way to make the buffer space available
                     *  again, and it also minimizes the amount of
                     *  data copying the SD card code does.
                     */
                    writesize = MIN(writesize,
                                    (~((uint16_t) writefile->position())
                                     & (FILEBLKSIZ-1)) + 1);
                    if (writefile->write(rdptr, writesize) != writesize) {
                        goto writefail;
                    }
                }

                recsofar += writesize;
            }

            /* Advance the read offset */
            rdptr += writesize;
            RINGPTR_WRAP(rdptr, &(pstatep->buf[0]), TAPEBUFSIZ);
            ATOMIC_WRITE(pstatep->rdptr, rdptr);

            /* Did we finish this particular record? */
            if (trp != NULL && recsofar == reclen) {
                /* Yes, we did.  Write the footer, if one is needed. */
                if (reclen != 0) {
                    uint8_t      postgap;

                    if (writefile != NULL) {
                        /*
                         * First write dummy data to skip ahead to
                         *  an aligned (2-byte aligned) offset.  Note that
                         *  it is only ever written immediately after
                         *  record data of length n*2+1.
                         */
                        postgap = -((uint8_t) reclen) & 1;
                        if (postgap != 0) {
                            if (writefile->write(&zeroword,
                                                postgap) != postgap) {
                                goto writefail;
                            }
                        }

                        /*
                         * Construct the header with the record length
                         *  and the flags glued together.
                         */
                        header = ((trp->len & 0x00ffffff)
                                  | (trp->flags << 24));

                        if (writefile->write(&header, sizeof(uint32_t))
                            != sizeof(uint32_t)) {
writefail:
                            streamp->println(F("Failure when writing."));

                            break;
                        }
                    }
                    if ((trp->flags & TAPE_REC_FLAGS_ERR) != 0) {
                        nerrs++;
                    }

                    nbytes += reclen;
                    pstatep->recsdone++;
                }
                else {
                    pstatep->filesdone++;
                }

                /*
                 * Note that if reclen == 0 then we just wrote
                 *  a tapemark, and so the last thing we wrote
                 *  was a header with 0 in it.  This serves as
                 *  its own footer.
                 */
                if (writefile != NULL) {
                    lastfooteroff = writefile->position();
                }

                /* Advance the ring buffer */
                NONATOMIC_INCR1_MOD(rdrec, TAPENRECS);
                pstatep->rdrec = rdrec;
                prevreclen = reclen;
                recsofar = 0;
            }
        }

        if ((inchar = streamp->read()) == '\03') {
            /*
             * We're aborting due to ^C.  Note that any other keypress
             *  will simply be eaten and thrown away.  Well, we're on
             *  the cheap.
             */
            uint8_t      oldopstate;

            oldopstate = ATOMIC_SWAP(pstatep->opstate,
                                     PERTEXUS_STATE_OPSTATE_IDLE);

            if (oldopstate != PERTEXUS_STATE_OPSTATE_IDLE
                && oldopstate != PERTEXUS_STATE_OPSTATE_ROVERRUN) {
                endwhy = ABORT;
                ifreset(pstatep);
            }
        }


        /*
         * Have we overrun?  If so then we need to abort.
         */
        if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_ROVERRUN) {
            endwhy = OVERRUN;
            /*
             * Note that we don't complete the record
             *  we've been receiving.  That's okay and
             *  we'll fix it by truncating the file, below.
             */
            ifreset(pstatep);
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;

            streamp->println(F("Terminating due to overrun"));
            debug_dumpstate(pstatep, streamp);
            streamp->print(F("writesize = "));
            streamp->print(writesize, DEC);
            streamp->print(F(", lastwritesize = "));
            streamp->println(lastwritesize, DEC);
            streamp->print(F("lastfooteroff = "));
            streamp->print(lastfooteroff, DEC);
            streamp->print(F(", position = "));
            streamp->println(writefile->position(), DEC);
        }
        else if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_EOT) {
            endwhy = EOT;
            /*
             * We've seen the end of the tape.
             *  Immediately stop reading.
             */
            streamp->println(F("Terminating due to end of tape"));
            ifreset(pstatep);
            pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;
        }
        else if ((pstatep->opstate == PERTEXUS_STATE_OPSTATE_RSTART
                  || pstatep->opstate == PERTEXUS_STATE_OPSTATE_RINREC)
                 && GP_READ(pstatep, GP_LPI2C4_INT) == 0) {
            /*
             * Were we reversing and saw a port expander interrupt?
             *   It could be we've bumped into the BOT.
             */
            unsigned int         regval;

            /* This asserts that the ILDP signal is on the port expander */
            ASSERT_CT((GP_GROUP(GP_ILDP) & 1) != 0);

            /*
             * Read all of the port expander bits.  We do this to
             *  guarantee that the interrupt line gets cleared.
             */
            regval = portexp_read(pstatep, -1);

            /*
             * Are we reading in reverse and the BOT is on?
             */
            if (rev && (regval & (1 << (GP_SETNUM(GP_ILDP) * 8
                                        + GP_BITNUM(GP_ILDP)))) == 0) {
                streamp->println(F("Terminating due to beginning of tape"));
                /* !!! May not need to do this reset: */
                ifreset(pstatep);
                pstatep->opstate = PERTEXUS_STATE_OPSTATE_IDLE;
                endwhy = BOT;
            }
        }

        /*
         * Now that we've made some space we can attempt to
         *  start up reception, if it's not running.
         *
         *  NOTE: it's safe to read wrptr and wrrec because we're
         *  in the RPAUSED state, during which the interrupt handlers
         *  will not change anything.
         */
        if (pstatep->opstate == PERTEXUS_STATE_OPSTATE_RPAUSED
            && RING_ISEMPTY(pstatep->wrrec,
                            pstatep->rdrec, TAPENRECS)
            && RINGPTR_ISEMPTY(pstatep->wrptr,
                               pstatep->rdptr, TAPEBUFSIZ)) {

            readrecstart(pstatep);
        }
    }

    interrupts_set(pstatep, INTCFG_IDLE);

    drivedesel(pstatep);

    /*
     * Finalize the file.  This involves truncating it
     *  to just the valid data (if we ended in an overrun)
     *  and going back and rewriting the record
     *  headers (which are all 0s right now).
     */
    if (writefile != NULL) {  /* && lastfooteroff >= sizeof(uint32_t)) { */
        unsigned int         trashsize, remnants;

        /*
         * First: truncate the file so that it extends only
         *  up to the last properly written footer.
         */
        writefile->flush();
        trashsize = writefile->size() - lastfooteroff;
        if (trashsize > 0) {
            streamp->print(F("Removing final "));
            streamp->print(trashsize, DEC);
            streamp->print(F(" bytes of file ("));

            remnants = writefile->position() - lastfooteroff;
            if (remnants > 0) {
                streamp->print(remnants, DEC);
                streamp->print(F(" bytes of partial final record"));
            }
            if (trashsize > remnants) {
                if (remnants > 0) {
                    streamp->print(F(", "));
                }
                streamp->print(trashsize - remnants, DEC);
                streamp->print(F(" bytes of old file data"));
            }
            streamp->println(F(")"));
            writefile->truncate(lastfooteroff);
        }

        /*
         * Now go backward through the file, copying the
         *  footers to headers.
         *  lastfooteroff normally points just past the
         *  footer.
         */
        if (rev) {
            /*
             * If we ended because we reached the beginning of
             *  the tape then we may have recorded a phantom
             *  0-byte record.  So we'll remove it if that
             *  happened.
             */
            if (endwhy == BOT
                && lastfooteroff >= sizeof(uint32_t)
                && prevreclen == 0) {
                lastfooteroff -= sizeof(uint32_t);
            }

            (void) tapfilerev(pstatep, streamp, &outfile,
                              &tmpfile, lastfooteroff);
            outfile.close();

            /* Close and delete the temporary file. */
            tmpfile.close();
            (void) pstatep->sdp->remove(tmpfn);
        }
        else {
            (void) tapfilefinalize(pstatep, streamp, &outfile, lastfooteroff);
            outfile.close();
        }
    }

#if 0
    /* turn timer interrupts back on again */
    TIMSK0 = timsk0_saved;
#endif

    streamp->print(F("Total of "));
    streamp->print(nbytes, DEC);
    streamp->print(F(" bytes in "));
    streamp->print(pstatep->recsdone, DEC);
    streamp->print(F(" records ("));
    streamp->print(nerrs, DEC);
    streamp->print(F(" records with errors) in "));
    streamp->print(pstatep->filesdone, DEC);
    streamp->println(F(" files"));
}

static void
cmd_nop(cmdif_t *cmdifp, int argc, char **argv)
{
    pertexus_state_t    *pstatep;

    UNUSED(argc);
    UNUSED(argv);
    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    drivesel(pstatep);

    if (GP_READ(pstatep, GP_IFPT) == 0) {
        cmdifp->outstreamp->println(F("Error: tape is write protected"));
    }
    else {
        pcmd(pstatep, PCMD_IERASE | PCMD_IEDIT);

        /* Wait for IFBY to go active (low) */
        if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), FALSE,
                     cmdifp->outstreamp, 100))  {
            cmdifp->outstreamp->println(
                F("error: failed to see IFBY be asserted within 100 microsec"));
            ifreset(pstatep);
        }
        else {
            /* Wait for IBY to go inactive (high) */
            if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), TRUE,
                         cmdifp->outstreamp, 300)) {
                cmdifp->outstreamp->println(
            F("error: failed to see IFBY be deasserted within 3100 microsec"));
                ifreset(pstatep);
            }
        }
    }

    drivedesel(pstatep);
}

/* Rewind the tape */
static void
cmd_rewind(cmdif_t *cmdifp, int argc, char **argv)
{
    pertexus_state_t    *pstatep;

    UNUSED(argc);
    UNUSED(argv);
    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    drivesel(pstatep);
    GP_WRITE(pstatep, GP_IREW, 0);      /* Make active */
    delayMicroseconds(5);
    GP_WRITE(pstatep, GP_IREW, 1);

    /* Now wait for IRDY to become asserted (go low) */
    delayMicroseconds(5);
    if (!bitwait(GP_DR_REG(GP_IRDY), GP_MASK(GP_IRDY), FALSE,
                 cmdifp->outstreamp,
                 ((MAXTAPELEN * INPERFT)/REWINDSPEED) * INVMICRO)) {
        ifreset(pstatep);
    }
    drivedesel(pstatep);
}

static void
cmd_unload(cmdif_t *cmdifp, int argc, char **argv)
{
    pertexus_state_t    *pstatep;

    UNUSED(argc);
    UNUSED(argv);
    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    drivesel(pstatep);
    GP_WRITE(pstatep, GP_IOFL, 0);      /* Make active */
    delayMicroseconds(5);
    GP_WRITE(pstatep, GP_IOFL, 1);
    drivedesel(pstatep);
}

static void
cmd_density(cmdif_t *cmdifp, int argc, char **argv)
{
    int     density;
    pertexus_state_t    *pstatep;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    if (argc < 2) {
usage:
        cmdifp->outstreamp->print(F("Usage: density 1600|3200\r\n"));
    }
    else {
        density = atoi(argv[1]);
        if (density != 1600 && density != 3200) {
            goto usage;
        }

        drivesel(pstatep);

        if (density == 1600) {
            pcmd(pstatep, PCMD_IERASE | PCMD_IWFM | PCMD_IEDIT);
        }
        else {
            pcmd(pstatep, PCMD_IREV | PCMD_IERASE | PCMD_IWFM | PCMD_IEDIT);
        }

        /* Wait for IFBY to go active (low) */
        if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), FALSE,
                     cmdifp->outstreamp, 100)
            /* Wait for IBY to go inactive (high) */
            || !bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), TRUE,
                        cmdifp->outstreamp, 300)) {
            ifreset(pstatep);
        }

        drivedesel(pstatep);
    }
}

static void
cmd_testread(cmdif_t *cmdifp, int argc, char **argv)
{
    uint8_t             *bufp;
    uint32_t             regval;
    unsigned int         wroff, nrecsleft, maxreclen, minreclen, recsread;
    unsigned int         filesread;
    unsigned int         nfilesleft;
    int                  idx;
    unsigned int         reclen;
    unsigned long        totalbytes;
    Stream              *streamp;
    bool_t               allrecs, allfiles, lasthigh, reverse;
    volatile uint32_t   *stroberegp, *busyregp, *datregp, *ifmkregp;
    pertexus_state_t    *pstatep;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    allrecs = TRUE;
    allfiles = FALSE;
    reverse = FALSE;
    nfilesleft = 1;
    nrecsleft = 0;      /* to keep compiler from whining */
    streamp = cmdifp->outstreamp;

    for (idx = 1; idx < argc; idx++) {
        if (*argv[idx] == '-') {
            if (strcmp_P(argv[idx], PSTR("-nrecs")) == 0) {
                if ((argc - idx) < 2) {
                    streamp->println(F("-nrecs must be followed by a value"));
                    goto usage;
                }
                nrecsleft = atoi(argv[idx+1]);
                if (nrecsleft == 0) {
                    allrecs = TRUE;
                }
                idx++;
            }
            else if (strcmp_P(argv[idx], PSTR("-nfiles")) == 0) {
                if ((argc - idx) < 2) {
                    streamp->println(F("-nfiles must be followed by a value"));
                    goto usage;
                }
                nfilesleft = atoi(argv[idx+1]);
                if (nfilesleft == 0) {
                    allfiles = TRUE;
                }
                idx++;
            }
            else if (strcmp_P(argv[idx], PSTR("-reverse")) == 0) {
                reverse = TRUE;
            }
            else {
                streamp->print(F("Unknown flag "));
                streamp->println(argv[idx]);
                goto usage;
            }
        }
        else {
            streamp->print(F("Unknown argument "));
            streamp->println(argv[idx]);
usage:
            streamp->println(F(
"Usage: testread [-nrecs nrecs] [-nfiles nfiles] [-reverse]\r\n"
"       where nrecs is the number of records\r\n"
"       per file to read (0 = unlimited, default = 0))\r\n"
"       and nfiles is the number of files\r\n"
"       to read (0 = unlimited, default = 1)\r\n"
"       -reverse tells the drive to read in reverse\r\n"));
            return;
        }
    }


    drivesel(pstatep);

    filesread = 0;
    recsread = 0;
    totalbytes = 0;
    maxreclen = 0;
    minreclen = 0;

    bufp = &(pstatep->buf[0]);
    stroberegp = GP_DR_REG(GP_IRSTR);
    datregp = GP_DR_REG(GP_IR7);
    ifmkregp = GP_DR_REG(GP_IFMK);

    drivesel(pstatep);

    while (allfiles || nfilesleft != 0) {
        while (allrecs || nrecsleft != 0) {
            /* Starting to read a record */
            wroff = 0;
            reclen = 0;

            if (reverse) {
                pcmd(pstatep, PCMD_IREV);        /* start the read in reverse */
            }
            else {
                pcmd(pstatep, 0);        /* start the read */
            }

            /*
             * First, wait for IFBY to go low (active)
             */
            if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), FALSE,
                         streamp, 0)) {
                streamp->println(
                    F("Failed waiting for IFBY to go low (active)"));
                goto alldone;
            }

            /*
             * Next, wait for IDBY to go low (active)
             */
            if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), FALSE,
                         streamp, 0)) {
                streamp->println(
                    F("Failed waiting for IDBY to go low (active)"));
                goto alldone;
            }

            busyregp = GP_DR_REG(GP_IDBY);

            lasthigh = TRUE;

            while (TRUE) {
                /* If IDBY goes high we're done */
                regval = *busyregp;
                if ((regval & GP_MASK(GP_IDBY)) != 0) {
                    break;
                }

                /* If IFMK goes active we're done */
                regval = *ifmkregp;
                if ((regval & GP_MASK(GP_IFMK)) == 0) {
                    /* !!! Could we get this before we the
                     *  record ends (e.g., before IDBY goes inactive)? */
                    goto filedone;
                }

                /* Has the strobe gone active (low)? */
                regval = *stroberegp;

                if ((regval & GP_MASK(GP_IRSTR)) != 0) {
                    lasthigh = TRUE;
                }
                else if (lasthigh) {
                    regval = ~*datregp;     /* Inverted because that's how
                                             *  the lines are defined. */
                    if (wroff < (TAPEBUFSIZ-1)) {
                        bufp[wroff++] = regval;
                    }
                    reclen++;
                    lasthigh = FALSE;
                }
            }

            /* Record is done. */
            recsread++;
            pstatep->wrptr = &(bufp[wroff]);     /* Stays valid only for
                                         * the last record read */
            if (minreclen == 0 || reclen < minreclen) {
                minreclen = reclen;
            }
            if (maxreclen == 0 || reclen > maxreclen) {
                maxreclen = reclen;
            }
            totalbytes += reclen;

            if (!allrecs) {
                nrecsleft--;
            }
        }
filedone:
        filesread++;
        if (!allfiles) {
            nfilesleft--;
        }
    }

alldone:
    drivedesel(pstatep);

    cmdifp->outstreamp->print(F("Total of "));
    cmdifp->outstreamp->print(totalbytes);
    cmdifp->outstreamp->print(F(" bytes in "));
    cmdifp->outstreamp->print(recsread);
    cmdifp->outstreamp->print(F(" records in"));
    cmdifp->outstreamp->print(filesread);
    cmdifp->outstreamp->println(F(" files."));

    if (recsread > 0) {
        if (minreclen == maxreclen) {
            cmdifp->outstreamp->print(F("Record size: "));
            cmdifp->outstreamp->print(minreclen);
            cmdifp->outstreamp->println(F(" bytes."));
        }
        else {
            cmdifp->outstreamp->print(F("Record size: minimum "));
            cmdifp->outstreamp->print(minreclen);
            cmdifp->outstreamp->print(F(", maximum "));
            cmdifp->outstreamp->print(maxreclen);
            cmdifp->outstreamp->println(F(" bytes."));
        }
    }
}

/*
 * Diddle GPIOs to confirm connectivity.
 */
static void
cmd_gpio(cmdif_t *cmdifp, int argc, char **argv)
{
    int             inchar;
    unsigned int    idx, regval;
    int             pinval, prevpinval, curtime, lasttime;
    uint8_t         ifconn, ifpin, pinnum;
    bool_t          asinputs, goingup, paused, forcemode, init, fini;
    pertexus_state_t    *pstatep;
    Stream          *streamp;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);
    streamp = cmdifp->outstreamp;

    init = FALSE;
    asinputs = TRUE;
    forcemode = FALSE;
    if (argc > 1) {
        if (strcmp_P(argv[1], PSTR("-forcein")) == 0) {
            asinputs = TRUE;
            forcemode = TRUE;
        }
        else if (strcmp_P(argv[1], PSTR("-forceout")) == 0) {
            asinputs = FALSE;
            forcemode = TRUE;
        }
        else if (strcmp_P(argv[1], PSTR("-out")) == 0) {
            asinputs = FALSE;
        }
        else if (strcmp_P(argv[1], PSTR("-init")) == 0) {
            init = TRUE;
        }
        else if (strcmp_P(argv[1], PSTR("-fini")) == 0) {
            fini = TRUE;
        }
        else {
            streamp->println(
                F("Usage: gpio [-fini|-init|[-out|-forcein|-forceout]]\r\n"));
            return;
        }
    }

    if (init && fini) {
        streamp->println(F("only one of -init/-fini allowed"));
        return;
    }
    if ((init || fini) && (forcemode || !asinputs)) {
        streamp->println(F("-init/-fini must be used by itself"));
        return;
    }

    if (init) {
        gpios_init(pstatep);
        return;
    }

    if (fini) {
        gpios_fini(pstatep);
        return;
    }

    /* If we're being asked to force, set the directions first */
    if (forcemode && asinputs) {
        /* Set the port expander pins as inputs */
        portexp_writeall(pstatep, PCA9535_CMD_CFG0, 0xff);

        /* Set both direction controls to LOW ("B to A") */
        digitalWrite(GP_PINNUM(GP_DIRA), LOW);      /* "B to A" (inputs) */
        digitalWrite(GP_PINNUM(GP_DIRB), LOW);      /* "B to A" (inputs) */

        for (idx = 0; idx < NELEM(gpinfos); idx++) {
            if ((gpinfos[idx].group & 1) == 0) {
                /* It's a traditional GPIO */
                pinMode(gpinfos[idx].pinnum, INPUT);
            }
        }
    }
    else if (forcemode && !asinputs) {
        /* Set the port expander pin values high */
        portexp_writeall(pstatep, PCA9535_CMD_OUTP0, 0xff);

        /* Set the port expander pins as outputs */
        portexp_writeall(pstatep, PCA9535_CMD_CFG0, 0);

        /* Set both direction controls to HIGH ("A to B") */
        digitalWrite(GP_PINNUM(GP_DIRA), HIGH);     /* "A to B" (outputs) */
        digitalWrite(GP_PINNUM(GP_DIRB), HIGH);     /* "A to B" (outputs) */

        for (idx = 0; idx < NELEM(gpinfos); idx++) {
            if ((gpinfos[idx].group & 1) == 0) {
                /* It's a traditional GPIO */
                digitalWrite(gpinfos[idx].pinnum, HIGH);
                pinMode(gpinfos[idx].pinnum, OUTPUT);
            }
        }
    }

    for (idx = 0; idx < PCA9535_NPORTS; idx++) {
        regval = portexp_read(pstatep, idx);
        streamp->print(F("INP"));
        streamp->print(idx);
        streamp->print(F("   0x"));
        hexprint(streamp, regval, 2);
        streamp->println();

        streamp->print(F("OUTP"));
        streamp->print(idx);
        streamp->print(F("  0x"));
        hexprint(streamp, pstatep->pexpregs[PCA9535_CMD_OUTP0 + idx], 2);
        streamp->println();

        streamp->print(F("INV"));
        streamp->print(idx);
        streamp->print(F("   0x"));
        hexprint(streamp, pstatep->pexpregs[PCA9535_CMD_INV0 + idx], 2);
        streamp->println();

        streamp->print(F("CFG"));
        streamp->print(idx);
        streamp->print(F("   0x"));
        hexprint(streamp, pstatep->pexpregs[PCA9535_CMD_CFG0 + idx], 2);
        streamp->println();
    }

    goingup = TRUE;
    prevpinval = 0;     /* dummy to nuke compiler whining */
    lasttime = 0;

#define TOGGLE_PERIOD       1000        /* 1 second */

#define GROUP_ISINPUT(g)    ((g) == GROUPA0 || (g) == GROUPA1           \
                             || (g) == GROUPC0 || (g) == GROUPC1)

    /* Walk through all the GPIOs, in Pertec pin order */
    ifconn = 1;
    ifpin = 1;

    while (ifconn >= 1 && ifconn <= 2) {
        for (idx = 0; idx < NELEM(gpinfos); idx++) {

            if (gpinfos[idx].percon == ifconn
                && gpinfos[idx].perpin == ifpin
                && (forcemode
                    || (asinputs && GROUP_ISINPUT(gpinfos[idx].group))
                    || (!asinputs && !GROUP_ISINPUT(gpinfos[idx].group)))) {

                paused = FALSE;

                printpad(streamp, &(gpinfos[idx].ifname[0]), 7);

                streamp->print(F(" J"));
                streamp->print(ifconn);
                streamp->print(':');
                streamp->print(ifpin);
                streamp->print(F(": "));
                pinnum = gpinfos[idx].pinnum;

                if (asinputs) {
                    if ((gpinfos[idx].group & 1) != 0) {
                        /* Extender GPIO */
                        pinval = _GP_PORTEXP_READ(pstatep, gpinfos[idx].setnum,
                                                  gpinfos[idx].bitnum);
                    }
                    else {
                        /* Traditional GPIO */
                        pinval = digitalRead(pinnum);
                    }
                    /* Input */
                    prevpinval = pinval;

                    streamp->print(pinval == HIGH ? F("HIGH")
                                              : F("LOW "));
                }
                else {
                    /* Output.  Toggle the output value 1/sec */
                    pinval = LOW;
                    lasttime = millis() - TOGGLE_PERIOD;
                    streamp->print(F("    "));
                }

                while (TRUE) {
                    inchar = streamp->read();
                    if (inchar >= 0) {
                        if (inchar == 3) {      /* ^C */
                            streamp->print(F("^C\r\n"));
                            goto cmdfinish;
                        }
                        else if (inchar == 13
                                 || inchar == 'n' || inchar == 'p') {
                            if (inchar == 'n') {
                                goingup = TRUE;
                            }
                            else if (inchar == 'p') {
                                goingup = FALSE;
                            }
                            streamp->print(F("\r\n"));
                            goto nextifpin;
                        }
                        else if (!asinputs && inchar == ' ') {
                            paused = !paused;
                        }
                    }
                    if (asinputs) {
                        if ((gpinfos[idx].group & 1) != 0) {
                            /* Extender GPIO */
                            pinval = _GP_PORTEXP_READ(pstatep,
                                                      gpinfos[idx].setnum,
                                                      gpinfos[idx].bitnum);
                        }
                        else {
                            /* Traditional GPIO */
                            pinval = digitalRead(pinnum);
                        }
                        if (pinval != prevpinval) {
                            streamp->print(
                                pinval == HIGH
                                ? F("\b\b\b\bHIGH")
                                : F("\b\b\b\bLOW "));
                            prevpinval = pinval;
                        }
                    }
                    else {
                        curtime = millis();
                        if (!paused && (curtime - lasttime) >= TOGGLE_PERIOD) {
                            if (pinval == LOW) {
                                streamp->print(
                                    F("\b\b\b\bHIGH"));
                                pinval = HIGH;
                            }
                            else {
                                streamp->print(
                                    F("\b\b\b\bLOW "));
                                pinval = LOW;
                            }
                            if ((gpinfos[idx].group & 1) != 0) {
                                /* Extender GPIO */
                                _GP_PORTEXP_WRITE(pstatep,
                                                  gpinfos[idx].setnum,
                                                  gpinfos[idx].bitnum,
                                                  pinval);
                            }
                            else {
                                /* Traditional GPIO */
                                digitalWrite(pinnum, pinval);
                            }
                            lasttime = curtime;
                        }
                    }
                }
            }   /* if GPIO matches ifconn/ifpin */
        }   /* for (idx = 0...) */
nextifpin:
        if (goingup) {
            if (ifpin < 50) {
                ifpin++;
            }
            else {
                ifconn++;
                ifpin = 1;
            }
        }
        else {
            if (ifpin > 1) {
                ifpin--;
            }
            else {
                ifconn--;
                ifpin = 50;
            }
        }
    }

cmdfinish:
    /* Restore GPIO functions, if necessary */
    if (forcemode) {
        gpios_init(pstatep);
    }
}

/*
 * Do commands on the SD card
 */
static void
cmd_sd(cmdif_t *cmdifp, int argc, char **argv)
{
    pertexus_state_t    *pstatep;
    Stream              *streamp;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);

    streamp = cmdifp->outstreamp;

    if (argc < 2) {
usage:
        streamp->println(F("Usage: sd init|dir|read|rm"));
        return;
    }

    if (strcmp_P(argv[1], PSTR("init")) == 0) {
        (void) sdcard_init(pstatep, streamp);
    }
    else if (strcmp_P(argv[1], PSTR("dir")) == 0) {
        File root = pstatep->sdp->open(PSTR("/"));
        /* !!!???error check */
        printDirectory(root, 0, streamp);
    }
    else if (strcmp_P(argv[1], PSTR("rm")) == 0) {
        if (argc < 3) {
            streamp->println(F("Usage: sd rm filename"));
            return;
        }

        if (!pstatep->sdp->remove(argv[2])) {
            streamp->print(F("Error: failed to remove "));
            streamp->println(argv[2]);
            return;
        }
    }
    else if (strcmp_P(argv[1], PSTR("read")) == 0) {
        File          readfile;
        uint32_t        header, footer, reclen, recleft;
        unsigned long   nbytes, nrecs, bytesread;
        unsigned long   starttime, deltatime;
        unsigned int    nfiles;
        uint16_t        readlen;
        uint8_t         buf[512];

        if (argc < 3) {
            streamp->println(F("Usage: sd read filename"));
            return;
        }

        readfile = pstatep->sdp->open(argv[2], FILE_READ);

        if (!readfile) {
            streamp->println(F("unable to open file"));
            return;
        }

        nrecs = nbytes = 0UL;
        nfiles = 0;
        starttime = millis();

        while (readfile.read(&header, sizeof(uint32_t)) == sizeof(uint32_t)) {
            if (header == 0UL) {
                /* Tape mark */
                nfiles++;
            }
            else if (header == 0xffffffeUL) {
                /* Erase gap */
            }
            else if (header >= 0xff000000UL && header <= 0xfffffffdUL) {
                /* Reserved */
            }
            else {
                reclen = header & 0x00ffffffUL;
                nbytes += (unsigned long) reclen;
                nrecs++;

                recleft = reclen;

                while (recleft != 0) {
                    if (recleft > sizeof(buf)) {
                        readlen = sizeof(buf);
                    }
                    else {
                        readlen = (uint16_t) recleft;
                    }
                    bytesread = readfile.read(&(buf[0]), readlen);
                    if ((uint16_t) bytesread != readlen) {
                        streamp->print(F("premature EOF, expected "));
                        streamp->print(readlen);
                        streamp->print(F(" bytes, got "));
                        streamp->print(bytesread);
                        streamp->println(F(" bytes"));
                        readfile.close();
                        return;
                    }
                    recleft -= (uint32_t) readlen;
                }

                /* Optionally skip a byte to keep headers 16-bit aligned */
                if ((reclen & 1) != 0) {
                    (void) readfile.seek(1, SeekCur);
                }

                if (readfile.read(&footer, sizeof(uint32_t))
                    != sizeof(uint32_t)) {
                    streamp->println(F("error: EOF when reading record footer"));
                    readfile.close();
                    return;
                }
                if (footer != header) {
                    streamp->print(F("warning: footer (0x"));
                    hexprint32(streamp, footer, 8);
                    streamp->print(F(") != header (0x"));
                    hexprint32(streamp, header, 8);
                    streamp->println(F(")"));
                }
            }
        }

        deltatime = (millis() - starttime);
        readfile.close();

        streamp->print(F("Total of "));
        streamp->print(nbytes, DEC);
        streamp->print(F(" bytes in "));
        streamp->print(nrecs, DEC);
        streamp->print(F(" records in "));
        streamp->print(nfiles, DEC);
        streamp->println(F(" files."));
        streamp->print(F(" Read in "));
        streamp->print(deltatime, DEC);
        streamp->print(F(" ms, or "));
        streamp->print((float) deltatime / (float) nrecs);
        streamp->print(F(" ms/rec, "));
        streamp->print(((float) nbytes * 1000.0) / (float) deltatime);
        streamp->println(F(" bytes/sec"));
    }
    else {
        goto usage;
    }
}

/*
 * Perform a "security" erase of an entire tape.
 */
static void
cmd_erase(cmdif_t *cmdifp, int argc, char **argv)
{
    unsigned long        starttime, deltatime;
    unsigned long        ips;
    bool_t               cmdcancel;
    pertexus_state_t    *pstatep;
    Stream              *streamp;

    pstatep = FROMWITHIN(cmdifp, pertexus_state_t, cmdif);
    streamp = cmdifp->outstreamp;

    UNUSED(argc);
    UNUSED(argv);

    drivesel(pstatep);

    /*
     * Check to see if writing is disabled.
     */
    if (GP_READ(pstatep, GP_IFPT) == 0) {
        streamp->println(F("Error: tape is write protected"));
        goto finish;
    }

    /*
     * And make sure we're at the beginning of the tape.
     */
    if (GP_READ(pstatep, GP_ILDP) != 0) {
        streamp->println(F("Error: tape is not at load point"));
        goto finish;
    }

    starttime = millis();
    cmdcancel = FALSE;

    /* Issue the command to search forward for a file mark */
    pcmd(pstatep, PCMD_IWRT | PCMD_IERASE | PCMD_IWFM | PCMD_IEDIT);

    if (!bitwait(GP_DR_REG(GP_IFBY), GP_MASK(GP_IFBY), FALSE, NULL, 1000)) {
        streamp->println(F("Error: IFBY failed to assert"));
        cmdcancel = TRUE;
    }
    else if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), FALSE,
                      streamp, 0)) {
        /* We must have aborted via ^C */
        streamp->println(F("\r\nAborted waiting for IDBY to assert."));
        cmdcancel = TRUE;
    }
    else if (!bitwait(GP_DR_REG(GP_IDBY), GP_MASK(GP_IDBY), TRUE,
                      streamp, 0)) {
        /* We must have aborted via ^C */
        streamp->println(F("\r\nAborted waiting for IDBY to deassert."));
        cmdcancel = TRUE;
    }
    else {
        streamp->println(F("Erase completed succesfully."));
    }

    if (cmdcancel) {
        /* Cancel any pending command */
        ifreset(pstatep);
    }

#define ROUNDDIV(x, y)      (((x) + ((y)/2))/(y))

    deltatime = (millis() - starttime);
    streamp->print(F("A total of "));
    streamp->print(deltatime, DEC);
    streamp->println(F(" ms elapsed."));
    ips = 50UL;
    streamp->print(F("At "));
    streamp->print(ips, DEC);
    streamp->print(F(" ips that represents "));
    streamp->print(ROUNDDIV(deltatime * ips, 1000UL), DEC);
    streamp->print(F(" inches, or "));
    streamp->print(ROUNDDIV(deltatime * ips, 12UL * 1000UL), DEC);
    streamp->println(F(" feet."));

finish:
    drivedesel(pstatep);
}

/* -------------------------------------------------------------------------- */
/* Static functions (FLASHMEM).  These are low-use functions that can
 *  exist entirely in slow-access flash */

/*
 * Initialize GPIOs
 */
static void
gpios_init(pertexus_state_t *pstatep)
{
    unsigned int        regval;
    unsigned int        idx, ninregs, portnum;

    /*
     * Next, initialize the PCA9535 directions.
     */
    /* Clear the inverting bits */
    portexp_writeall(pstatep, PCA9535_CMD_INV0, 0);
    /* Set the outputs to all high */
    portexp_writeall(pstatep, PCA9535_CMD_OUTP0, 0xff);

    for (portnum = 0; portnum < PCA9535_NPORTS; portnum++) {
        regval = 0xff; /* Configuration: default is input */

        for (idx = 0; idx < NELEM(gpinfos); idx++) {
            if (gpinfos[idx].group == GROUPB1
                && gpinfos[idx].setnum == portnum) {
                /* Controller outputs on the port expander */
                regval &= ~(1 << gpinfos[idx].bitnum);
            }
        }
        portexp_write(pstatep, PCA9535_CMD_CFG0 + portnum, regval);
    }

#if 1
    /*
     * Initialize all lines that go through level shifters
     *  as inputs with weak pullups so that
     *  when we turn on the level shifters there are defined values
     *  for those lines that will be outputs.
     */
    for (idx = 0; idx < NELEM(gpinfos); idx++) {
        if (gpinfos[idx].group == GROUPB0 || gpinfos[idx].group == GROUPA0) {
            digitalWrite(gpinfos[idx].pinnum, HIGH);
            pinMode(gpinfos[idx].pinnum, INPUT_PULLUP);
        }
    }
#endif

#if 0
#define CHECKPOINT(s)                                                   \
    do {                                                                \
        int     inchar;                                                 \
        Serial.print(F(s " hit any key to continue"));                  \
        do {                                                            \
            inchar = Serial.read();                                     \
        } while (inchar < 0);                                           \
        Serial.println();                                               \
    } while (FALSE)

    CHECKPOINT("#1");
#else
#define CHECKPOINT(s)
#endif


#if 0
    /*
     * Give it 10 milliseconds to settle.  This is probably
     *  way more time than is really needed.
     */
    delay(10);
#endif

    /*
     * Now set the level shifter direction and output enable pins as outputs
     *  and with their correct values.
     */

#if 1
    /*
     * Bug workaround: pinMode(, OUTPUT) does not set the value of an
     *  output before it switches the output on.  This means that
     *  calls to digitalWrite() before pinMode() don't actually set
     *  the output -- they turn on the pullup.
     * So we need to set the value by touching the DR register
     *  (actually, via DR_SET and DR_CLEAR) first before calling
     *  pinMode.  Yuck.
     */
    GP_WRITE(pstatep, GP_DIRA, 0);
    GP_WRITE(pstatep, GP_DIRB, 1);
    GP_WRITE(pstatep, GP_OEA, 0);
    pinMode(GP_PINNUM(GP_DIRA), OUTPUT);
    pinMode(GP_PINNUM(GP_DIRB), OUTPUT);
    pinMode(GP_PINNUM(GP_OEA), OUTPUT);
    digitalWrite(GP_PINNUM(GP_DIRA), LOW);      /* "B to A" (inputs) */
    digitalWrite(GP_PINNUM(GP_DIRB), HIGH);     /* "A to B" (outputs) */
    digitalWrite(GP_PINNUM(GP_OEA), LOW);       /* enable */
#else
    digitalWrite(GP_PINNUM(GP_DIRA), HIGH);     /* "A to B" (outputs) */
    digitalWrite(GP_PINNUM(GP_DIRB), LOW);     /* "A to B" (outputs) */
    digitalWrite(GP_PINNUM(GP_OEA), LOW);       /* enable */
    pinMode(GP_PINNUM(GP_DIRA), OUTPUT);
    pinMode(GP_PINNUM(GP_DIRB), OUTPUT);
    pinMode(GP_PINNUM(GP_OEA), OUTPUT);
#endif

    CHECKPOINT("#2");
    /*
     * Initialize inputs
     *  (This just turns off the now-unneeded pullup resistors)
     */
    ninregs = 0;
    for (idx = 0; idx < NELEM(gpinfos); idx++) {
        if (gpinfos[idx].group == GROUPA0) {
            pinMode(gpinfos[idx].pinnum, INPUT);
            ninregs++;
        }
    }
    pstatep->ninregs = ninregs;

    CHECKPOINT("#3");
#if 1
    /* Initialize outputs */
    for (idx = 0; idx < NELEM(gpinfos); idx++) {
        if (gpinfos[idx].group == GROUPB0) {
            /*
             * Bug workaround (see above).
             */
            _GP_TRADGPIO_WRITE(gpinfos[idx].setnum, gpinfos[idx].bitnum, 1);
            pinMode(gpinfos[idx].pinnum, OUTPUT);
            digitalWrite(gpinfos[idx].pinnum, HIGH);
//            delay(10);
        }
    }

#else
    Serial.println(F("Hit any key to initialize each signal"));
    for (idx = 0; idx < NELEM(gpinfos); idx++) {
        int inchar;

        if (gpinfos[idx].group == GROUPB0) {
            Serial.print(gpinfos[idx].ifname);
            do {
                inchar = Serial.read();
            } while (inchar < 0);
            pinMode(gpinfos[idx].pinnum, OUTPUT);
            digitalWrite(gpinfos[idx].pinnum, HIGH);
            Serial.println();
        }
    }
#endif

#if 0
    CHECKPOINT("#4");

    /* Set IFEN */
    GP_WRITE(pstatep, GP_IFEN, 0);
#endif
}


/*
 * Mostly undo the action of gpios_init()
 */
static void
gpios_fini(pertexus_state_t *pstatep)
{
    unsigned int        idx;

#if 1
#define FINICHECKPOINT(s)                                               \
    do {                                                                \
        int     inchar;                                                 \
        Serial.print(F(s " hit any key to continue"));                  \
        do {                                                            \
            inchar = Serial.read();                                     \
        } while (inchar < 0);                                           \
        Serial.println();                                               \
    } while (FALSE)
#else
#define FINICHECKPOINT(s)
#endif

    /* First, set all traditional GPIOS to be inputs */
    FINICHECKPOINT("#1");

    for (idx = 0; idx < NELEM(gpinfos); idx++) {
        if (gpinfos[idx].group == GROUPB0 || gpinfos[idx].group == GROUPA0) {
            digitalWrite(gpinfos[idx].pinnum, HIGH);
            pinMode(gpinfos[idx].pinnum, INPUT_PULLUP);
        }
    }

    /* Next, switch the port expander outputs to be inputs as well. */
    FINICHECKPOINT("#2");

    portexp_writeall(pstatep, PCA9535_CMD_CFG0, 0xff);

    /* Finally, switch the level shifters to be inputs only */
    FINICHECKPOINT("#3");
    digitalWrite(GP_PINNUM(GP_DIRA), LOW);      /* "B to A" (inputs) */
    digitalWrite(GP_PINNUM(GP_DIRB), LOW);      /* "B to A" (inputs) */
    digitalWrite(GP_PINNUM(GP_OEA), HIGH);      /* Disable output,
                                                 * just for fun */
}


/*
 * Print out all of the state.
 */
static void
debug_dumpstate(pertexus_state_t *pstatep, Print *outstreamp)
{
    unsigned int         nbytes, baseoff, off;
    uint8_t             byteval;
    uint8_t             *baserdptr, *rdptr;

    outstreamp->print(F("opstate = "));
    outstreamp->println(pstatep->opstate, DEC);

    outstreamp->print(F("fmcount = "));
    outstreamp->println(pstatep->fmcount, DEC);

    outstreamp->print(F("rdrec = "));
    outstreamp->print(pstatep->rdrec, DEC);
    outstreamp->print(F(", wrrec = "));
    outstreamp->print(pstatep->wrrec, DEC);
    outstreamp->print(F(", ISEMPTY = "));
    outstreamp->print((int) RING_ISEMPTY(pstatep->wrrec,
                                         pstatep->rdrec, TAPENRECS), DEC);
    outstreamp->print(F(", ISFULL = "));
    outstreamp->println((int) RING_ISFULL(pstatep->wrrec,
                                         pstatep->rdrec, TAPENRECS), DEC);

    outstreamp->print(F("base = 0x"));
    hexprint32(outstreamp, (uint32_t) &(pstatep->buf[0]), 8);
    outstreamp->print(F(", rdptr = 0x"));
    hexprint32(outstreamp, (uint32_t) pstatep->rdptr, 8);
    outstreamp->print(F(", wrptr = 0x"));
    hexprint32(outstreamp, (uint32_t) pstatep->wrptr, 8);
    outstreamp->print(F(", FREE = "));
    outstreamp->print(RINGPTR_FREE(pstatep->wrptr,
                                   pstatep->rdptr, TAPEBUFSIZ), DEC);
    outstreamp->print(F(", USED = "));
    outstreamp->print(RINGPTR_USED(pstatep->wrptr,
                                   pstatep->rdptr, TAPEBUFSIZ), DEC);
    outstreamp->print(F(", ISEMPTY = "));
    outstreamp->print(RINGPTR_ISEMPTY(pstatep->wrptr,
                                      pstatep->rdptr, TAPEBUFSIZ), DEC);
    outstreamp->print(F(", ISFULL = "));
    outstreamp->println(RINGPTR_ISFULL(pstatep->wrptr,
                                       pstatep->rdptr, TAPEBUFSIZ), DEC);

    outstreamp->print(F("harderrcount = "));
    outstreamp->println(pstatep->harderrcount, DEC);

    outstreamp->print(F("recbytes = "));
    outstreamp->println((uint32_t) pstatep->recbytes, DEC);

    outstreamp->print(F("filesdone = "));
    outstreamp->println(pstatep->filesdone, DEC);

    outstreamp->print(F("recsdone = "));
    outstreamp->println(pstatep->recsdone, DEC);

    /* print out the buffer pointers */
    nbytes = RINGPTR_USED(pstatep->wrptr, pstatep->rdptr, TAPEBUFSIZ);

    /* now dump the data buffer */
    baserdptr = pstatep->rdptr;
    baseoff = 0;

    while (baseoff < nbytes) {
        hexprint(outstreamp, baseoff, 4);
        outstreamp->print(F(" "));
        hexprint32(outstreamp, (uint32_t) baserdptr, 8);
        outstreamp->print(F(":  "));

        rdptr = baserdptr;
        /* First the hexadecimal values */
        for (off = 0; off < 16; off++) {
            outstreamp->print(F(" "));
            if ((baseoff + off) < nbytes) {
                hexprint(outstreamp, *rdptr++, 2);
                RINGPTR_WRAP(rdptr, &(pstatep->buf[0]), TAPEBUFSIZ);
            }
            else {
                outstreamp->print(F("  "));
            }
        }

        outstreamp->print(F("   |"));
        /* Now the text */
        for (off = 0; off < 16; off++) {
            if ((baseoff + off) < nbytes) {
                byteval = *baserdptr++;

                if (isPrintable(byteval)) {
                    outstreamp->print((char) byteval);
                }
                else {
                    outstreamp->print('.');
                }
                RINGPTR_WRAP(baserdptr, &(pstatep->buf[0]), TAPEBUFSIZ);
            }
            else {
                outstreamp->print(' ');
            }
        }
        outstreamp->println(F("|"));
        baseoff += 16;
    }

    /*
     * Now dump the record buffer.
     */
}

/*
 * Initialize the connection to the SD card.
 */
static bool_t
sdcard_init(pertexus_state_t *pstatep, Print *streamp)
{
    streamp->print(F("\nInitializing SD card..."));

    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!pstatep->sdp->begin(BUILTIN_SDCARD)) {
        streamp->println(
            F(
"initialization failed. Things to check:\r\n"
"* is a card inserted?\r\n"
"* is your wiring correct?\r\n"));
        return FALSE;
    }
    else {
        streamp->println(F("Wiring is correct and a card is present."));
    }

    streamp->print(F("FAT type: "));
    pstatep->sdp->sdfs.printFatType(streamp);

    // print the type of card
    streamp->println();
    streamp->print(F("Card type:         "));

    switch (pstatep->sdp->sdfs.card()->type()) {
    case SD_CARD_TYPE_SD1:
        streamp->println(F("SD1"));
        break;

    case SD_CARD_TYPE_SD2:
        streamp->println(F("SD2"));
        break;

    case SD_CARD_TYPE_SDHC:
        streamp->println(F("SDHC"));
        break;

    default:
        streamp->println(F("Unknown"));
        break;
    }

    pstatep->sdcardinit = TRUE;

    return TRUE;
}

static void
printDirectory(File dir, int numSpaces, Print *outstreamp)
{
    File        entry;

    while ((entry = dir.openNextFile())) {
        printSpaces(numSpaces, outstreamp);
        outstreamp->print(entry.name());
        if (entry.isDirectory()) {
            outstreamp->println(F("/"));
            printDirectory(entry, numSpaces+2, outstreamp);
        }
        else {
            // files have sizes, directories do not
            unsigned int n = log10(entry.size());
            n = MIN(n, 10);
            printSpaces(50 - numSpaces - strlen(entry.name()) - n + 2,
                        outstreamp);
            outstreamp->print(entry.size(), DEC);
            DateTimeFields datetime;
            if (entry.getModifyTime(datetime)) {
                printSpaces(4, outstreamp);
                printTime(datetime, outstreamp, FALSE, FALSE);
            }
            outstreamp->println();
        }
        entry.close();
    }
}

static void
printSpaces(int num, Print *outstreamp)
{
    for (int i=0; i < num; i++) {
        outstreamp->print(' ');
    }
}

static void
printTime(const DateTimeFields tm, Print *outstreamp,
          bool_t printwday, bool_t printsecs)
{
    if (printwday) {
        if (tm.wday >= 7) {
            outstreamp->print(F("???"));
        }
        else {
            outstreamp->write(&(daysofweek[tm.wday*3]), 3);
        }
        outstreamp->print(' ');
    }

    if (tm.mon >= 12) {
        outstreamp->print(F("???"));
    }
    else {
        outstreamp->write(&(monthnames[tm.mon*3]), 3);
    }
    outstreamp->print(' ');

    if (tm.mday < 10) {
        outstreamp->print(' ');
    }
    outstreamp->print(tm.mday);
    outstreamp->print(' ');

    if (tm.hour < 10) {
        outstreamp->print(' ');
    }
    outstreamp->print(tm.hour);
    outstreamp->print(':');
    if (tm.min < 10) {
        outstreamp->print('0');
    }
    outstreamp->print(tm.min);

    if (printsecs) {
        outstreamp->print(':');
        if (tm.sec < 10) {
            outstreamp->print('0');
        }
        outstreamp->print(tm.sec);
    }
    outstreamp->print(' ');
    outstreamp->print(tm.year + 1900);
}

/*
 * Print out a string, with optional trailing padding.
 */
static void
printpad(Print *streamp, const char *strp, int padlen)
{
    char         strc;

    while (TRUE) {
        strc = *strp;
        if (strc == '\0') {
            break;
        }
        streamp->print(strc);
        if (padlen > 0) {
            padlen--;
        }

        strp++;
    }

    while (padlen > 0) {
        streamp->print(' ');
        padlen--;
    }
}

/*
 * Print a hexadecimal value on outstream.  ndigits
 *  is the minimum number of digits to use.  Specify 0
 *  to use as many digits as needed.
 */
static void
hexprint(Print *outstream, unsigned int val, int ndigits)
{
#if 1
    /* The loop version */
    unsigned int         mask, shift;

    /* Find the first digital of the value */
    mask = 0xf;
    shift = 0;
    while (--ndigits > 0 || val > mask) {
        mask = (mask << 4) | 0xf;
        shift += 4;
    }

    while (mask != 0) {
        outstream->print(HEXDIGIT((val & mask) >> shift));
        mask >>= 4;
        shift -= 4;
    }
#else
    /* Recursive version */
    ndigits--;
    if (ndigits > 0 || val > 0x0f) {
        hexprint(outstream, val >> 4, ndigits);
    }
    outstream->print(HEXDIGIT(val));
#endif
}


/*
 * Print a 32-bit hexadecimal value on outstream.  ndigits
 *  is the minimum number of digits to use.  Specify 0
 *  to use as many digits as needed.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
/* This function is called from debugging code so it may
 *  not be needed.  Hence the 'pragma' */
static void
hexprint32(Print *outstream, uint32_t val, int ndigits)
{
#if 1
    /* The loop version */
    uint32_t         mask;
    unsigned int         shift;

    /* Find the first digital of the value */
    mask = 0xf;
    shift = 0;
    while (--ndigits > 0 || val > mask) {
        mask = (mask << 4) | 0xf;
        shift += 4;
    }

    while (mask != 0) {
        outstream->print(HEXDIGIT((val & mask) >> shift));
        mask >>= 4;
        shift -= 4;
    }
#else
    /* Recursive version */
    ndigits--;
    if (ndigits > 0 || val > 0x0f) {
        hexprint32(outstream, val >> 4, ndigits);
    }
    outstream->print(HEXDIGIT(val));
#endif
}
#pragma GCC diagnostic pop


/*
 * Glue code to get the TimeLib to use the built-in
 *  Teensy 4.1 RTC (which for some reason is called 'Teensy3Clock').
 */
static time_t
getTeensy3Time(void)
{
    return Teensy3Clock.get();
}

/*
 * Set up the chip to support communication two/from the
 *  PCA9535 port expander.
 */
static void
pca9535_init(pertexus_state_t *pstatep)
{
    pstatep->portexp = &Wire2;
    pstatep->sdp = &SD;

    /*
     * First, setup the I2C to communicate with the
     *  port expander.
     */
    pstatep->portexp->begin();
    pstatep->portexp->setClock(400000);

    /*
     * Now, set up the INT GPIO line as an input.
     */
    pinMode(GP_PINNUM(GP_LPI2C4_INT), INPUT_PULLUP);
}

/*
 * Cook up a temporary filename on the given FS.
 *  bufp should point to at least 13 bytes (TMPFNLEN+1) of storage.
 */
static bool_t
tmpfn_gen(FS *fsp, char *bufp)
{
    char            *nump;
    unsigned int     idx, divisor, tmp;

    strcpy_P(bufp, PSTR("TEMP0000.TMP"));

    idx = 0;
    do {
        nump = &(bufp[4]);
        divisor = 1000;
        tmp = idx;
        do {
            *nump++ = '0' + (tmp / divisor);
            tmp = tmp % divisor;
            divisor /= 10;
        }
        while (divisor != 0);

        if (!fsp->exists(bufp)) {
            return TRUE;
        }
        idx++;
    } while (idx < 10000);

    return FALSE;
}

/*
 * Copy the (already open) TAP file to a reversed
 *  data file.
 */

static bool_t
tapfilerev(pertexus_state_t *pstatep, Print *outprintp,
           File *writefile, File *tmpfile, uint32_t lastfooteroff)
{
    uint32_t     header, reclen, chunksize, swapcount;
    uint8_t     *rdp, *wrp;
    File        newfile;

    outprintp->print(F("Reversing file, filesize = "));
    outprintp->print(tmpfile->size(), DEC);
    outprintp->print(F(", current position = "));
    outprintp->print(tmpfile->position(), DEC);
    outprintp->print(F(", lastfooteroff = "));
    outprintp->println(lastfooteroff, DEC);

    /*
     * Go back, record by record, and reverse the data.
     */

    while (lastfooteroff >= sizeof(uint32_t)) {
        lastfooteroff -= sizeof(uint32_t);
        tmpfile->seek(lastfooteroff, SeekSet);

        if (tmpfile->read(&header, sizeof(uint32_t)) !=
            sizeof(uint32_t)) {
            outprintp->println(F("Error reading footers when reversing file"));
            return FALSE;
        }
#if 0
        outprintp->print(F("Read header at "));
        outprintp->print(lastfooteroff);
        outprintp->print(F(", value = 0x"));
        hexprint32(outprintp, header, 8);
#endif

        /*
         * Yuck.  Read bytes, 2 at a time, and write them
         *  back out again.
         */
        if (writefile->write(&header, sizeof(header))
            != sizeof(uint32_t)) {
            outprintp->println(F("Error writing header to reversed file"));
            return FALSE;
        }

        reclen = (recsize_t) (header & 0x00ffffff);

        if (reclen == 0) {
            /* It's just a marker with no footer.  Skip back. */
            continue;
        }
        /* Convert reclen to a multiple of 2 */
        reclen = (reclen + 1) & ~1;

        /* Read the bytes, chunk at a time */
        while (reclen != 0) {
            chunksize = MIN(reclen, TAPEBUFSIZ/2);
            lastfooteroff -= chunksize;

            if (!tmpfile->seek(lastfooteroff, SeekSet)) {
                outprintp->print(F("error: unable to seek to "));
                outprintp->print(lastfooteroff);
                outprintp->println(F(" in temporary file"));
                return FALSE;
            }

            if (tmpfile->read(&(pstatep->buf[0]), chunksize) != chunksize) {
                outprintp->print(F("error: unable to read data at "));
                outprintp->print(lastfooteroff);
                outprintp->println(F(" from temporary file"));
                return FALSE;
            }

            /*
             * Rewrite the data.
             */
            swapcount = chunksize;
            rdp = &(pstatep->buf[chunksize]);
            wrp = &(pstatep->buf[TAPEBUFSIZ/2]);

            while (swapcount-- != 0) {
                *wrp++ = *--rdp;
            }

            if (writefile->write(&(pstatep->buf[TAPEBUFSIZ/2]), chunksize)
                != chunksize) {

                outprintp->print(
                    F("error: unable to write data from temporary file at "));
                outprintp->print(lastfooteroff);
                outprintp->println(F(" to reversed file"));
                return FALSE;
            }

            reclen -= chunksize;
        }

        /* Now skip back over the gap in the temporary
         *  file where the header was supposed to go. */
        lastfooteroff -= sizeof(uint32_t);

        /* And write another copy of the footer */

        if (writefile->write(&header, sizeof(header))
            != sizeof(header)) {
            outprintp->println(
                F("error: unable to write footer to reversed file"));
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * "Finalize" a TAP file by walking backwards through it to
 *  fill in the headers (which were written as 0s) with copies
 *  of the footers.
 */

static bool_t
tapfilefinalize(pertexus_state_t *pstatep, Print *outprintp,
                File *writefile, uint32_t lastfooteroff)
{
    uint32_t         header, reclen;

    outprintp->print(F("Finalizing file, filesize = "));
    outprintp->print(writefile->size(), DEC);
    outprintp->print(F(", current position = "));
    outprintp->print(writefile->position(), DEC);
    outprintp->print(F(", lastfooteroff = "));
    outprintp->println(lastfooteroff, DEC);

    while (lastfooteroff >= sizeof(uint32_t)) {
        lastfooteroff -= sizeof(uint32_t);
        writefile->seek(lastfooteroff, SeekSet);
        /* outprintp->print(F(" seeking to read at ")); */
        /* outprintp->println(lastfooteroff, DEC); */

        if (writefile->read(&header, sizeof(uint32_t)) != sizeof(uint32_t)) {
            outprintp->println(F("Error reading footers when finalizing file"));
            return FALSE;
        }

        reclen = (recsize_t) (header & 0x00ffffff);
        /* outprintp->print(F(" read header = 0x")); */
        /* hexprint32(streamp, header, 8); */
        /* outprintp->print(F(", reclen = ")); */
        /* outprintp->println(reclen, DEC); */

        if (reclen == 0) {
            /* It's just a marker with no footer.  Skip back. */
            continue;
        }

        /* Round up to an even record length so that
         *  headers and footers stay 16-bit aligned */
        reclen = (reclen + 1) & ~1;
        lastfooteroff -= (reclen + sizeof(uint32_t));
        /* outprintp->print(F(" seeking to write at ")); */
        /* outprintp->println(lastfooteroff, DEC); */

        writefile->seek(lastfooteroff, SeekSet);

        if (writefile->write(&header, sizeof(uint32_t)) != sizeof(uint32_t)) {
            outprintp->println(F("Error writing header when finalizing file"));
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * Select the drive.
 */
static void
drivesel(pertexus_state_t *pstatep)
{
    uint8_t     portval;

    /*
     * Cheat: we know all of the address lines (IFAD, ITAD0, ITAD1)
     *  are all on the port expander and in the same byte.
     */
    /* Assert all of the lines are on the port expander */
    ASSERT_CT((GP_GROUP(GP_IFAD) & 1) != 0);
    ASSERT_CT((GP_GROUP(GP_ITAD0) & 1) != 0);
    ASSERT_CT((GP_GROUP(GP_ITAD1) & 1) != 0);
    /* Assert they're all in the same set */
    ASSERT_CT(GP_SETNUM(GP_IFAD) == GP_SETNUM(GP_ITAD0));
    ASSERT_CT(GP_SETNUM(GP_ITAD0) == GP_SETNUM(GP_ITAD1));

    portval = pstatep->pexpregs[PCA9535_CMD_OUTP0 + GP_SETNUM(GP_IFAD)];

    /*
     * Reference: Ref[1] p. 3-5 section 3.5.2 "Address Lines and
     *  Formatter Select"
     *                          SELECTED
     *  IFAD    ITAD0   ITAD1   DRIVE
     *  H       H       H       0
     *  H       H       L       1
     *  H       L       H       2
     *  H       L       L       3
     *  L       H       H       4
     *  L       H       L       5
     *  L       L       H       6
     *  L       L       L       7
     */
    portval |= (GP_MASK(GP_IFAD) | GP_MASK(GP_ITAD0) | GP_MASK(GP_ITAD1));
    if ((pstatep->driveaddr & 1) != 0) {
        portval &= ~GP_MASK(GP_ITAD1);
    }
    if ((pstatep->driveaddr & 2) != 0) {
        portval &= ~GP_MASK(GP_ITAD0);
    }
    if ((pstatep->driveaddr & 4) != 0) {
        portval &= ~GP_MASK(GP_IFAD);
    }

    portexp_write(pstatep, PCA9535_CMD_OUTP0 + GP_SETNUM(GP_IFAD), portval);

    /* Set IFEN */
    GP_WRITE(pstatep, GP_IFEN, 0);
}

/*
 * Deselect the drive.
 */
static void
drivedesel(pertexus_state_t *pstatep)
{
    /* Set IFEN */
    GP_WRITE(pstatep, GP_IFEN, 1);
}


