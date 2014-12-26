#define TRDB_D5M_I2C_WRITE_ADDRESS                                                          (0xba)
#define TRDB_D5M_I2C_READ_ADDRESS                                                           (0xbb)

///////////////
// REGISTERS //
///////////////

// RO
#define TRDB_D5M_CHIP_VERSION_REG                                                           (0X00)
#define TRDB_D5M_CHIP_VERSION_REG_PART_ID_MASK                                              (0xff00)
#define TRDB_D5M_CHIP_VERSION_REG_ANALOG_REVISION_MASK                                      (0x00f0)
#define TRDB_D5M_CHIP_VERSION_REG_DIGITAL_REVISION_MASK                                     (0x000f)
#define TRDB_D5M_CHIP_VERSION_REG_PART_ID_READ(full_reg_value)                              (((full_reg_value) & TRDB_D5M_CHIP_VERSION_REG_PART_ID_MASK) >> 8)
#define TRDB_D5M_CHIP_VERSION_REG_ANALOG_REVISION_READ(full_reg_value)                      (((full_reg_value) & TRDB_D5M_CHIP_VERSION_REG_ANALOG_REVISION_MASK) >> 4)
#define TRDB_D5M_CHIP_VERSION_REG_DIGITAL_REVISION_READ(full_reg_value)                     (((full_reg_value) & TRDB_D5M_CHIP_VERSION_REG_DIGITAL_REVISION_MASK) >> 0)

// RW
#define TRDB_D5M_ROW_START_REG                                                              (0x01)

// RW
#define TRDB_D5M_COLUMN_START_REG                                                           (0x02)

// RW
#define TRDB_D5M_ROW_SIZE_REG                                                               (0x03)

// RW
#define TRDB_D5M_COLUMN_SIZE_REG                                                            (0x04)

// RW
#define TRDB_D5M_HORIZONTAL_BLANK_REG                                                       (0x05)

// RW
#define TRDB_D5M_VERTICAL_BLANK_REG                                                         (0x06)

// RW
#define TRDB_D5M_OUTPUT_CONTROL_REG                                                         (0x07)
#define TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_MASK                                   (0x1c00)
#define TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_MASK                                   (0x0380)
#define TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_MASK                                 (0x0004)
#define TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_MASK                                        (0x0002)
#define TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_MASK                                (0X0001)
#define TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_READ(full_reg_value)                   (((full_reg_value) & TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_MASK) >> 10)
#define TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_READ(full_reg_value)                   (((full_reg_value) & TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_MASK) >> 7)
#define TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_READ(full_reg_value)                 (((full_reg_value) & TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_MASK) >> 2)
#define TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_READ(full_reg_value)                        (((full_reg_value) & TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_MASK) >> 1)
#define TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_READ(full_reg_value)                (((full_reg_value) & TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_MASK) >> 0)
#define TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_WRITE(value)                           (((value) << 10) & TRDB_D5M_OUTPUT_CONTROL_REG_OUTPUT_SLEW_RATE_MASK)
#define TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_WRITE(value)                           (((value) << 7) & TRDB_D5M_OUTPUT_CONTROL_REG_PIXCLK_SLEW_RATE_MASK)
#define TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_WRITE(value)                         (((value) << 2) & TRDB_D5M_OUTPUT_CONTROL_REG_FIFO_PARALLEL_DATA_MASK)
#define TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_WRITE(value)                                (((value) << 1) & TRDB_D5M_OUTPUT_CONTROL_REG_CHIP_ENABLE_MASK)
#define TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_WRITE(value)                        (((value) << 0) & TRDB_D5M_OUTPUT_CONTROL_REG_SYNCHRONIZE_CHANGES_MASK)

// RW
#define TRDB_D5M_SHUTTER_WIDTH_UPPER_REG                                                    (0x08)

// RW
#define TRDB_D5M_SHUTTER_WIDTH_LOWER_REG                                                    (0x09)

