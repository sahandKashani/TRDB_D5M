#ifndef __CMOS_SENSOR_INPUT_IO_H__
#define __CMOS_SENSOR_INPUT_IO_H__

#ifdef __nios2_arch__
#include "io.h"

#define cmos_sensor_input_write_word(dest, src) (IOWR_32DIRECT((dest), 0, (src)))
#define cmos_sensor_input_read_word(src)        (IORD_32DIRECT((src), 0))

#else

#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define CMOS_SENSOR_INPUT_CAST(type, ptr)       ((type) (ptr))

#define cmos_sensor_input_write_word(dest, src) (*CMOS_SENSOR_INPUT_CAST(volatile uint32_t *, (dest)) = (src))
#define cmos_sensor_input_read_word(src)        (*CMOS_SENSOR_INPUT_CAST(volatile uint32_t *, (src)))

#endif

#endif /* __CMOS_SENSOR_INPUT_IO_H__ */
