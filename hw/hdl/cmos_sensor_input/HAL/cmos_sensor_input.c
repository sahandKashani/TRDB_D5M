#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#include "cmos_sensor_input.h"
#include "cmos_sensor_input_regs.h"

/*******************************************************************************
 *  Private API
 ******************************************************************************/
static uint32_t ceil_div(uint32_t x, uint32_t y);
static uint32_t read_config_reg_irq_flag(cmos_sensor_input_dev *dev);
static uint32_t read_config_reg_debayer_pattern_flag(cmos_sensor_input_dev *dev);
static void write_config_reg_irq_flag(cmos_sensor_input_dev *dev, bool irq_enabled);
static void write_config_reg_debayer_pattern_flag(cmos_sensor_input_dev *dev, cmos_sensor_input_debayer_pattern pattern);
static void write_command_reg_get_frame_info(cmos_sensor_input_dev *dev);
static void write_command_reg_snapshot(cmos_sensor_input_dev *dev);
static void write_command_reg_irq_ack(cmos_sensor_input_dev *dev);
static void write_command_reg_stop_and_reset(cmos_sensor_input_dev *dev);
static uint32_t read_status_reg_state_flag(cmos_sensor_input_dev *dev);
static uint32_t read_status_reg_fifo_ovfl_flag(cmos_sensor_input_dev *dev);
static uint32_t read_status_reg_fifo_fill_level_flag(cmos_sensor_input_dev *dev);
static uint32_t read_frame_info_reg_frame_width_flag(cmos_sensor_input_dev *dev);
static uint32_t read_frame_info_reg_frame_height_flag(cmos_sensor_input_dev *dev);

/*
 * ceil_div
 *
 * Computes ceil(x / y) for the 2 supplied 32-bit unsigned numbers. Must be sure
 * that (x != 0) before calling this function.
 *
 * Full credit for this code comes from the link below:
 *   http://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
 */
static uint32_t ceil_div(uint32_t x, uint32_t y) {
    return 1 + ((x - 1) / y);
}

/*
 * read_config_reg_irq_flag
 *
 * Returns CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE if irq generation is disabled.
 * Returns CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE if irq generation is enabled.
 */
static uint32_t read_config_reg_irq_flag(cmos_sensor_input_dev *dev) {
    uint32_t config_reg = CMOS_SENSOR_INPUT_RD_CONFIG(dev->base);
    uint32_t irq_flag = (config_reg & CMOS_SENSOR_INPUT_CONFIG_IRQ_MASK) >> CMOS_SENSOR_INPUT_CONFIG_IRQ_OFST;
    return irq_flag;
}

/*
 * read_config_reg_debayer_pattern_flag
 *
 * Returns one of the following values depending on the flag contents:
 *  - CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB
 *  - CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BGGR
 *  - CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GRBG
 *  - CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GBRG
 */
static uint32_t read_config_reg_debayer_pattern_flag(cmos_sensor_input_dev *dev) {
    uint32_t config_reg = CMOS_SENSOR_INPUT_RD_CONFIG(dev->base);
    uint32_t debayer_pattern_flag = (config_reg & CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_MASK) >> CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_OFST;
    return debayer_pattern_flag;
}

/*
 * write_config_reg_irq_flag
 *
 * Enables interrupt generation if irq_enable is true.
 * Disables interrupt generation if irq_enable is false.
 */
static void write_config_reg_irq_flag(cmos_sensor_input_dev *dev, bool irq_enabled) {
    uint32_t config_reg = CMOS_SENSOR_INPUT_RD_CONFIG(dev->base);
    config_reg &= ~CMOS_SENSOR_INPUT_CONFIG_IRQ_MASK;

    if (irq_enabled) {
        config_reg |= CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE_MASK;
    } else {
        config_reg |= CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE_MASK;
    }

    CMOS_SENSOR_INPUT_WR_CONFIG(dev->base, config_reg);
}

/*
 * write_config_reg_debayer_pattern_flag
 *
 * Sets the debayering pattern to be used.
 */
static void write_config_reg_debayer_pattern_flag(cmos_sensor_input_dev *dev, cmos_sensor_input_debayer_pattern pattern) {
    uint32_t config_reg = CMOS_SENSOR_INPUT_RD_CONFIG(dev->base);
    config_reg &= ~CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_MASK;

    if (pattern == RGGB) {
        config_reg |= CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB_MASK;
    } else if (pattern == BGGR) {
        config_reg |= CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BGGR_MASK;
    } else if (pattern == GRBG) {
        config_reg |= CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GRBG_MASK;
    } else if (pattern == GBRG) {
        config_reg |= CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GBRG_MASK;
    }

    CMOS_SENSOR_INPUT_WR_CONFIG(dev->base, config_reg);
}