// RW
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG                                                    (0x0a)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_MASK                            (0x8000)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_MASK                             (0x0700)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_MASK                            (0x007f)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_READ(full_reg_value)            (((full_reg_value) & TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_INVERT_PIXEL_CLOCK_MASK) >> 15)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_READ(full_reg_value)             (((full_reg_value) & TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_SHIFT_PIXEL_CLOCK_MASK) >> 8)
#define TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_READ(full_reg_value)            (((full_reg_value) & TRDB_D5M_PIXEL_CLOCK_CONTROL_REG_DIVIDE_PIXEL_CLOCK_MASK) >> 0)

// RW
#define TRDB_D5M_RESTART_REG                                                                (0x0b)
#define TRDB_D5M_RESTART_REG_TRIGGER_MASK                                                   (0x0004)
#define TRDB_D5M_RESTART_REG_PAUSE_RESTART_MASK                                             (0x0002)
#define TRDB_D5M_RESTART_REG_RESTART_MASK                                                   (0x0001)
#define TRDB_D5M_RESTART_REG_TRIGGER_READ(full_reg_value)                                   (((full_reg_value) & TRDB_D5M_RESTART_REG_TRIGGER_MASK) >> 2)
#define TRDB_D5M_RESTART_REG_PAUSE_RESTART_READ(full_reg_value)                             (((full_reg_value) & TRDB_D5M_RESTART_REG_PAUSE_RESTART_MASK) >> 1)
#define TRDB_D5M_RESTART_REG_RESTART_READ(full_reg_value)                                   (((full_reg_value) & TRDB_D5M_RESTART_REG_RESTART_MASK) >> 0)

// RW
#define TRDB_D5M_SHUTTER_DELAY_REG                                                          (0x0c)

// RW
#define TRDB_D5M_RESET_REG                                                                  (0x0d)

// RW
#define TRDB_D5M_PLL_CONTROL_REG                                                            (0x10)
#define TRDB_D5M_PLL_CONTROL_REG_USE_PLL_MASK                                               (0x0002)
#define TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_MASK                                             (0x0001)
#define TRDB_D5M_PLL_CONTROL_REG_USE_PLL_READ(full_reg_value)                               (((full_reg_value) & TRDB_D5M_PLL_CONTROL_REG_USE_PLL_MASK) >> 1)
#define TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_READ(full_reg_value)                             (((full_reg_value) & TRDB_D5M_PLL_CONTROL_REG_POWER_PLL_MASK) >> 0)

// RW
#define TRDB_D5M_PLL_CONFIG_1_REG                                                           (0x11)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_MASK                                         (0xff00)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_MASK                                        (0x003f)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_PLL_CONFIG_1_REG_PLL_M_FACTOR_MASK) >> 8)
#define TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_READ(full_reg_value)                        (((full_reg_value) & TRDB_D5M_PLL_CONFIG_1_REG_PLL_N_DIVIDER_MASK) >> 0)

// RW
#define TRDB_D5M_PLL_CONFIG_2_REG                                                           (0x12)
#define TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_MASK                                       (0x001f)
#define TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_READ(full_reg_value)                       (((full_reg_value) & TRDB_D5M_PLL_CONFIG_2_REG_PLL_P1_DIVIDER_MASK) >> 0)

