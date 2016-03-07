/*
###################
# trdb_d5m_regs.h #
###################
Device            : TRDB-D5M camera module from terasic
Author            : Sahand Kashani-Akhavan / Philémon Favrod
Revision          : 1.4
Modification date : 07/03/2016
*/

#ifndef __TRDB_D5M_REGS_H__
#define __TRDB_D5M_REGS_H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

///////////////////////
// GENERAL FUNCTIONS //
///////////////////////

// Calculates log2 of a number. Attention: the number must be a power of 2
static uint16_t TRDB_D5M_log2(uint16_t power_of_2) {
    if      (power_of_2 == 0x0001) return 0;
    else if (power_of_2 == 0x0002) return 1;
    else if (power_of_2 == 0x0004) return 2;
    else if (power_of_2 == 0x0008) return 3;
    else if (power_of_2 == 0x0010) return 4;
    else if (power_of_2 == 0x0020) return 5;
    else if (power_of_2 == 0x0040) return 6;
    else if (power_of_2 == 0x0080) return 7;
    else if (power_of_2 == 0x0100) return 8;
    else if (power_of_2 == 0x0200) return 9;
    else if (power_of_2 == 0x0400) return 10;
    else if (power_of_2 == 0x0800) return 11;
    else if (power_of_2 == 0x1000) return 12;
    else if (power_of_2 == 0x2000) return 13;
    else if (power_of_2 == 0x4000) return 14;
    else                           return 15;
}

static uint16_t TRDB_D5M_shift_amount(uint16_t mask) {
    // (mask & -mask) clears all but the lowest bit of x
    return TRDB_D5M_log2(mask & (~mask + 1));
}

static uint16_t TRDB_D5M_READ(uint16_t full_reg_value, uint16_t mask) {
    return (full_reg_value & mask) >> TRDB_D5M_shift_amount(mask);
}

static uint16_t TRDB_D5M_WRITE(uint16_t full_reg_value, uint16_t mask, uint16_t write_value) {
    return (full_reg_value & (~mask)) | ((write_value << TRDB_D5M_shift_amount(mask)) & mask);
}

/////////////////////////////////////
// TRDB-D5M I2C READ/WRITE ADDRESS //
/////////////////////////////////////

#define TRDB_D5M_I2C_WRITE_ADDRESS                                                                        (0xba)
#define TRDB_D5M_I2C_READ_ADDRESS                                                                         (0xbb)

///////////////
// REGISTERS //
///////////////

// RO
#define TRDB_D5M_CHIP_VERSION_REG                                                                         (0x00)
#define TRDB_D5M_CHIP_VERSION_REG_DEFAULT                                                                 (0x1801)
#define TRDB_D5M_CHIP_VERSION_REG_PART_ID_MASK                                                            (0xff00)
#define TRDB_D5M_CHIP_VERSION_REG_ANALOG_REVISION_MASK                                                    (0x00f0)
#define TRDB_D5M_CHIP_VERSION_REG_DIGITAL_REVISION_MASK                                                   (0x000f)
#define TRDB_D5M_CHIP_VERSION_REG_PART_ID_READ(full_reg_value)                                            (TRDB_D5M_READ((full_reg_value), TRDB_D5M_CHIP_VERSION_REG_PART_ID_MASK))
#define TRDB_D5M_CHIP_VERSION_REG_ANALOG_REVISION_READ(full_reg_value)                                    (TRDB_D5M_READ((full_reg_value), TRDB_D5M_CHIP_VERSION_REG_ANALOG_REVISION_MASK))
#define TRDB_D5M_CHIP_VERSION_REG_DIGITAL_REVISION_READ(full_reg_value)                                   (TRDB_D5M_READ((full_reg_value), TRDB_D5M_CHIP_VERSION_REG_DIGITAL_REVISION_MASK))

