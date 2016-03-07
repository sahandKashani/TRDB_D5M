#ifndef __CMOS_SENSOR_INPUT_H__
#define __CMOS_SENSOR_INPUT_H__

#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif

/* cmos_sensor_input device structure */
typedef struct cmos_sensor_input_dev {
    void     *base;          /* Base address of component */
    uint8_t  pix_depth;      /* Depth of each pixel sample */
    uint32_t max_width;      /* Maximum input frame width */
    uint32_t max_height;     /* Maximum input frame height */
    uint32_t output_width;   /* Bus output width */
    uint32_t fifo_depth;     /* Output FIFO depth */
    bool     debayer_enable; /* Debayering enabled */
    bool     packer_enable;  /* Packer enabled */
} cmos_sensor_input_dev;

typedef enum cmos_sensor_input_debayer_pattern {RGGB, BGGR, GRBG, GBRG} cmos_sensor_input_debayer_pattern;

/*******************************************************************************
 *  Public API
 ******************************************************************************/
cmos_sensor_input_dev cmos_sensor_input_inst(void *base, uint8_t pix_depth, uint32_t max_width, uint32_t max_height, uint32_t output_width, uint32_t fifo_depth, bool debayer_enable, bool packer_enable);

void cmos_sensor_input_init(cmos_sensor_input_dev *dev);

void cmos_sensor_input_configure(cmos_sensor_input_dev *dev, bool irq, cmos_sensor_input_debayer_pattern pattern);
bool cmos_sensor_input_config_irq_enabled(cmos_sensor_input_dev *dev);
cmos_sensor_input_debayer_pattern cmos_sensor_input_config_debayer_pattern(cmos_sensor_input_dev *dev);
void cmos_sensor_input_command_get_frame_info_sync(cmos_sensor_input_dev *dev);
void cmos_sensor_input_command_get_frame_info_async(cmos_sensor_input_dev *dev);
bool cmos_sensor_input_command_snapshot_sync(cmos_sensor_input_dev *dev);
void cmos_sensor_input_command_snapshot_async(cmos_sensor_input_dev *dev);
void cmos_sensor_input_command_irq_ack(cmos_sensor_input_dev *dev);
void cmos_sensor_input_command_stop_and_reset(cmos_sensor_input_dev *dev);
bool cmos_sensor_input_status_idle(cmos_sensor_input_dev *dev);
bool cmos_sensor_input_status_fifo_ovfl(cmos_sensor_input_dev *dev);
uint32_t cmos_sensor_input_status_fifo_fill_level(cmos_sensor_input_dev *dev);
uint32_t cmos_sensor_input_frame_info_frame_width(cmos_sensor_input_dev *dev);
uint32_t cmos_sensor_input_frame_info_frame_height(cmos_sensor_input_dev *dev);
bool cmos_sensor_input_wait_until_idle(cmos_sensor_input_dev *dev);
size_t cmos_sensor_input_frame_size(cmos_sensor_input_dev *dev);

#endif /* __CMOS_SENSOR_INPUT_H__ */