// RW
#define TRDB_D5M_READ_MODE_1_REG                                                            (0x1e)
#define TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_MASK                                        (0x0800)
#define TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_MASK                                 (0x0400)
#define TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_MASK                                        (0x0200)
#define TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_MASK                                              (0x0100)
#define TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_MASK                                          (0x0080)
#define TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_MASK                                         (0x0040)
#define TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_MASK                                         (0x0020)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_MASK                                         (0x0010)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_MASK                                          (0x000c)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_END_MASK                                            (0x0003)
#define TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_READ(full_reg_value)                        (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_XOR_LINE_VALID_MASK) >> 11)
#define TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_READ(full_reg_value)                 (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_CONTINUOUS_LINE_VALID_MASK) >> 10)
#define TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_READ(full_reg_value)                        (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_INVERT_TRIGGER_MASK) >> 9)
#define TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_READ(full_reg_value)                              (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_SNAPSHOT_MASK) >> 8)
#define TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_READ(full_reg_value)                          (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_GLOBAL_RESET_MASK) >> 7)
#define TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_BULB_EXPOSURE_MASK) >> 6)
#define TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_INVERT_STROBE_MASK) >> 5)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_STROBE_ENABLE_MASK) >> 4)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_START_READ(full_reg_value)                          (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_STROBE_START_MASK) >> 2)
#define TRDB_D5M_READ_MODE_1_REG_STROBE_END_READ(full_reg_value)                            (((full_reg_value) & TRDB_D5M_READ_MODE_1_REG_STROBE_END_MASK) >> 0)
// #define TRDB_D5M_READ_MODE_1_REG_STROBE_START_FIRST_TRIGGER                               (0)
// #define TRDB_D5M_READ_MODE_1_REG_STROBE_START_START_OF_SIMULTANEOUS_EXPOSURE              (1)
// #define TRDB_D5M_READ_MODE_1_REG_STROBE_START_SHUTTER_WIDTH                               (2)
// #define TRDB_D5M_READ_MODE_1_REG_STROBE_START_SECOND_TRIGGER                              (3)

// RW
#define TRDB_D5M_READ_MODE_2_REG                                                            (0x20)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_MASK                                            (0x8000)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_MASK                                         (0x4000)
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_MASK                                     (0x1000)
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_MASK                                        (0x0800)
#define TRDB_D5M_READ_MODE_2_REG_ROW_BLC_MASK                                               (0x0040)
#define TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_MASK                                            (0x0020)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_READ(full_reg_value)                            (((full_reg_value) & TRDB_D5M_READ_MODE_2_REG_MIRROR_ROW_MASK) >> 15)
#define TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_READ_MODE_2_REG_MIRROR_COLUMN_MASK) >> 14)
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_READ(full_reg_value)                     (((full_reg_value) & TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_COLUMNS_MASK) >> 12)
#define TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_READ(full_reg_value)                        (((full_reg_value) & TRDB_D5M_READ_MODE_2_REG_SHOW_DARK_ROWS_MASK) >> 11)
#define TRDB_D5M_READ_MODE_2_REG_ROW_BLC_READ(full_reg_value)                               (((full_reg_value) & TRDB_D5M_READ_MODE_2_REG_ROW_BLC_MASK) >> 6)
#define TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_READ(full_reg_value)                            (((full_reg_value) & TRDB_D5M_READ_MODE_2_REG_COLUMN_SUM_MASK) >> 5)

// RW
#define TRDB_D5M_ROW_ADDRESS_MODE_REG                                                       (0x22)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_MASK                                          (0x0030)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_MASK                                         (0x0007)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_READ(full_reg_value)                          (((full_reg_value) & TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_BIN_MASK) >> 4)
#define TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_ROW_ADDRESS_MODE_REG_ROW_SKIP_MASK) >> 0)

// RW
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG                                                    (0x23)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_MASK                                    (0x0030)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_MASK                                   (0x0007)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_READ(full_reg_value)                    (((full_reg_value) & TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_BIN_MASK) >> 4)
#define TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_READ(full_reg_value)                   (((full_reg_value) & TRDB_D5M_COLUMN_ADDRESS_MODE_REG_COLUMN_SKIP_MASK) >> 0)

// RW
#define TRDB_D5M_GREEN_1_GAIN_REG                                                           (0x2b)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_MASK                                 (0x7f00)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_MASK                            (0x0040)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_MASK                                  (0x003f)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_READ(full_reg_value)                 (((full_reg_value) & TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_DIGITAL_GAIN_MASK) >> 8)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_READ(full_reg_value)            (((full_reg_value) & TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_MULTIPLIER_MASK) >> 6)
#define TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_READ(full_reg_value)                  (((full_reg_value) & TRDB_D5M_GREEN_1_GAIN_REG_GREEN_1_ANALOG_GAIN_MASK) >> 0)