// RW
#define TRDB_D5M_ROW_START_REG                                                                            (0x01)
#define TRDB_D5M_ROW_START_REG_DEFAULT                                                                    (0x0036)
#define TRDB_D5M_ROW_START_REG_MASK                                                                       (0xffff)
#define TRDB_D5M_ROW_START_REG_READ(full_reg_value)                                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_ROW_START_REG_MASK))
#define TRDB_D5M_ROW_START_REG_WRITE(full_reg_value, write_value)                                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_ROW_START_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_COLUMN_START_REG                                                                         (0x02)
#define TRDB_D5M_COLUMN_START_REG_DEFAULT                                                                 (0x0010)
#define TRDB_D5M_COLUMN_START_REG_MASK                                                                    (0xffff)
#define TRDB_D5M_COLUMN_START_REG_READ(full_reg_value)                                                    (TRDB_D5M_READ((full_reg_value), TRDB_D5M_COLUMN_START_REG_MASK))
#define TRDB_D5M_COLUMN_START_REG_WRITE(full_reg_value, write_value)                                      (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_COLUMN_START_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_ROW_SIZE_REG                                                                             (0x03)
#define TRDB_D5M_ROW_SIZE_REG_DEFAULT                                                                     (0x0797)
#define TRDB_D5M_ROW_SIZE_REG_MASK                                                                        (0xffff)
#define TRDB_D5M_ROW_SIZE_REG_READ(full_reg_value)                                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_ROW_SIZE_REG_MASK))
#define TRDB_D5M_ROW_SIZE_REG_WRITE(full_reg_value, write_value)                                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_ROW_SIZE_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_COLUMN_SIZE_REG                                                                          (0x04)
#define TRDB_D5M_COLUMN_SIZE_REG_DEFAULT                                                                  (0x0a1f)
#define TRDB_D5M_COLUMN_SIZE_REG_MASK                                                                     (0xffff)
#define TRDB_D5M_COLUMN_SIZE_REG_READ(full_reg_value)                                                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_COLUMN_SIZE_REG_MASK))
#define TRDB_D5M_COLUMN_SIZE_REG_WRITE(full_reg_value, write_value)                                       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_COLUMN_SIZE_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_HORIZONTAL_BLANK_REG                                                                     (0x05)
#define TRDB_D5M_HORIZONTAL_BLANK_REG_DEFAULT                                                             (0x0000)
#define TRDB_D5M_HORIZONTAL_BLANK_REG_MASK                                                                (0xffff)
#define TRDB_D5M_HORIZONTAL_BLANK_REG_READ(full_reg_value)                                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_HORIZONTAL_BLANK_REG_MASK))
#define TRDB_D5M_HORIZONTAL_BLANK_REG_WRITE(full_reg_value, write_value)                                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_HORIZONTAL_BLANK_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_VERTICAL_BLANK_REG                                                                       (0x06)
#define TRDB_D5M_VERTICAL_BLANK_REG_DEFAULT                                                               (0x0019)
#define TRDB_D5M_VERTICAL_BLANK_REG_MASK                                                                  (0xffff)
#define TRDB_D5M_VERTICAL_BLANK_REG_READ(full_reg_value)                                                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_VERTICAL_BLANK_REG_MASK))
#define TRDB_D5M_VERTICAL_BLANK_REG_WRITE(full_reg_value, write_value)                                    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_VERTICAL_BLANK_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_OUTPUT_CONTROL_REG                                                                       (0x07)
#define TRDB_D5M_OUTPUT_CONTROL_REG_DEFAULT                                                               (0x1f82)
#define TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_MASK                                                 (0x1c00)
#define TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_MASK                                                 (0x0380)
#define TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_MASK                                               (0x0004)
#define TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_MASK                                                      (0x0002)
#define TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_MASK                                              (0X0001)
#define TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_READ(full_reg_value)                                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_MASK))
#define TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_READ(full_reg_value)                                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_MASK))
#define TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_READ(full_reg_value)                               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_MASK))
#define TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_READ(full_reg_value)                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_MASK))
#define TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_READ(full_reg_value)                              (TRDB_D5M_READ((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_MASK))
#define TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_WRITE(full_reg_value, write_value)                   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_MASK, (write_value)))
#define TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_WRITE(full_reg_value, write_value)                   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_MASK, (write_value)))
#define TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_WRITE(full_reg_value, write_value)                 (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_MASK, (write_value)))
#define TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_WRITE(full_reg_value, write_value)                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_MASK, (write_value)))
#define TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_WRITE(full_reg_value, write_value)                (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_MASK, (write_value)))