/*
 * write_command_reg_get_frame_info
 *
 * Sends a GET_FRAME_INFO command to the controller.
 */
static void write_command_reg_get_frame_info(cmos_sensor_input_dev *dev) {
    CMOS_SENSOR_INPUT_WR_COMMAND(dev->base, CMOS_SENSOR_INPUT_COMMAND_GET_FRAME_INFO);
}

/*
 * write_command_reg_snapshot
 *
 * Sends a SNAPSHOT command to the controller.
 */
static void write_command_reg_snapshot(cmos_sensor_input_dev *dev) {
    CMOS_SENSOR_INPUT_WR_COMMAND(dev->base, CMOS_SENSOR_INPUT_COMMAND_SNAPSHOT);
}

/*
 * write_command_reg_irq_ack
 *
 * Sends an IRQ_ACK command to the controller.
 */
static void write_command_reg_irq_ack(cmos_sensor_input_dev *dev) {
    CMOS_SENSOR_INPUT_WR_COMMAND(dev->base, CMOS_SENSOR_INPUT_COMMAND_IRQ_ACK);
}

/*
 * write_command_reg_stop_and_reset
 *
 * Sends a STOP_AND_RESET command to the controller.
 */
static void write_command_reg_stop_and_reset(cmos_sensor_input_dev *dev) {
    CMOS_SENSOR_INPUT_WR_COMMAND(dev->base, CMOS_SENSOR_INPUT_COMMAND_STOP_AND_RESET);
}

/*
 * read_status_reg_state_flag
 *
 * Returns CMOS_SENSOR_INPUT_STATUS_STATE_IDLE if the controller is idle.
 * Returns CMOS_SENSOR_INPUT_STATUS_STATE_BUSY if the controller is busy.
 */
static uint32_t read_status_reg_state_flag(cmos_sensor_input_dev *dev) {
    uint32_t status_reg = CMOS_SENSOR_INPUT_RD_STATUS(dev->base);
    uint32_t state_flag = (status_reg & CMOS_SENSOR_INPUT_STATUS_STATE_MASK) >> CMOS_SENSOR_INPUT_STATUS_STATE_OFST;
    return state_flag;
}

/*
 * read_status_reg_fifo_ovfl_flag
 *
 * Returns CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_NO_OVERFLOW if the fifo has not overflowed.
 * Returns CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW if the fifo has overflowed.
 */
static uint32_t read_status_reg_fifo_ovfl_flag(cmos_sensor_input_dev *dev) {
    uint32_t status_reg = CMOS_SENSOR_INPUT_RD_STATUS(dev->base);
    uint32_t ovfl_flag = (status_reg & CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_MASK) >> CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OFST;
    return ovfl_flag;
}

/*
 * read_status_reg_fifo_fill_level_flag
 *
 * Returns the current fill level of the fifo.
 */
static uint32_t read_status_reg_fifo_fill_level_flag(cmos_sensor_input_dev *dev) {
    uint32_t status_reg = CMOS_SENSOR_INPUT_RD_STATUS(dev->base);
    uint32_t fill_level_flag = (status_reg & CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_MASK) >> CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_OFST;
    return fill_level_flag;
}

/*
 * read_frame_info_reg_frame_width_flag
 *
 * Returns the frame_width discovered by the device.
 */
static uint32_t read_frame_info_reg_frame_width_flag(cmos_sensor_input_dev *dev) {
    uint32_t frame_info_reg = CMOS_SENSOR_INPUT_RD_FRAME_INFO(dev->base);
    uint32_t frame_width_flag = (frame_info_reg & CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_MASK) >> CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_OFST;
    return frame_width_flag;
}

/*
 * read_frame_info_reg_frame_height_flag
 *
 * Returns the height discovered by the device.
 */
static uint32_t read_frame_info_reg_frame_height_flag(cmos_sensor_input_dev *dev) {
    uint32_t frame_info_reg = CMOS_SENSOR_INPUT_RD_FRAME_INFO(dev->base);
    uint32_t frame_height_flag = (frame_info_reg & CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_MASK) >> CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_OFST;
    return frame_height_flag;
}

/*******************************************************************************
 *  Public API
 ******************************************************************************/
/*
 * cmos_sensor_input_inst
 *
 * Constructs a device structure.
 */