// RW
#define TRDB_D5M_BLUE_GAIN_REG                                                              (0x2c)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_MASK                                       (0x7f00)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_MASK                                  (0x0040)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_MASK                                        (0x003f)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_READ(full_reg_value)                       (((full_reg_value) & TRDB_D5M_BLUE_GAIN_REG_BLUE_DIGITAL_GAIN_MASK) >> 8)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_READ(full_reg_value)                  (((full_reg_value) & TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_MULTIPLIER_MASK) >> 6)
#define TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_READ(full_reg_value)                        (((full_reg_value) & TRDB_D5M_BLUE_GAIN_REG_BLUE_ANALOG_GAIN_MASK) >> 0)

// RW
#define TRDB_D5M_RED_GAIN_REG                                                               (0x2d)
#define TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_MASK                                         (0x7f00)
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_MASK                                    (0x0040)
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_MASK                                          (0x003f)
#define TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_RED_GAIN_REG_RED_DIGITAL_GAIN_MASK) >> 8)
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_READ(full_reg_value)                    (((full_reg_value) & TRDB_D5M_RED_GAIN_REG_RED_ANALOG_MULTIPLIER_MASK) >> 6)
#define TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_READ(full_reg_value)                          (((full_reg_value) & TRDB_D5M_RED_GAIN_REG_RED_ANALOG_GAIN_MASK) >> 0)

// RW
#define TRDB_D5M_GREEN_2_GAIN_REG                                                           (0x2e)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_MASK                                 (0x7f00)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_MASK                            (0x0040)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_MASK                                  (0x003f)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_READ(full_reg_value)                 (((full_reg_value) & TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_DIGITAL_GAIN_MASK) >> 8)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_READ(full_reg_value)            (((full_reg_value) & TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_MULTIPLIER_MASK) >> 6)
#define TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_READ(full_reg_value)                  (((full_reg_value) & TRDB_D5M_GREEN_2_GAIN_REG_GREEN_2_ANALOG_GAIN_MASK) >> 0)

// WO
#define TRDB_D5M_GLOBAL_GAIN_REG                                                            (0x35)

// RW
#define TRDB_D5M_ROW_BLACK_TARGET_REG                                                       (0x49)

// RW
#define TRDB_D5M_ROW_BLACK_DEFAULT_OFFSET_REG                                               (0x4b)

// RW
#define TRDB_D5M_BLC_SAMPLE_SIZE_REG                                                        (0x5b)

// RW
#define TRDB_D5M_BLC_TUNE_1_REG                                                             (0x5c)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_MASK                                      (0x0f00)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_MASK                                  (0x00ff)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_READ(full_reg_value)                      (((full_reg_value) & TRDB_D5M_BLC_TUNE_1_REG_BLC_DELTA_DAMPING_MASK) >> 8)
#define TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_READ(full_reg_value)                  (((full_reg_value) & TRDB_D5M_BLC_TUNE_1_REG_BLC_DAC_SETTLING_TIME_MASK) >> 0)

// RW
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG                                                   (0x5d)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_MASK                     (0x7f00)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_MASK                      (0x007f)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_READ(full_reg_value)     (((full_reg_value) & TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_HIGH_DELTA_THRESHOLD_MASK) >> 8)
#define TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_READ(full_reg_value)      (((full_reg_value) & TRDB_D5M_BLC_DELTA_THRESHOLDS_REG_BLC_LOW_DELTA_THRESHOLD_MASK) >> 0)

// RW
#define TRDB_D5M_BLC_TUNE_2_REG                                                             (0x5e)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_MASK                                          (0x7000)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_MASK                                         (0x01ff)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_READ(full_reg_value)                          (((full_reg_value) & TRDB_D5M_BLC_TUNE_2_REG_BLC_STEP_SIZE_MASK) >> 12)
#define TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_READ(full_reg_value)                         (((full_reg_value) & TRDB_D5M_BLC_TUNE_2_REG_BLC_MAX_ADJUST_MASK) >> 0)