// RW
#define TRDB_D5M_SHUTTER_WIDTH_UPPER_REG                                                                  (0x08)
#define TRDB_D5M_SHUTTER_WIDTH_UPPER_REG_DEFAULT                                                          (0x0000)
#define TRDB_D5M_SHUTTER_WIDTH_UPPER_REG_MASK                                                             (0xffff)
#define TRDB_D5M_SHUTTER_WIDTH_UPPER_REG_READ(full_reg_value)                                             (TRDB_D5M_READ((full_reg_value), TRDB_D5M_SHUTTER_WIDTH_UPPER_REG_MASK))
#define TRDB_D5M_SHUTTER_WIDTH_UPPER_REG_WRITE(full_reg_value, write_value)                               (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_SHUTTER_WIDTH_UPPER_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_SHUTTER_WIDTH_LOWER_REG                                                                  (0x09)
#define TRDB_D5M_SHUTTER_WIDTH_LOWER_REG_DEFAULT                                                          (0x0797)
#define TRDB_D5M_SHUTTER_WIDTH_LOWER_REG_MASK                                                             (0xffff)
#define TRDB_D5M_SHUTTER_WIDTH_LOWER_REG_READ(full_reg_value)                                             (TRDB_D5M_READ((full_reg_value), TRDB_D5M_SHUTTER_WIDTH_LOWER_REG_MASK))
#define TRDB_D5M_SHUTTER_WIDTH_LOWER_REG_WRITE(full_reg_value, write_value)                               (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_SHUTTER_WIDTH_LOWER_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG                                                                  (0x0a)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DEFAULT                                                          (0x0000)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_MASK                                          (0x8000)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_MASK                                           (0x0700)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_MASK                                          (0x007f)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_READ(full_reg_value)                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_MASK))
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_READ(full_reg_value)                           (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_MASK))
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_READ(full_reg_value)                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_MASK))
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_WRITE(full_reg_value, write_value)            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_MASK, (write_value)))
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_WRITE(full_reg_value, write_value)             (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_MASK, (write_value)))
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_WRITE(full_reg_value, write_value)            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_MASK, (write_value)))

// RW
#define TRDB_D5M_RESTART_REG                                                                              (0x0b)
#define TRDB_D5M_RESTART_REG_DEFAULT                                                                      (0x0000)
#define TRDB_D5M_RESTART_REG_TRIGGER_MASK                                                                 (0x0004)
#define TRDB_D5M_RESTART_REG_PAUSE_RESTART_MASK                                                           (0x0002)
#define TRDB_D5M_RESTART_REG_RESTART_MASK                                                                 (0x0001)
#define TRDB_D5M_RESTART_REG_TRIGGER_READ(full_reg_value)                                                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RESTART_REG_TRIGGER_MASK))
#define TRDB_D5M_RESTART_REG_PAUSE_RESTART_READ(full_reg_value)                                           (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RESTART_REG_PAUSE_RESTART_MASK))
#define TRDB_D5M_RESTART_REG_RESTART_READ(full_reg_value)                                                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RESTART_REG_RESTART_MASK))
#define TRDB_D5M_RESTART_REG_TRIGGER_WRITE(full_reg_value, write_value)                                   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RESTART_REG_TRIGGER_MASK, (write_value)))
#define TRDB_D5M_RESTART_REG_PAUSE_RESTART_WRITE(full_reg_value, write_value)                             (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RESTART_REG_PAUSE_RESTART_MASK, (write_value)))
#define TRDB_D5M_RESTART_REG_RESTART_WRITE(full_reg_value, write_value)                                   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RESTART_REG_RESTART_MASK, (write_value)))