cmos_sensor_input_dev cmos_sensor_input_inst(void *base, uint8_t pix_depth, uint32_t max_width, uint32_t max_height, uint32_t output_width, uint32_t fifo_depth, bool debayer_enable, bool packer_enable) {
    cmos_sensor_input_dev dev;

    dev.base = base;
    dev.pix_depth = pix_depth;
    dev.max_width = max_width;
    dev.max_height = max_height;
    dev.output_width = output_width;
    dev.fifo_depth = fifo_depth;
    dev.debayer_enable = debayer_enable;
    dev.packer_enable = packer_enable;

    return dev;
}

/*
 * cmos_sensor_input_init
 *
 * Initializes the controller.
 *
 * This routine disables interrupts and sets the debayering unit (if enabled) to
 * RGGB mode.
 */
void cmos_sensor_input_init(cmos_sensor_input_dev *dev) {
    cmos_sensor_input_command_stop_and_reset(dev);
    cmos_sensor_input_configure(dev, false, RGGB);
}

/*
 * cmos_sensor_input_configure
 *
 * Configure the controller.
 *
 * Setting the irq paramater to true enables interrupt generation at the end of
 * a frame, and false disables interrupt generation.
 *
 * The pattern argument sets the debayering pattern to be used by the unit. This
 * argument is only used if debayering is enabled.
 */
void cmos_sensor_input_configure(cmos_sensor_input_dev *dev, bool irq, cmos_sensor_input_debayer_pattern pattern) {
    cmos_sensor_input_wait_until_idle(dev);
    write_config_reg_irq_flag(dev, irq);
    write_config_reg_debayer_pattern_flag(dev, pattern);
}

/*
 * cmos_sensor_input_config_irq_enabled
 *
 * Returns true if interrupt generation is enabled.
 * Returns false if interrupt generation is disabled.
 */
bool cmos_sensor_input_config_irq_enabled(cmos_sensor_input_dev *dev) {
    return read_config_reg_irq_flag(dev) == CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE;
}

/*
 * cmos_sensor_input_config_debayer_pattern
 *
 * Returns the debayering pattern currently used by the unit (if applicable).
 */
cmos_sensor_input_debayer_pattern cmos_sensor_input_config_debayer_pattern(cmos_sensor_input_dev *dev) {
    uint32_t debayer_pattern = read_config_reg_debayer_pattern_flag(dev);

    if (debayer_pattern == CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB) {
        return RGGB;
    } else if (debayer_pattern == CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BGGR) {
        return BGGR;
    } else if (debayer_pattern == CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GRBG) {
        return GRBG;
    } else {
        /* debayer_pattern == CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GBRG */
        return GBRG;
    }
}

/*
 * cmos_sensor_input_get_frame_info_sync
 *
 * Instructs the controller to analyze a frame and store its properties in
 * internal registers. This function MUST be called before calling
 * cmos_sensor_input_snapshot() to ensure correct functionality.
 *
 * This function waits until the GET_FRAME_INFO command finishes before
 * returning.
 */
void cmos_sensor_input_command_get_frame_info_sync(cmos_sensor_input_dev *dev) {
    cmos_sensor_input_wait_until_idle(dev);
    write_command_reg_get_frame_info(dev);
    cmos_sensor_input_wait_until_idle(dev);
}

/*
 * cmos_sensor_input_get_frame_info_async
 *
 * Instructs the controller to analyze a frame and store its properties in
 * internal registers. This function MUST be called before calling
 * cmos_sensor_input_snapshot() to ensure correct functionality.
 *
 * This function returns as soon as the command is sent to the controller. It is
 * the caller's responsability to check if the controller is idle before
 * submitting any further commands.
 */
void cmos_sensor_input_command_get_frame_info_async(cmos_sensor_input_dev *dev) {
    cmos_sensor_input_wait_until_idle(dev);
    write_command_reg_get_frame_info(dev);
}

/*
 * cmos_sensor_input_snapshot_sync
 *
 * Instructs the controller to capture a frame. You MUST call
 * cmos_sensor_input_get_frame_info() before calling this function to ensure
 * correct functionality.
 *
 * This function waits until the SNAPSHOT command finishes before returning.
 *
 * Returns true if the fifo did not overflow, and false otherwise.
 */
bool cmos_sensor_input_command_snapshot_sync(cmos_sensor_input_dev *dev) {
    cmos_sensor_input_wait_until_idle(dev);
    write_command_reg_snapshot(dev);
    return cmos_sensor_input_wait_until_idle(dev);
}