// RW
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG                                                  (0x5f)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_MASK                   (0x7f00)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_MASK                    (0x007f)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_READ(full_reg_value)   (((full_reg_value) & TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_HIGH_TARGET_THRESHOLD_MASK) >> 8)
#define TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_READ(full_reg_value)    (((full_reg_value) & TRDB_D5M_BLC_TARGET_THRESHOLDS_REG_BLC_LOW_TARGET_THRESHOLD_MASK) >> 0)

// RW
#define TRDB_D5M_GREEN_1_OFFSET_REG                                                         (0x60)

// RW
#define TRDB_D5M_GREEN_2_OFFSET_REG                                                         (0x61)

// RW
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG                                                (0x62)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_MASK                       (0x8000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_MASK                    (0x4000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_MASK                 (0x2000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_MASK                   (0x1000)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_MASK                     (0x0800)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_MASK                       (0x0002)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_MASK                                (0x0001)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_READ(full_reg_value)       (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_FAST_SAMPLE_MASK) >> 15)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_READ(full_reg_value)    (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_GREEN_CALIBRATION_MASK) >> 14)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_READ(full_reg_value) (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_LOCK_RED_BLUE_CALIBRATION_MASK) >> 13)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_READ(full_reg_value)   (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_RECALCULATE_BLACK_LEVEL_MASK) >> 12)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_READ(full_reg_value)     (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_BINARY_SEARCH_MASK) >> 11)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_READ(full_reg_value)       (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_DISABLE_CALIBRATION_MASK) >> 1)
#define TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_READ(full_reg_value)                (((full_reg_value) & TRDB_D5M_BLACK_LEVEL_CALIBRATION_REG_MANUAL_BLC_MASK) >> 0)

// RW
#define TRDB_D5M_RED_OFFSET_REG                                                             (0x63)

// RW
#define TRDB_D5M_BLUE_OFFSET_REG                                                            (0x64)

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG                                                   (0xa0)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MASK                         (0x0078)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_MASK                          (0x0001)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_READ(full_reg_value)         (((full_reg_value) & TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MASK) >> 3)
#define TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_READ(full_reg_value)          (((full_reg_value) & TRDB_D5M_TEST_PATTERN_CONTROL_REG_ENABLE_TEST_PATTERN_MASK) >> 0)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_COLOR_FIELD                (0x0000)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_HORIZONTAL_GRADIENT        (0x0008)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_VERTICAL_GRADIENT          (0x0010)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_DIAGONAL                   (0x0018)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_CLASSIC                    (0x0020)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MARCHING_1S                (0x0028)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MONOCHROME_HORIZONTAL_BARS (0x0030)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_MONOCHROME_VERTICAL_BARS   (0x0038)
// #define TRDB_D5M_TEST_PATTERN_CONTROL_REG_TEST_PATTERN_CONTROL_VERTICAL_COLOR_BARS        (0x0040)

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_GREEN_REG                                                     (0xa1)

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_RED_REG                                                       (0xa2)

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_BLUE_REG                                                      (0xa3)

// RW / WO not specified in datasheet
#define TRDB_D5M_TEST_PATTERN_BAR_WIDTH_REG                                                 (0xa4)

// RO
#define TRDB_D5M_CHIP_VERSION_ALT_REG                                                       (0xff)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_DIGITAL_REVISION_MASK                                 (0xf000)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_ANALOG_REVISION_MASK                                  (0x0f00)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_PART_ID_MASK                                          (0x00ff)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_DIGITAL_REVISION_READ(full_reg_value)                 (((full_reg_value) & TRDB_D5M_CHIP_VERSION_ALT_REG_DIGITAL_REVISION_MASK) >> 12)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_ANALOG_REVISION_READ(full_reg_value)                  (((full_reg_value) & TRDB_D5M_CHIP_VERSION_ALT_REG_ANALOG_REVISION_MASK) >> 8)
#define TRDB_D5M_CHIP_VERSION_ALT_REG_PART_ID_READ(full_reg_value)                          (((full_reg_value) & TRDB_D5M_CHIP_VERSION_ALT_REG_PART_ID_MASK) >> 0)