// RW
#define TRDB_D5M_SHUTTER_DELAY_REG                                                                        (0x0c)
#define TRDB_D5M_SHUTTER_DELAY_REG_DEFAULT                                                                (0x0000)
#define TRDB_D5M_SHUTTER_DELAY_REG_MASK                                                                   (0xffff)
#define TRDB_D5M_SHUTTER_DELAY_REG_READ(full_reg_value)                                                   (TRDB_D5M_READ((full_reg_value), TRDB_D5M_SHUTTER_DELAY_REG_MASK))
#define TRDB_D5M_SHUTTER_DELAY_REG_WRITE(full_reg_value, write_value)                                     (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_SHUTTER_DELAY_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_RESET_REG                                                                                (0x0d)
#define TRDB_D5M_RESET_REG_DEFAULT                                                                        (0x0000)
#define TRDB_D5M_RESET_REG_MASK                                                                           (0xffff)
#define TRDB_D5M_RESET_REG_READ(full_reg_value)                                                           (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RESET_REG_MASK))
#define TRDB_D5M_RESET_REG_WRITE(full_reg_value, write_value)                                             (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RESET_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_PLL_CONTROL_REG                                                                          (0x10)
#define TRDB_D5M_PLL_CONTROL_REG_DEFAULT                                                                  (0x0050)
#define TRDB_D5M_PLL_CONTROL_REG_USE_PLL_MASK                                                             (0x0002)
#define TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_MASK                                                           (0x0001)
#define TRDB_D5M_PLL_CONTROL_REG_USE_PLL_READ(full_reg_value)                                             (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PLL_CONTROL_REG_USE_PLL_MASK))
#define TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_READ(full_reg_value)                                           (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_MASK))
#define TRDB_D5M_PLL_CONTROL_REG_USE_PLL_WRITE(full_reg_value, write_value)                               (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PLL_CONTROL_REG_USE_PLL_MASK, (write_value)))
#define TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_WRITE(full_reg_value, write_value)                             (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_MASK, (write_value)))

// RW
#define TRDB_D5M_PLL_CONFIG_1_REG                                                                         (0x11)
#define TRDB_D5M_PLL_CONFIG_1_REG_DEFAULT                                                                 (0x6404)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_MASK                                                       (0xff00)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_MASK                                                      (0x003f)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_MASK))
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_READ(full_reg_value)                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_MASK))
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_MASK, (write_value)))
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_WRITE(full_reg_value, write_value)                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_MASK, (write_value)))

// RW
#define TRDB_D5M_PLL_CONFIG_2_REG                                                                         (0x12)
#define TRDB_D5M_PLL_CONFIG_2_REG_DEFAULT                                                                 (0x0000)
#define TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_MASK                                                     (0x001f)
#define TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_READ(full_reg_value)                                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_MASK))
#define TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_WRITE(full_reg_value, write_value)                       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_MASK, (write_value)))

// RW
#define TRDB_D5M_READ_MODE_1_REG                                                                          (0x1e)
#define TRDB_D5M_READ_MODE_1_REG_DEFAULT                                                                  (0x4006)
#define TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_MASK                                                      (0x0800)
#define TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_MASK                                               (0x0400)
#define TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_MASK                                                      (0x0200)
#define TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_MASK                                                            (0x0100)
#define TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_MASK                                                        (0x0080)
#define TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_MASK                                                       (0x0040)
#define TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_MASK                                                       (0x0020)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_MASK                                                       (0x0010)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_MASK                                                        (0x000c)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_END_MASK                                                          (0x0003)
#define TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_READ(full_reg_value)                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_MASK))
#define TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_READ(full_reg_value)                               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_MASK))
#define TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_READ(full_reg_value)                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_MASK))
#define TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_READ(full_reg_value)                                            (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_MASK))
#define TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_MASK))
#define TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_MASK))
#define TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_MASK))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_MASK))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_STROBE_START_MASK))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_END_READ(full_reg_value)                                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_1_REG_STROBE_END_MASK))
#define TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_WRITE(full_reg_value, write_value)                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_WRITE(full_reg_value, write_value)                 (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_WRITE(full_reg_value, write_value)                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_WRITE(full_reg_value, write_value)                              (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_WRITE(full_reg_value, write_value)                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_WRITE(full_reg_value, write_value)                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_STROBE_START_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_END_WRITE(full_reg_value, write_value)                            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_1_REG_STROBE_END_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_FIRST_TRIGGER                                               (0)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_START_OF_SIMULTANEOUS_EXPOSURE                              (1)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_SHUTTER_WIDTH                                               (2)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_SECOND_TRIGGER                                              (3)

