#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "trdb_d5m.h"
#include "system.h"

#define I2C_FREQ (50000000) /* 50 MHz */

#define TRDB_D5M_COLUMN_SIZE_REG_DATA (2559)
#define TRDB_D5M_ROW_SIZE_REG_DATA    (1919)
#define TRDB_D5M_ROW_BIN_REG_DATA     (3)
#define TRDB_D5M_ROW_SKIP_REG_DATA    (3)
#define TRDB_D5M_COLUMN_BIN_REG_DATA  (3)
#define TRDB_D5M_COLUMN_SKIP_REG_DATA (3)

uint16_t max_pixel_value(uint16_t *frame, uint32_t width, uint32_t height) {
    uint16_t max = 0;

    for (uint32_t row = 0; row < height; row++) {
        for (uint32_t col = 0; col < width; col++) {
            uint16_t current = frame[row * width + col];
            if (current > max) {
                max = current;
            }
        }
    }

    return max;
}

bool write_pgm(uint16_t *frame, uint32_t width, uint32_t height, const char *filename) {
    FILE *foutput = fopen(filename, "w");
    if (!foutput) {
        printf("Error: could not open \"%s\" for writing\n", filename);
        return false;
    }

    fprintf(foutput, "P2\n"); /* PGM magic number */
    fprintf(foutput, "%" PRIu32 " %" PRIu32 "\n", width, height); /* frame dimensions */
    fprintf(foutput, "%" PRIu16 "\n", max_pixel_value(frame, width, height)); /* max value */

    for (uint32_t row = 0; row < height; row++) {
        for (uint32_t col = 0; col < width; col++) {
            fprintf(foutput, "%05" PRIu16, frame[row * width + col]);

            if (col != (width - 1)) {
                fprintf(foutput, " ");
            }
        }

        if (row != (height - 1)) {
            fprintf(foutput, "\n");
        }
    }

    if (fclose(foutput)) {
        printf("Error: could not close \"%s\"\n", filename);
        return false;
    }

    return true;
}

int main(void) {
    /*
     * instantiate camera control structure
     */
    trdb_d5m_dev trdb_d5m = trdb_d5m_inst((void *) CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_BASE,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_PIX_DEPTH,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_MAX_WIDTH,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_MAX_HEIGHT,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_OUTPUT_WIDTH,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_FIFO_DEPTH,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_DEBAYER_ENABLE,
                                          CMOS_SENSOR_ACQUISITION_0_CMOS_SENSOR_INPUT_0_PACKER_ENABLE,
                                          (void *) CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_BASE,
                                          (void *) CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_DESCRIPTOR_SLAVE_BASE,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_DESCRIPTOR_SLAVE_DESCRIPTOR_FIFO_DEPTH,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_BURST_ENABLE,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_BURST_WRAPPING_SUPPORT,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_DATA_FIFO_DEPTH,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_DATA_WIDTH,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_MAX_BURST_COUNT,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_MAX_BYTE,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_MAX_STRIDE,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_PROGRAMMABLE_BURST_ENABLE,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_STRIDE_ENABLE,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_ENHANCED_FEATURES,
                                          CMOS_SENSOR_ACQUISITION_0_MSGDMA_0_CSR_RESPONSE_PORT,
                                          (void *) I2C_0_BASE);

    /*
     * initialize camera
     */
    trdb_d5m_init(&trdb_d5m, I2C_FREQ);

    /*
     * configure camera
     */
    if (!trdb_d5m_configure(&trdb_d5m,
                            TRDB_D5M_COLUMN_SIZE_REG_DATA, TRDB_D5M_ROW_SIZE_REG_DATA,
                            TRDB_D5M_ROW_BIN_REG_DATA, TRDB_D5M_ROW_SKIP_REG_DATA,
                            TRDB_D5M_COLUMN_BIN_REG_DATA, TRDB_D5M_COLUMN_SKIP_REG_DATA,
                            true)) {
        printf("Error: could not configure trdb_d5m\n");
        return EXIT_FAILURE;
    }

    /*
     * allocate frame memory
     */
    size_t frame_size = trdb_d5m_frame_size(&trdb_d5m);
    void *frame = calloc(frame_size, 1);
    if (!frame) {
        printf("Error: could not allocate memory for frame\n");
        return EXIT_FAILURE;
    }

    /*
     * take snapshot
     */
    if (!trdb_d5m_snapshot(&trdb_d5m, frame, frame_size)) {
        printf("Error: could not take snapshot\n");
        return EXIT_FAILURE;
    }

    /*
     * write image to host
     */
    if (!write_pgm((uint16_t *) frame,
                   trdb_d5m_frame_width(&trdb_d5m), trdb_d5m_frame_height(&trdb_d5m),
                   "/mnt/host/image.pgm")) {
        printf("Error: could not write image to file\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
