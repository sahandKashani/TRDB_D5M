#ifndef __TRDB_D5M_H__
#define __TRDB_D5M_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cmos_sensor_acquisition.h"
#include "i2c.h"
#include "trdb_d5m_regs.h"

/* trdb_d5m device structure */
typedef struct trdb_d5m_dev {
    cmos_sensor_acquisition_dev cmos_sensor_acquisition;
    i2c_dev                     i2c;
} trdb_d5m_dev;

trdb_d5m_dev trdb_d5m_inst(void     *cmos_sensor_acquisition_cmos_sensor_input_base,
                           uint8_t  cmos_sensor_acquisition_cmos_sensor_input_pix_depth,
                           uint32_t cmos_sensor_acquisition_cmos_sensor_input_max_width,
                           uint32_t cmos_sensor_acquisition_cmos_sensor_Input_max_height,
                           uint32_t cmos_sensor_acquisition_cmos_sensor_input_output_width,
                           uint32_t cmos_sensor_acquisition_cmos_sensor_input_fifo_depth,
                           bool     cmos_sensor_acquisition_cmos_sensor_input_debayer_enable,
                           bool     cmos_sensor_acquisition_cmos_sensor_input_pack_enable,
                           void     *cmos_sensor_acquisiton_sgdma_csr_base,
                           void     *cmos_sensor_acquisiton_sgdma_descriptor_base,
                           uint32_t cmos_sensor_acquisition_msgdma_descriptor_fifo_depth,
                           uint8_t  cmos_sensor_acquisition_msgdma_csr_burst_enable,
                           uint8_t  cmos_sensor_acquisition_msgdma_csr_burst_wrapping_support,
                           uint32_t cmos_sensor_acquisition_msgdma_csr_data_fifo_depth,
                           uint32_t cmos_sensor_acquisition_msgdma_csr_data_width,
                           uint32_t cmos_sensor_acquisition_msgdma_csr_max_burst_count,
                           uint32_t cmos_sensor_acquisition_msgdma_csr_max_byte,
                           uint64_t cmos_sensor_acquisition_msgdma_csr_max_stride,
                           uint8_t  cmos_sensor_acquisition_msgdma_csr_programmable_burst_enable,
                           uint8_t  cmos_sensor_acquisition_msgdma_csr_stride_enable,
                           uint8_t  cmos_sensor_acquisition_msgdma_csr_enhanced_features,
                           uint8_t  cmos_sensor_acquisition_msgdma_csr_response_port,
                           void     *i2c_base);

void trdb_d5m_init(trdb_d5m_dev *trdb_d5m, uint32_t i2c_freq);

bool trdb_d5m_configure(trdb_d5m_dev *dev,
                        uint16_t column_size, uint16_t row_size,
                        uint16_t row_bin, uint16_t row_skip,
                        uint16_t column_bin, uint16_t column_skip,
                        bool     continuous);

bool trdb_d5m_write(trdb_d5m_dev *trdb_d5m, uint8_t register_offset, uint16_t data);
bool trdb_d5m_read(trdb_d5m_dev *trdb_d5m, uint8_t register_offset, uint16_t *data);
bool trdb_d5m_snapshot(trdb_d5m_dev *dev, void *frame, size_t frame_size);
size_t trdb_d5m_frame_size(trdb_d5m_dev *dev);
uint32_t trdb_d5m_frame_width(trdb_d5m_dev *dev);
uint32_t trdb_d5m_frame_height(trdb_d5m_dev *dev);

#endif /* __TRDB_D5M_H__ */