// RW
#define TRDB_D5M_READ_MODE_2_REG                                                                          (0x20)
#define TRDB_D5M_READ_MODE_2_REG_DEFAULT                                                                  (0x0040)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_MASK                                                          (0x8000)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_MASK                                                       (0x4000)
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_MASK                                                   (0x1000)
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_MASK                                                      (0x0800)
#define TRDB_D5M_READ_MODE_2_REG_ROW_BLC_MASK                                                             (0x0040)
#define TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_MASK                                                          (0x0020)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_READ(full_reg_value)                                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_MASK))
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_MASK))
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_READ(full_reg_value)                                   (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_MASK))
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_READ(full_reg_value)                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_MASK))
#define TRDB_D5M_READ_MODE_2_REG_ROW_BLC_READ(full_reg_value)                                             (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_2_REG_ROW_BLC_MASK))
#define TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_READ(full_reg_value)                                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_MASK))
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_WRITE(full_reg_value, write_value)                            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_WRITE(full_reg_value, write_value)                     (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_WRITE(full_reg_value, write_value)                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_2_REG_ROW_BLC_WRITE(full_reg_value, write_value)                               (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_2_REG_ROW_BLC_MASK, (write_value)))
#define TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_WRITE(full_reg_value, write_value)                            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_MASK, (write_value)))

// RW
#define TRDB_D5M_ROW_ADDRESS_MODE_REG                                                                     (0x22)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_DEFAULT                                                             (0x0000)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_MASK                                                        (0x0030)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_MASK                                                       (0x0007)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_MASK))
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_MASK))
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_WRITE(full_reg_value, write_value)                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_MASK, (write_value)))
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_MASK, (write_value)))

// RW
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG                                                                  (0x23)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_DEFAULT                                                          (0x0000)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_MASK                                                  (0x0030)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_MASK                                                 (0x0007)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_READ(full_reg_value)                                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_MASK))
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_READ(full_reg_value)                                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_MASK))
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_WRITE(full_reg_value, write_value)                    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_MASK, (write_value)))
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_WRITE(full_reg_value, write_value)                   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_MASK, (write_value)))

// RW
#define TRDB_D5M_GREEN_1_GAIN_REG                                                                         (0x2b)
#define TRDB_D5M_GREEN_1_GAIN_REG_DEFAULT                                                                 (0x0008)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_MASK                                               (0x7f00)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_MASK                                          (0x0040)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_MASK                                                (0x003f)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_READ(full_reg_value)                               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_MASK))
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_READ(full_reg_value)                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_MASK))
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_READ(full_reg_value)                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_MASK))
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_WRITE(full_reg_value, write_value)                 (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_MASK, (write_value)))
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_WRITE(full_reg_value, write_value)            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_MASK, (write_value)))
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_WRITE(full_reg_value, write_value)                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_MASK, (write_value)))

// RW
#define TRDB_D5M_BLUE_GAIN_REG                                                                            (0x2c)
#define TRDB_D5M_BLUE_GAIN_REG_DEFAULT                                                                    (0x0008)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_MASK                                                     (0x7f00)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_MASK                                                (0x0040)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_MASK                                                      (0x003f)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_READ(full_reg_value)                                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_MASK))
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_READ(full_reg_value)                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_MASK))
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_READ(full_reg_value)                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_MASK))
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_WRITE(full_reg_value, write_value)                       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_MASK, (write_value)))
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_WRITE(full_reg_value, write_value)                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_MASK, (write_value)))
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_WRITE(full_reg_value, write_value)                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_MASK, (write_value)))

// RW
#define TRDB_D5M_RED_GAIN_REG                                                                             (0x2d)
#define TRDB_D5M_RED_GAIN_REG_DEFAULT                                                                     (0x0008)
#define TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_MASK                                                       (0x7f00)
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_MASK                                                  (0x0040)
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_MASK                                                        (0x003f)
#define TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_MASK))
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_READ(full_reg_value)                                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_MASK))
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_MASK))
#define TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_MASK, (write_value)))
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_WRITE(full_reg_value, write_value)                    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_MASK, (write_value)))
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_WRITE(full_reg_value, write_value)                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_MASK, (write_value)))

// RW
#define TRDB_D5M_GREEN_2_GAIN_REG                                                                         (0x2e)
#define TRDB_D5M_GREEN_2_GAIN_REG_DEFAULT                                                                 (0x0008)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_MASK                                               (0x7f00)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_MASK                                          (0x0040)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_MASK                                                (0x003f)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_READ(full_reg_value)                               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_MASK))
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_READ(full_reg_value)                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_MASK))
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_READ(full_reg_value)                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_MASK))
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_WRITE(full_reg_value, write_value)                 (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_MASK, (write_value)))
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_WRITE(full_reg_value, write_value)            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_MASK, (write_value)))
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_WRITE(full_reg_value, write_value)                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_MASK, (write_value)))