/*
 * cmos_sensor_input_snapshot_async
 *
 * Instructs the controller to capture a frame. You MUST call
 * cmos_sensor_input_get_frame_info() before calling this function to ensure
 * correct functionality.
 *
 * This function returns as soon as the command is sent to the controller. It is
 * the caller's responsability to check if the fifo overflowed before submitting
 * any further commands.
 */
void cmos_sensor_input_command_snapshot_async(cmos_sensor_input_dev *dev) {
    cmos_sensor_input_wait_until_idle(dev);
    write_command_reg_snapshot(dev);
}

/*
 * cmos_sensor_input_irq_ack
 *
 * Instructs the controller to acknowledge any pending interrupts.
 */
void cmos_sensor_input_command_irq_ack(cmos_sensor_input_dev *dev) {
    write_command_reg_irq_ack(dev);
}

/*
 * cmos_sensor_input_stop_and_reset
 *
 * Instructs the controller to halt all operations and to reset all internal
 * units. This function is useful if an operation needs to be stopped midway.
 */
void cmos_sensor_input_command_stop_and_reset(cmos_sensor_input_dev *dev) {
    write_command_reg_stop_and_reset(dev);
}

/*
 * cmos_sensor_input_status_idle
 *
 * Returns true if the controller is idle.
 * Returns false if the controller is busy.
 */
bool cmos_sensor_input_status_idle(cmos_sensor_input_dev *dev) {
    return read_status_reg_state_flag(dev) == CMOS_SENSOR_INPUT_STATUS_STATE_IDLE;
}

/*
 * cmos_sensor_input_status_fifo_ovfl
 *
 * Returns true if the fifo has overflowed.
 * Returns false if the fifo has not overflowed.
 */
bool cmos_sensor_input_status_fifo_ovfl(cmos_sensor_input_dev *dev) {
    return read_status_reg_fifo_ovfl_flag(dev) == CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW;
}

/*
 * cmos_sensor_input_status_fifo_fill_level
 *
 * Returns the fill level of the fifo.
 */
uint32_t cmos_sensor_input_status_fifo_fill_level(cmos_sensor_input_dev *dev) {
    return read_status_reg_fifo_fill_level_flag(dev);
}

/*
 * cmos_sensor_input_frame_info_frame_width
 *
 * Returns the frame width discovered when a GET_FRAME_INFO command was sent.
 */
uint32_t cmos_sensor_input_frame_info_frame_width(cmos_sensor_input_dev *dev) {
    return read_frame_info_reg_frame_width_flag(dev);
}

/*
 * cmos_sensor_input_frame_info_frame_height
 *
 * Returns the frame height discovered when a GET_FRAME_INFO command was sent.
 */
uint32_t cmos_sensor_input_frame_info_frame_height(cmos_sensor_input_dev *dev) {
    return read_frame_info_reg_frame_height_flag(dev);
}

/*
 * cmos_sensor_input_wait_until_idle
 *
 * Waits until the controller is idle.
 *
 * Returns true if the fifo did not overflow.
 * Returns false if the fifo did overflow.
 */
bool cmos_sensor_input_wait_until_idle(cmos_sensor_input_dev *dev) {
    while (!cmos_sensor_input_status_idle(dev)) {
        if (cmos_sensor_input_status_fifo_ovfl(dev)) {
            return false;
        }
    }

    return true;
}

/*
 * cmos_sensor_input_frame_size
 *
 * Returns the total size of a frame in bytes outputted by the cmos_sensor_input
 * unit in its current configuration.
 */
size_t cmos_sensor_input_frame_size(cmos_sensor_input_dev *dev) {
    cmos_sensor_input_wait_until_idle(dev);

    uint32_t frame_width = cmos_sensor_input_frame_info_frame_width(dev);
    uint32_t frame_height = cmos_sensor_input_frame_info_frame_height(dev);
    uint32_t frame_total_pixels = frame_width * frame_height;
    uint32_t num_pixels_in_output_width = 0;

    if (!dev->debayer_enable && !dev->packer_enable) {
        num_pixels_in_output_width = 1;
    } else if (!dev->debayer_enable && dev->packer_enable) {
        num_pixels_in_output_width = dev->output_width / dev->pix_depth;
    } else if (dev->debayer_enable && !dev->packer_enable) {
        num_pixels_in_output_width = 1;
    } else if (dev->debayer_enable && dev->packer_enable) {
        num_pixels_in_output_width = dev->output_width / (3 * dev->pix_depth);
    }

    uint32_t num_output_width_packets = ceil_div(frame_total_pixels, num_pixels_in_output_width);
    uint32_t frame_size_in_bytes = num_output_width_packets * (dev->output_width / 8);

    return frame_size_in_bytes;
}
