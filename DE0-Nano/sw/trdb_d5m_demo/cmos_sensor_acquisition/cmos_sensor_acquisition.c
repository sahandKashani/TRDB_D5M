#include "cmos_sensor_acquisition.h"

/*******************************************************************************
 *  Private API
 ******************************************************************************/

/*******************************************************************************
 *  Public API
 ******************************************************************************/
 /*
 * cmos_sensor_acquisition_inst
 *
 * Constructs a device structure.
 */
cmos_sensor_acquisition_dev cmos_sensor_acquisition_inst(void     *cmos_sensor_input_base,
                                                         uint8_t  cmos_sensor_input_pix_depth,
                                                         uint32_t cmos_sensor_input_max_width,
                                                         uint32_t cmos_sensor_Input_max_height,
                                                         uint32_t cmos_sensor_input_output_width,
                                                         uint32_t cmos_sensor_input_fifo_depth,
                                                         bool     cmos_sensor_input_debayer_enable,
                                                         bool     cmos_sensor_input_pack_enable,
                                                         void     *msgdma_csr_base,
                                                         void     *msgdma_descriptor_base,
                                                         uint32_t msgdma_descriptor_fifo_depth,
                                                         uint8_t  msgdma_csr_burst_enable,
                                                         uint8_t  msgdma_csr_burst_wrapping_support,
                                                         uint32_t msgdma_csr_data_fifo_depth,
                                                         uint32_t msgdma_csr_data_width,
                                                         uint32_t msgdma_csr_max_burst_count,
                                                         uint32_t msgdma_csr_max_byte,
                                                         uint64_t msgdma_csr_max_stride,
                                                         uint8_t  msgdma_csr_programmable_burst_enable,
                                                         uint8_t  msgdma_csr_stride_enable,
                                                         uint8_t  msgdma_csr_enhanced_features,
                                                         uint8_t  msgdma_csr_response_port) {

    cmos_sensor_input_dev cmos_sensor_input = cmos_sensor_input_inst(cmos_sensor_input_base,
                                                                     cmos_sensor_input_pix_depth,
                                                                     cmos_sensor_input_max_width,
                                                                     cmos_sensor_Input_max_height,
                                                                     cmos_sensor_input_output_width,
                                                                     cmos_sensor_input_fifo_depth,
                                                                     cmos_sensor_input_debayer_enable,
                                                                     cmos_sensor_input_pack_enable);

    msgdma_dev msgdma = msgdma_csr_descriptor_inst(msgdma_csr_base,
                                                   msgdma_descriptor_base,
                                                   msgdma_descriptor_fifo_depth,
                                                   msgdma_csr_burst_enable,
                                                   msgdma_csr_burst_wrapping_support,
                                                   msgdma_csr_data_fifo_depth,
                                                   msgdma_csr_data_width,
                                                   msgdma_csr_max_burst_count,
                                                   msgdma_csr_max_byte,
                                                   msgdma_csr_max_stride,
                                                   msgdma_csr_programmable_burst_enable,
                                                   msgdma_csr_stride_enable,
                                                   msgdma_csr_enhanced_features,
                                                   msgdma_csr_response_port);

    cmos_sensor_acquisition_dev dev;
    dev.cmos_sensor_input = cmos_sensor_input;
    dev.msgdma = msgdma;

    return dev;
}

/*
 * cmos_sensor_acquisition_init
 *
 * Initializes the device.
 *
 * This routine configures the cmos_sensor_input to disable interrupts and sets
 * the debayering unit (if enabled) to RGGB mode.
 * The Modular Scatter-Gather DMA core is configured to disable interrupts and
 * descriptor processing.
 *
 */
void cmos_sensor_acquisition_init(cmos_sensor_acquisition_dev *dev) {
    cmos_sensor_input_init(&dev->cmos_sensor_input);
    msgdma_init(&dev->msgdma);
}

/*
 * cmos_sensor_acquisition_configure
 *
 * Configure the device.
 *
 * This function configures the cmos_sensor_input to disable interrupt
 * generation, and sets the device debayering mode (if applicable) to RGGB mode.
 *
 * A GET_FRAME_INFO command is sent to analyze a current frame in order to
 * extract its width and height.
 */
void cmos_sensor_acquisition_configure(cmos_sensor_acquisition_dev *dev) {
    cmos_sensor_input_configure(&dev->cmos_sensor_input, false, RGGB);
    cmos_sensor_input_command_get_frame_info_sync(&dev->cmos_sensor_input);
}

/*
 * cmos_sensor_acquisition_frame_size
 *
 * Returns the total size of a frame in bytes outputted by the cmos_sensor_input
 * unit in its current configuration.
 */
size_t cmos_sensor_acquisition_frame_size(cmos_sensor_acquisition_dev *dev) {
    return cmos_sensor_input_frame_size(&dev->cmos_sensor_input);
}

/*
 * cmos_sensor_acquisition_frame_width
 *
 * Returns the width of a frame in pixels (determined by the cmos_sensor_input
 * unit).
 */
uint32_t cmos_sensor_acquisition_frame_width(cmos_sensor_acquisition_dev *dev) {
    return cmos_sensor_input_frame_info_frame_width(&dev->cmos_sensor_input);
}

/*
 * cmos_sensor_acquisition_frame_height
 *
 * Returns the height of a frame in pixels (determined by the cmos_sensor_input
 * unit).
 */
uint32_t cmos_sensor_acquisition_frame_height(cmos_sensor_acquisition_dev *dev) {
    return cmos_sensor_input_frame_info_frame_height(&dev->cmos_sensor_input);
}

/*
 * cmos_sensor_acquisition_snapshot
 *
 * Performs a blocking snapshot operation.
 *
 * Returns true if the frame was successfully saved, and false otherwise.
 *
 * A frame is considered successfully saved if and only if the msgdma can handle
 * the required frame size in a single descriptor and if the FIFO in the
 * cmos_sensor_input did not overflow.
 */
bool cmos_sensor_acquisition_snapshot(cmos_sensor_acquisition_dev *dev, void *frame, size_t frame_size) {
    msgdma_standard_descriptor desc;
    if (msgdma_construct_standard_st_to_mm_descriptor(&dev->msgdma, &desc, frame, frame_size, 0)) {
        return false;
    }

    /* send async dma transfer command to have the dma unit ready for data in
     * the fifo */
    if (msgdma_standard_descriptor_async_transfer(&dev->msgdma, &desc)) {
        return false;
    }

    /* start cmos_sensor_input capture logic */
    if (!cmos_sensor_input_command_snapshot_sync(&dev->cmos_sensor_input)) {
        return false;
    }

    msgdma_wait_until_idle(&dev->msgdma);
    return true;
}