// WO
#define TRDB_D5M_GLOBAL_GAIN_REG                                                                          (0x35)
#define TRDB_D5M_GLOBAL_GAIN_REG_DEFAULT                                                                  (0x0008)
#define TRDB_D5M_GLOBAL_GAIN_REG_MASK                                                                     (0xffff)
#define TRDB_D5M_GLOBAL_GAIN_REG_READ(full_reg_value)                                                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GLOBAL_GAIN_REG_MASK))
#define TRDB_D5M_GLOBAL_GAIN_REG_WRITE(full_reg_value, write_value)                                       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GLOBAL_GAIN_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_ROW_BLACK_TARGET_REG                                                                     (0x49)
#define TRDB_D5M_ROW_BLACK_TARGET_REG_DEFAULT                                                             (0x00a8)
#define TRDB_D5M_ROW_BLACK_TARGET_REG_MASK                                                                (0xffff)
#define TRDB_D5M_ROW_BLACK_TARGET_REG_READ(full_reg_value)                                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_ROW_BLACK_TARGET_REG_MASK))
#define TRDB_D5M_ROW_BLACK_TARGET_REG_WRITE(full_reg_value, write_value)                                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_ROW_BLACK_TARGET_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG                                                             (0x4b)
#define TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG_DEFAULT                                                     (0x0028)
#define TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG_MASK                                                        (0xffff)
#define TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG_MASK))
#define TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG_WRITE(full_reg_value, write_value)                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_BLC_SAMPLE_SIZE_REG                                                                      (0x5b)
#define TRDB_D5M_BLC_SAMPLE_SIZE_REG_DEFAULT                                                              (0x0001)
#define TRDB_D5M_BLC_SAMPLE_SIZE_REG_MASK                                                                 (0xffff)
#define TRDB_D5M_BLC_SAMPLE_SIZE_REG_READ(full_reg_value)                                                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_SAMPLE_SIZE_REG_MASK))
#define TRDB_D5M_BLC_SAMPLE_SIZE_REG_WRITE(full_reg_value, write_value)                                   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_SAMPLE_SIZE_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_BLC_TUNE_1_REG                                                                           (0x5c)
#define TRDB_D5M_BLC_TUNE_1_REG_DEFAULT                                                                   (0x005a)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_MASK                                                    (0x0f00)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_MASK                                                (0x00ff)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_READ(full_reg_value)                                    (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_MASK))
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_READ(full_reg_value)                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_MASK))
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_WRITE(full_reg_value, write_value)                      (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_MASK, (write_value)))
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_WRITE(full_reg_value, write_value)                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_MASK, (write_value)))

// RW
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG                                                                 (0x5d)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_DEFAULT                                                         (0x2d13)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_MASK                                   (0x7f00)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_MASK                                    (0x007f)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_READ(full_reg_value)                   (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_MASK))
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_READ(full_reg_value)                    (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_MASK))
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_WRITE(full_reg_value, write_value)     (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_MASK, (write_value)))
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_WRITE(full_reg_value, write_value)      (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_MASK, (write_value)))

// RW
#define TRDB_D5M_BLC_TUNE_2_REG                                                                           (0x5e)
#define TRDB_D5M_BLC_TUNE_2_REG_DEFAULT                                                                   (0x41ff)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_MASK                                                        (0x7000)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_MASK                                                       (0x01ff)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_MASK))
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_READ(full_reg_value)                                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_MASK))
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_WRITE(full_reg_value, write_value)                          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_MASK, (write_value)))
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_WRITE(full_reg_value, write_value)                         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_MASK, (write_value)))

// RW
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG                                                                (0x5f)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_DEFAULT                                                        (0x231d)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_MASK                                 (0x7f00)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_MASK                                  (0x007f)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_READ(full_reg_value)                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_MASK))
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_READ(full_reg_value)                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_MASK))
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_WRITE(full_reg_value, write_value)   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_MASK, (write_value)))
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_WRITE(full_reg_value, write_value)    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_MASK, (write_value)))

