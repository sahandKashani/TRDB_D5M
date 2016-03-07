#ifndef __CMOS_SENSOR_INPUT_REGS_H__
#define __CMOS_SENSOR_INPUT_REGS_H__

#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#include "cmos_sensor_input_io.h"

/*
 * log2_of_pow_2
 *
 * Computes log2(x) for the supplied 32-bit unsigned number. Must be sure
 * that x is a power of 2 before calling this function.
 *
 * Full credit for this code comes from the link below:
 *   http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
 */
static uint32_t log2_of_pow_2(uint32_t x) {
    static const int MultiplyDeBruijnBitPosition2[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    return MultiplyDeBruijnBitPosition2[(uint32_t) (x * 0x077CB531U) >> 27];
}

/*
 * mask_ofst
 *
 * Computes the number of 0s to the right of the mask's least significant bit
 * set to 1. The mask must only contain consecutive 1 bits, and cannot have
 * 1 bits separated by any 0s.
 *                                              vv
 * Ex : 0x 00 0c 00 00 (good) = 0b 00000000 00001100 00000000 00000000
 *
 *                                               vxv
 * Ex : 0x 00 05 00 00 (bad)  = 0b 00000000 00000101 00000000 00000000
 */
static uint32_t mask_ofst(uint32_t mask) {
    /* (mask & -mask) clears all but the lowest bit of mask */
    return log2_of_pow_2(mask & (~mask + 1));
}

#define CMOS_SENSOR_INPUT_CONFIG_OFST                       (0 * 4) /* RW */
#define CMOS_SENSOR_INPUT_COMMAND_OFST                      (1 * 4) /* WO */
#define CMOS_SENSOR_INPUT_STATUS_OFST                       (2 * 4) /* RO */
#define CMOS_SENSOR_INPUT_FRAME_INFO_OFST                   (3 * 4) /* RO */

#define CMOS_SENSOR_INPUT_CONFIG_ADDR(base)                 ((void *) ((uint8_t *) (base) + CMOS_SENSOR_INPUT_CONFIG_OFST))
#define CMOS_SENSOR_INPUT_COMMAND_ADDR(base)                ((void *) ((uint8_t *) (base) + CMOS_SENSOR_INPUT_COMMAND_OFST))
#define CMOS_SENSOR_INPUT_STATUS_ADDR(base)                 ((void *) ((uint8_t *) (base) + CMOS_SENSOR_INPUT_STATUS_OFST))
#define CMOS_SENSOR_INPUT_FRAME_INFO_ADDR(base)             ((void *) ((uint8_t *) (base) + CMOS_SENSOR_INPUT_FRAME_INFO_OFST))

#define CMOS_SENSOR_INPUT_CONFIG_IRQ_MASK                   (0x00000001)
#define CMOS_SENSOR_INPUT_CONFIG_IRQ_OFST                   (mask_ofst(CMOS_SENSOR_INPUT_CONFIG_IRQ_MASK))
#define CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE                (0)
#define CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE                 (1)
#define CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE_MASK           (CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE << CMOS_SENSOR_INPUT_CONFIG_IRQ_OFST)
#define CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE_MASK            (CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE << CMOS_SENSOR_INPUT_CONFIG_IRQ_OFST)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_MASK       (0x00000006)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_OFST       (mask_ofst(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_MASK))
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB       (0)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BGGR       (1)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GRBG       (2)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GBRG       (3)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB_MASK  (0 << CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_OFST)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BGGR_MASK  (1 << CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_OFST)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GRBG_MASK  (2 << CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_OFST)
#define CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GBRG_MASK  (3 << CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_OFST)

#define CMOS_SENSOR_INPUT_COMMAND_GET_FRAME_INFO            (0)
#define CMOS_SENSOR_INPUT_COMMAND_SNAPSHOT                  (1)
#define CMOS_SENSOR_INPUT_COMMAND_IRQ_ACK                   (2)
#define CMOS_SENSOR_INPUT_COMMAND_STOP_AND_RESET            (3)

#define CMOS_SENSOR_INPUT_STATUS_STATE_MASK                 (0x00000001)
#define CMOS_SENSOR_INPUT_STATUS_STATE_OFST                 (mask_ofst(CMOS_SENSOR_INPUT_STATUS_STATE_MASK))
#define CMOS_SENSOR_INPUT_STATUS_STATE_IDLE                 (0)
#define CMOS_SENSOR_INPUT_STATUS_STATE_BUSY                 (1)
#define CMOS_SENSOR_INPUT_STATUS_STATE_IDLE_MASK            (0 << CMOS_SENSOR_INPUT_STATUS_STATE_OFST)
#define CMOS_SENSOR_INPUT_STATUS_STATE_BUSY_MASK            (1 << CMOS_SENSOR_INPUT_STATUS_STATE_OFST)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_MASK             (0x00000002)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OFST             (mask_ofst(CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_MASK))
#define CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_NO_OVERFLOW      (0)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW         (1)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_NO_OVERFLOW_MASK (CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_NO_OVERFLOW << CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OFST)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW_MASK    (CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW << CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OFST)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_MASK            (0x00001ffc)
#define CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_OFST            (mask_ofst(CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_MASK))

#define CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_MASK       (0x0000ffff)
#define CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_OFST       (mask_ofst(CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_MASK))
#define CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_MASK      (0xffff0000)
#define CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_OFST      (mask_ofst(CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_MASK))

#define CMOS_SENSOR_INPUT_WR_CONFIG(base, data)             cmos_sensor_input_write_word(CMOS_SENSOR_INPUT_CONFIG_ADDR((base)), (data))
#define CMOS_SENSOR_INPUT_WR_COMMAND(base, data)            cmos_sensor_input_write_word(CMOS_SENSOR_INPUT_COMMAND_ADDR((base)), (data))
#define CMOS_SENSOR_INPUT_RD_CONFIG(base)                   cmos_sensor_input_read_word(CMOS_SENSOR_INPUT_CONFIG_ADDR((base)))
#define CMOS_SENSOR_INPUT_RD_STATUS(base)                   cmos_sensor_input_read_word(CMOS_SENSOR_INPUT_STATUS_ADDR((base)))
#define CMOS_SENSOR_INPUT_RD_FRAME_INFO(base)               cmos_sensor_input_read_word(CMOS_SENSOR_INPUT_FRAME_INFO_ADDR((base)))

#endif /* __CMOS_SENSOR_INPUT_REGS_H__ */
