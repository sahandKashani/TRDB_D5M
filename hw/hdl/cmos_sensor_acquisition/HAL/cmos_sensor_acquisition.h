#ifndef __CMOS_SENSOR_ACQUISITION_H__
#define __CMOS_SENSOR_ACQUISITION_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cmos_sensor_input.h"
#include "msgdma.h"

typedef struct cmos_sensor_acquisition_dev {
    cmos_sensor_input_dev cmos_sensor_input;
    msgdma_dev            msgdma;
} cmos_sensor_acquisition_dev;

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
                                                         uint8_t  msgdma_csr_response_port);

void cmos_sensor_acquisition_init(cmos_sensor_acquisition_dev *dev);

void cmos_sensor_acquisition_configure(cmos_sensor_acquisition_dev *dev);
size_t cmos_sensor_acquisition_frame_size(cmos_sensor_acquisition_dev *dev);
uint32_t cmos_sensor_acquisition_frame_width(cmos_sensor_acquisition_dev *dev);
uint32_t cmos_sensor_acquisition_frame_height(cmos_sensor_acquisition_dev *dev);
bool cmos_sensor_acquisition_snapshot(cmos_sensor_acquisition_dev *dev, void *frame, size_t frame_size);

#endif /* __CMOS_SENSOR_ACQUISITION_H__ */