// RW
#define TRDB_D5M_GREEN_1_OFFSET_REG                                                                       (0x60)
#define TRDB_D5M_GREEN_1_OFFSET_REG_DEFAULT                                                               (0x0020) // Reading from device returns 0x0015
#define TRDB_D5M_GREEN_1_OFFSET_REG_MASK                                                                  (0xffff)
#define TRDB_D5M_GREEN_1_OFFSET_REG_READ(full_reg_value)                                                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_1_OFFSET_REG_MASK))
#define TRDB_D5M_GREEN_1_OFFSET_REG_WRITE(full_reg_value, write_value)                                    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_1_OFFSET_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_GREEN_2_OFFSET_REG                                                                       (0x61)
#define TRDB_D5M_GREEN_2_OFFSET_REG_DEFAULT                                                               (0x0020) // Reading from device returns 0x0015
#define TRDB_D5M_GREEN_2_OFFSET_REG_MASK                                                                  (0xffff)
#define TRDB_D5M_GREEN_2_OFFSET_REG_READ(full_reg_value)                                                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_GREEN_2_OFFSET_REG_MASK))
#define TRDB_D5M_GREEN_2_OFFSET_REG_WRITE(full_reg_value, write_value)                                    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_GREEN_2_OFFSET_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG                                                              (0x62)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DEFAULT                                                      (0x0000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_MASK                                     (0x8000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_MASK                                  (0x4000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_MASK                               (0x2000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_MASK                                 (0x1000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_MASK                                   (0x0800)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_MASK                                     (0x0002)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_MASK                                              (0x0001)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_READ(full_reg_value)                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_READ(full_reg_value)                  (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_READ(full_reg_value)               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_READ(full_reg_value)                 (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_READ(full_reg_value)                   (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_READ(full_reg_value)                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_READ(full_reg_value)                              (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_MASK))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_WRITE(full_reg_value, write_value)       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_MASK, (write_value)))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_WRITE(full_reg_value, write_value)    (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_MASK, (write_value)))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_WRITE(full_reg_value, write_value) (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_MASK, (write_value)))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_WRITE(full_reg_value, write_value)   (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_MASK, (write_value)))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_WRITE(full_reg_value, write_value)     (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_MASK, (write_value)))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_WRITE(full_reg_value, write_value)       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_MASK, (write_value)))
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_WRITE(full_reg_value, write_value)                (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_MASK, (write_value)))

// RW
#define TRDB_D5M_RED_OFFSET_REG                                                                           (0x63)
#define TRDB_D5M_RED_OFFSET_REG_DEFAULT                                                                   (0x0020) // Reading from device returns 0x0027
#define TRDB_D5M_RED_OFFSET_REG_MASK                                                                      (0xffff)
#define TRDB_D5M_RED_OFFSET_REG_READ(full_reg_value)                                                      (TRDB_D5M_READ((full_reg_value), TRDB_D5M_RED_OFFSET_REG_MASK))
#define TRDB_D5M_RED_OFFSET_REG_WRITE(full_reg_value, write_value)                                        (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_RED_OFFSET_REG_MASK, (write_value)))

// RW
#define TRDB_D5M_BLUE_OFFSET_REG                                                                          (0x64)
#define TRDB_D5M_BLUE_OFFSET_REG_DEFAULT                                                                  (0x0020) // Reading from device returns 0x0027
#define TRDB_D5M_BLUE_OFFSET_REG_MASK                                                                     (0xffff)
#define TRDB_D5M_BLUE_OFFSET_REG_READ(full_reg_value)                                                     (TRDB_D5M_READ((full_reg_value), TRDB_D5M_BLUE_OFFSET_REG_MASK))
#define TRDB_D5M_BLUE_OFFSET_REG_WRITE(full_reg_value, write_value)                                       (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_BLUE_OFFSET_REG_MASK, (write_value)))

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG                                                                 (0xa0)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_DEFAULT                                                         (0x0000)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MASK                                       (0x0078)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_MASK                                        (0x0001)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_READ(full_reg_value)                       (TRDB_D5M_READ((full_reg_value), TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MASK))
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_READ(full_reg_value)                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_MASK))
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_WRITE(full_reg_value, write_value)         (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MASK, (write_value)))
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_WRITE(full_reg_value, write_value)          (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_MASK, (write_value)))
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_COLOR_FIELD                                (0x0000)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_HORIZONTAL_GRADIENT                        (0x0008)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_VERTICAL_GRADIENT                          (0x0010)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_DIAGONAL                                   (0x0018)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_CLASSIC                                    (0x0020)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MARCHING_1S                                (0x0028)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MONOCHROME_HORIZONTAL_BARS                 (0x0030)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MONOCHROME_VERTICAL_BARS                   (0x0038)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_VERTICAL_COLOR_BARS                        (0x0040)

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_GREEN_REG                                                                   (0xa1)
#define TRDB_D5M_TEST_PATTERN_GREEN_REG_DEFAULT                                                           (0x0000)
#define TRDB_D5M_TEST_PATTERN_GREEN_REG_MASK                                                              (0xffff)
#define TRDB_D5M_TEST_PATTERN_GREEN_REG_READ(full_reg_value)                                              (TRDB_D5M_READ((full_reg_value), TRDB_D5M_TEST_PATTERN_GREEN_REG_MASK))
#define TRDB_D5M_TEST_PATTERN_GREEN_REG_WRITE(full_reg_value, write_value)                                (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_TEST_PATTERN_GREEN_REG_MASK, (write_value)))

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_RED_REG                                                                     (0xa2)
#define TRDB_D5M_TEST_PATTERN_RED_REG_DEFAULT                                                             (0x0000)
#define TRDB_D5M_TEST_PATTERN_RED_REG_MASK                                                                (0xffff)
#define TRDB_D5M_TEST_PATTERN_RED_REG_READ(full_reg_value)                                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_TEST_PATTERN_RED_REG_MASK))
#define TRDB_D5M_TEST_PATTERN_RED_REG_WRITE(full_reg_value, write_value)                                  (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_TEST_PATTERN_RED_REG_MASK, (write_value)))

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_BLUE_REG                                                                    (0xa3)
#define TRDB_D5M_TEST_PATTERN_BLUE_REG_DEFAULT                                                            (0x0000)
#define TRDB_D5M_TEST_PATTERN_BLUE_REG_MASK                                                               (0xffff)
#define TRDB_D5M_TEST_PATTERN_BLUE_REG_READ(full_reg_value)                                               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_TEST_PATTERN_BLUE_REG_MASK))
#define TRDB_D5M_TEST_PATTERN_BLUE_REG_WRITE(full_reg_value, write_value)                                 (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_TEST_PATTERN_BLUE_REG_MASK, (write_value)))

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG                                                               (0xa4)
#define TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG_DEFAULT                                                       (0x0000)
#define TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG_MASK                                                          (0xffff)
#define TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG_READ(full_reg_value)                                          (TRDB_D5M_READ((full_reg_value), TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG_MASK))
#define TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG_WRITE(full_reg_value, write_value)                            (TRDB_D5M_WRITE((full_reg_value), TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG_MASK, (write_value)))

// RO
#define TRDB_D5M_CHIP_VERSION_ALT_REG                                                                     (0xff)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_DEFAULT                                                             (0x1801) // Doesn't make sense, since the hardware manual says that this register is a mirror of TRDB_D5M_CHIP_VERSION_REG, which also has a default value of 0x1801
#define TRDB_D5M_CHIP_VERSION_ALT_REG_DIGITAL_REVISION_MASK                                               (0xf000)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_ANALOG_REVISION_MASK                                                (0x0f00)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_PART_ID_MASK                                                        (0x00ff)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_DIGITAL_REVISION_READ(full_reg_value)                               (TRDB_D5M_READ((full_reg_value), TRDB_D5M_CHIP_VERSION_ALT_REG_DIGITAL_REVISION_MASK))
#define TRDB_D5M_CHIP_VERSION_ALT_REG_ANALOG_REVISION_READ(full_reg_value)                                (TRDB_D5M_READ((full_reg_value), TRDB_D5M_CHIP_VERSION_ALT_REG_ANALOG_REVISION_MASK))
#define TRDB_D5M_CHIP_VERSION_ALT_REG_PART_ID_READ(full_reg_value)                                        (TRDB_D5M_READ((full_reg_value), TRDB_D5M_CHIP_VERSION_ALT_REG_PART_ID_MASK))

#endif /* __TRDB_D5M_REGS_H__ */
