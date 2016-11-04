package require -exact qsys 15.1


#
# module cmos_sensor_input
#
set_module_property DESCRIPTION {"cmos_sensor_input -> dc_fifo -> msgdma"}
set_module_property NAME {cmos_sensor_acquisition}
set_module_property VERSION {15.1}
set_module_property OPAQUE_ADDRESS_MAP false
set_module_property AUTHOR {"Sahand Kashani-Akhavan"}
set_module_property DISPLAY_NAME {cmos sensor acquisition}
set_module_property COMPOSITION_CALLBACK compose

proc compose {} {
    set CLK_IN_CLOCK_FREQUENCY [get_parameter_value CLK_IN_CLOCK_FREQUENCY]
    set CLK_OUT_CLOCK_FREQUENCY [get_parameter_value CLK_OUT_CLOCK_FREQUENCY]

    set CMOS_SENSOR_INPUT_PIX_DEPTH [get_parameter_value CMOS_SENSOR_INPUT_PIX_DEPTH]
    set CMOS_SENSOR_INPUT_SAMPLE_EDGE [get_parameter_value CMOS_SENSOR_INPUT_SAMPLE_EDGE]
    set CMOS_SENSOR_INPUT_MAX_WIDTH [get_parameter_value CMOS_SENSOR_INPUT_MAX_WIDTH]
    set CMOS_SENSOR_INPUT_MAX_HEIGHT [get_parameter_value CMOS_SENSOR_INPUT_MAX_HEIGHT]
    set CMOS_SENSOR_INPUT_OUTPUT_WIDTH [get_parameter_value CMOS_SENSOR_INPUT_OUTPUT_WIDTH]
    set CMOS_SENSOR_INPUT_FIFO_DEPTH [get_parameter_value CMOS_SENSOR_INPUT_FIFO_DEPTH]
    set CMOS_SENSOR_INPUT_DEVICE_FAMILY [get_parameter_value CMOS_SENSOR_INPUT_DEVICE_FAMILY]
    set CMOS_SENSOR_INPUT_DEBAYER_ENABLE [get_parameter_value CMOS_SENSOR_INPUT_DEBAYER_ENABLE]
    set CMOS_SENSOR_INPUT_PACKER_ENABLE [get_parameter_value CMOS_SENSOR_INPUT_PACKER_ENABLE]

    set DC_FIFO_DEPTH [get_parameter_value DC_FIFO_DEPTH]
    set DC_FIFO_WIDTH [get_parameter_value DC_FIFO_WIDTH]
    set DC_FIFO_SYMBOLS_PER_BEAT [expr $DC_FIFO_WIDTH / 8]

    set MSGDMA_DATA_WIDTH [get_parameter_value MSGDMA_DATA_WIDTH]
    set MSGDMA_DATA_FIFO_DEPTH [get_parameter_value MSGDMA_DATA_FIFO_DEPTH]
    set MSGDMA_DESCRIPTOR_FIFO_DEPTH [get_parameter_value MSGDMA_DESCRIPTOR_FIFO_DEPTH]
    set MSGDMA_MAX_BYTE [get_parameter_value MSGDMA_MAX_BYTE]
    set MSGDMA_BURST_ENABLE [get_parameter_value MSGDMA_BURST_ENABLE]
    set MSGDMA_MAX_BURST_COUNT [get_parameter_value MSGDMA_MAX_BURST_COUNT]

    # Instances and instance parameters
    # (disabled instances are intentionally culled)
    add_instance clk_in clock_source 15.1
    set_instance_parameter_value clk_in {clockFrequency} $CLK_IN_CLOCK_FREQUENCY
    set_instance_parameter_value clk_in {clockFrequencyKnown} {1}
    set_instance_parameter_value clk_in {resetSynchronousEdges} {NONE}

    add_instance clk_out clock_source 15.1
    set_instance_parameter_value clk_out {clockFrequency} $CLK_OUT_CLOCK_FREQUENCY
    set_instance_parameter_value clk_out {clockFrequencyKnown} {1}
    set_instance_parameter_value clk_out {resetSynchronousEdges} {NONE}

    add_instance cmos_sensor_input_0 cmos_sensor_input 15.1
    set_instance_parameter_value cmos_sensor_input_0 {PIX_DEPTH} $CMOS_SENSOR_INPUT_PIX_DEPTH
    set_instance_parameter_value cmos_sensor_input_0 {SAMPLE_EDGE} $CMOS_SENSOR_INPUT_SAMPLE_EDGE
    set_instance_parameter_value cmos_sensor_input_0 {MAX_WIDTH} $CMOS_SENSOR_INPUT_MAX_WIDTH
    set_instance_parameter_value cmos_sensor_input_0 {MAX_HEIGHT} $CMOS_SENSOR_INPUT_MAX_HEIGHT
    set_instance_parameter_value cmos_sensor_input_0 {OUTPUT_WIDTH} $CMOS_SENSOR_INPUT_OUTPUT_WIDTH
    set_instance_parameter_value cmos_sensor_input_0 {FIFO_DEPTH} $CMOS_SENSOR_INPUT_FIFO_DEPTH
    set_instance_parameter_value cmos_sensor_input_0 {DEVICE_FAMILY} $CMOS_SENSOR_INPUT_DEVICE_FAMILY
    set_instance_parameter_value cmos_sensor_input_0 {DEBAYER_ENABLE} $CMOS_SENSOR_INPUT_DEBAYER_ENABLE
    set_instance_parameter_value cmos_sensor_input_0 {PACKER_ENABLE} $CMOS_SENSOR_INPUT_PACKER_ENABLE

    add_instance dc_fifo_0 altera_avalon_dc_fifo 15.1
    set_instance_parameter_value dc_fifo_0 {SYMBOLS_PER_BEAT} $DC_FIFO_SYMBOLS_PER_BEAT
    set_instance_parameter_value dc_fifo_0 {BITS_PER_SYMBOL} {8}
    set_instance_parameter_value dc_fifo_0 {FIFO_DEPTH} $DC_FIFO_DEPTH
    set_instance_parameter_value dc_fifo_0 {CHANNEL_WIDTH} {0}
    set_instance_parameter_value dc_fifo_0 {ERROR_WIDTH} {0}
    set_instance_parameter_value dc_fifo_0 {USE_PACKETS} {0}
    set_instance_parameter_value dc_fifo_0 {USE_IN_FILL_LEVEL} {0}
    set_instance_parameter_value dc_fifo_0 {USE_OUT_FILL_LEVEL} {0}
    set_instance_parameter_value dc_fifo_0 {WR_SYNC_DEPTH} {3}
    set_instance_parameter_value dc_fifo_0 {RD_SYNC_DEPTH} {3}
    set_instance_parameter_value dc_fifo_0 {ENABLE_EXPLICIT_MAXCHANNEL} {0}
    set_instance_parameter_value dc_fifo_0 {EXPLICIT_MAXCHANNEL} {0}

    add_instance mm_bridge_0 altera_avalon_mm_bridge 15.1
    set_instance_parameter_value mm_bridge_0 {DATA_WIDTH} {32}
    set_instance_parameter_value mm_bridge_0 {SYMBOL_WIDTH} {8}
    set_instance_parameter_value mm_bridge_0 {ADDRESS_WIDTH} {10}
    set_instance_parameter_value mm_bridge_0 {USE_AUTO_ADDRESS_WIDTH} {1}
    set_instance_parameter_value mm_bridge_0 {ADDRESS_UNITS} {SYMBOLS}
    set_instance_parameter_value mm_bridge_0 {MAX_BURST_SIZE} {1}
    set_instance_parameter_value mm_bridge_0 {MAX_PENDING_RESPONSES} {4}
    set_instance_parameter_value mm_bridge_0 {LINEWRAPBURSTS} {0}
    set_instance_parameter_value mm_bridge_0 {PIPELINE_COMMAND} {0}
    set_instance_parameter_value mm_bridge_0 {PIPELINE_RESPONSE} {0}
    set_instance_parameter_value mm_bridge_0 {USE_RESPONSE} {0}

    add_instance msgdma_0 altera_msgdma 15.1
    set_instance_parameter_value msgdma_0 {MODE} {2}
    set_instance_parameter_value msgdma_0 {DATA_WIDTH} $MSGDMA_DATA_WIDTH
    set_instance_parameter_value msgdma_0 {USE_FIX_ADDRESS_WIDTH} {0}
    set_instance_parameter_value msgdma_0 {FIX_ADDRESS_WIDTH} {32}
    set_instance_parameter_value msgdma_0 {DATA_FIFO_DEPTH} $MSGDMA_DATA_FIFO_DEPTH
    set_instance_parameter_value msgdma_0 {DESCRIPTOR_FIFO_DEPTH} $MSGDMA_DESCRIPTOR_FIFO_DEPTH
    set_instance_parameter_value msgdma_0 {RESPONSE_PORT} {2}
    set_instance_parameter_value msgdma_0 {MAX_BYTE} $MSGDMA_MAX_BYTE
    set_instance_parameter_value msgdma_0 {TRANSFER_TYPE} {Aligned Accesses}
    set_instance_parameter_value msgdma_0 {BURST_ENABLE} $MSGDMA_BURST_ENABLE
    set_instance_parameter_value msgdma_0 {MAX_BURST_COUNT} $MSGDMA_MAX_BURST_COUNT
    set_instance_parameter_value msgdma_0 {BURST_WRAPPING_SUPPORT} {0}
    set_instance_parameter_value msgdma_0 {ENHANCED_FEATURES} {0}
    set_instance_parameter_value msgdma_0 {STRIDE_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {MAX_STRIDE} {1}
    set_instance_parameter_value msgdma_0 {PROGRAMMABLE_BURST_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {PACKET_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {ERROR_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {ERROR_WIDTH} {8}
    set_instance_parameter_value msgdma_0 {CHANNEL_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {CHANNEL_WIDTH} {8}
    set_instance_parameter_value msgdma_0 {PREFETCHER_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {PREFETCHER_READ_BURST_ENABLE} {0}
    set_instance_parameter_value msgdma_0 {PREFETCHER_DATA_WIDTH} {32}
    set_instance_parameter_value msgdma_0 {PREFETCHER_MAX_READ_BURST_COUNT} {2}

    # connections and connection parameters
    add_connection mm_bridge_0.m0 cmos_sensor_input_0.avalon_slave avalon
    set_connection_parameter_value mm_bridge_0.m0/cmos_sensor_input_0.avalon_slave arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/cmos_sensor_input_0.avalon_slave baseAddress {0x0030}
    set_connection_parameter_value mm_bridge_0.m0/cmos_sensor_input_0.avalon_slave defaultConnection {0}

    add_connection mm_bridge_0.m0 msgdma_0.csr avalon
    set_connection_parameter_value mm_bridge_0.m0/msgdma_0.csr arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/msgdma_0.csr baseAddress {0x0000}
    set_connection_parameter_value mm_bridge_0.m0/msgdma_0.csr defaultConnection {0}

    add_connection mm_bridge_0.m0 msgdma_0.descriptor_slave avalon
    set_connection_parameter_value mm_bridge_0.m0/msgdma_0.descriptor_slave arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/msgdma_0.descriptor_slave baseAddress {0x0020}
    set_connection_parameter_value mm_bridge_0.m0/msgdma_0.descriptor_slave defaultConnection {0}

    add_connection cmos_sensor_input_0.avalon_streaming_source dc_fifo_0.in avalon_streaming

    add_connection dc_fifo_0.out msgdma_0.st_sink avalon_streaming

    add_connection clk_out.clk mm_bridge_0.clk clock

    add_connection clk_out.clk msgdma_0.clock clock

    add_connection clk_in.clk cmos_sensor_input_0.clock clock

    add_connection clk_in.clk dc_fifo_0.in_clk clock

    add_connection clk_out.clk dc_fifo_0.out_clk clock

    add_connection clk_in.clk_reset dc_fifo_0.in_clk_reset reset

    add_connection clk_out.clk_reset dc_fifo_0.out_clk_reset reset

    add_connection clk_out.clk_reset mm_bridge_0.reset reset

    add_connection clk_in.clk_reset cmos_sensor_input_0.reset reset

    add_connection clk_out.clk_reset msgdma_0.reset_n reset

    # exported interfaces
    add_interface avalon_master avalon master
    set_interface_property avalon_master EXPORT_OF msgdma_0.mm_write
    add_interface avalon_slave avalon slave
    set_interface_property avalon_slave EXPORT_OF mm_bridge_0.s0
    add_interface clk_in_clk clock sink
    set_interface_property clk_in_clk EXPORT_OF clk_in.clk_in
    add_interface clk_in_reset reset sink
    set_interface_property clk_in_reset EXPORT_OF clk_in.clk_in_reset
    add_interface clk_out_clk clock sink
    set_interface_property clk_out_clk EXPORT_OF clk_out.clk_in
    add_interface clk_out_reset reset sink
    set_interface_property clk_out_reset EXPORT_OF clk_out.clk_in_reset
    add_interface cmos_sensor conduit end
    set_interface_property cmos_sensor EXPORT_OF cmos_sensor_input_0.cmos_sensor
    add_interface cmos_sensor_input_irq interrupt sender
    set_interface_property cmos_sensor_input_irq EXPORT_OF cmos_sensor_input_0.interrupt_sender
    add_interface msgdma_csr_irq interrupt sender
    set_interface_property msgdma_csr_irq EXPORT_OF msgdma_0.csr_irq

    # interconnect requirements
    set_interconnect_requirement {$system} {qsys_mm.clockCrossingAdapter} {HANDSHAKE}
    set_interconnect_requirement {$system} {qsys_mm.maxAdditionalLatency} {1}
    set_interconnect_requirement {$system} {qsys_mm.enableEccProtection} {FALSE}
    set_interconnect_requirement {$system} {qsys_mm.insertDefaultSlave} {FALSE}
}

#
# clk_in parameters
#
add_parameter CLK_IN_CLOCK_FREQUENCY NATURAL 50000000
set_parameter_property CLK_IN_CLOCK_FREQUENCY DISPLAY_NAME "Clock frequency"
set_parameter_property CLK_IN_CLOCK_FREQUENCY TYPE NATURAL
set_parameter_property CLK_IN_CLOCK_FREQUENCY UNITS hertz
set_parameter_property CLK_IN_CLOCK_FREQUENCY DESCRIPTION "Camera clock frequency"
set_parameter_property CLK_IN_CLOCK_FREQUENCY GROUP "CMOS Sensor Clock"

#
# cmos_sensor_input parameters
#
add_parameter CMOS_SENSOR_INPUT_PIX_DEPTH POSITIVE 8 "Depth of each pixel sample"
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH DISPLAY_NAME "Pixel Depth"
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH TYPE POSITIVE
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH UNITS bits
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH ALLOWED_RANGES {1:32}
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH DESCRIPTION "Depth of each pixel sample"
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_PIX_DEPTH GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_SAMPLE_EDGE STRING RISING "Clock edge used for sampling data from the CMOS sensor"
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE DISPLAY_NAME "Edge Type"
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE TYPE STRING
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE UNITS None
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE ALLOWED_RANGES {RISING FALLING}
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE DESCRIPTION "Clock edge used for sampling data from the CMOS sensor"
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_SAMPLE_EDGE GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_MAX_WIDTH POSITIVE 1920 "Maximum frame width"
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH DISPLAY_NAME "Maximum Frame Width"
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH TYPE POSITIVE
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH UNITS None
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH ALLOWED_RANGES {2:65535}
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH DESCRIPTION "Maximum frame width"
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_MAX_WIDTH GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_MAX_HEIGHT POSITIVE 1080 "Maximum frame height"
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT DISPLAY_NAME "Maximum Frame Height"
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT TYPE POSITIVE
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT UNITS None
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT ALLOWED_RANGES {1:65535}
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT DESCRIPTION "Maximum frame height"
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_MAX_HEIGHT GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_OUTPUT_WIDTH POSITIVE 32 "Pixel width used as output from the component"
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH DISPLAY_NAME "Pixel Output Data Width"
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH TYPE POSITIVE
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH UNITS bits
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH DESCRIPTION "Pixel width used as output from the component"
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_OUTPUT_WIDTH GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_FIFO_DEPTH POSITIVE 32 "Output FIFO depth"
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH DISPLAY_NAME "Output FIFO Depth"
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH TYPE POSITIVE
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH UNITS None
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH DESCRIPTION "Output FIFO depth"
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_FIFO_DEPTH GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_DEVICE_FAMILY STRING "Cyclone V" "Target FPGA device family (only used for efficient FIFO instantiation)"
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY DISPLAY_NAME "FPGA Device Family"
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY TYPE STRING
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY UNITS None
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY ALLOWED_RANGES {"Cyclone V" "Cyclone IV E"}
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY DESCRIPTION "Target FPGA device family (only used for efficient FIFO instantiation)"
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_DEVICE_FAMILY GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_DEBAYER_ENABLE BOOLEAN FALSE "Enable Debayering"
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE DISPLAY_NAME "Enable Debayering"
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE TYPE BOOLEAN
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE UNITS None
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE ALLOWED_RANGES {}
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE DESCRIPTION "Enable Debayering"
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE ENABLED false
set_parameter_property CMOS_SENSOR_INPUT_DEBAYER_ENABLE GROUP "CMOS Sensor Input"

add_parameter CMOS_SENSOR_INPUT_PACKER_ENABLE BOOLEAN FALSE "Enable packing of multiple pixels into a single output word of size OUTPUT_WIDTH"
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE DISPLAY_NAME "Enable Pixel Packer"
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE TYPE BOOLEAN
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE UNITS None
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE ALLOWED_RANGES {}
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE DESCRIPTION "Enable packing of multiple pixels into a single output word of size OUTPUT_WIDTH"
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE HDL_PARAMETER true
set_parameter_property CMOS_SENSOR_INPUT_PACKER_ENABLE GROUP "CMOS Sensor Input"

#
# dc_fifo parameters
#
add_parameter DC_FIFO_DEPTH POSITIVE 16 "FIFO depth"
set_parameter_property DC_FIFO_DEPTH DISPLAY_NAME "FIFO Depth"
set_parameter_property DC_FIFO_DEPTH TYPE POSITIVE
set_parameter_property DC_FIFO_DEPTH UNITS None
set_parameter_property DC_FIFO_DEPTH ALLOWED_RANGES {16 32 64 128 256 512 1024 2048 4096}
set_parameter_property DC_FIFO_DEPTH DESCRIPTION "FIFO depth"
set_parameter_property DC_FIFO_DEPTH HDL_PARAMETER true
set_parameter_property DC_FIFO_DEPTH GROUP "Dual-Clock FIFO"

add_parameter DC_FIFO_WIDTH POSITIVE 32 "FIFO width"
set_parameter_property DC_FIFO_WIDTH DISPLAY_NAME "FIFO Width"
set_parameter_property DC_FIFO_WIDTH TYPE POSITIVE
set_parameter_property DC_FIFO_WIDTH UNITS None
set_parameter_property DC_FIFO_WIDTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property DC_FIFO_WIDTH DESCRIPTION "FIFO width"
set_parameter_property DC_FIFO_WIDTH HDL_PARAMETER true
set_parameter_property DC_FIFO_WIDTH GROUP "Dual-Clock FIFO"

#
# msgdma parameters
#
add_parameter MSGDMA_DATA_WIDTH INTEGER 32 "Width of the data path. This impacts both Avalon MM as well as Avalon ST"
set_parameter_property MSGDMA_DATA_WIDTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property MSGDMA_DATA_WIDTH DISPLAY_NAME "Data Width"
set_parameter_property MSGDMA_DATA_WIDTH AFFECTS_GENERATION true
set_parameter_property MSGDMA_DATA_WIDTH DERIVED false
set_parameter_property MSGDMA_DATA_WIDTH HDL_PARAMETER false
set_parameter_property MSGDMA_DATA_WIDTH AFFECTS_ELABORATION true
set_parameter_property MSGDMA_DATA_WIDTH GROUP "Modular Scatter-Gather DMA"

add_parameter MSGDMA_DATA_FIFO_DEPTH INTEGER 64 "Depth of the internal data FIFO."
set_parameter_property MSGDMA_DATA_FIFO_DEPTH ALLOWED_RANGES {16 32 64 128 256 512 1024 2048 4096}
set_parameter_property MSGDMA_DATA_FIFO_DEPTH DISPLAY_NAME "Data Path FIFO Depth"
set_parameter_property MSGDMA_DATA_FIFO_DEPTH DERIVED false
set_parameter_property MSGDMA_DATA_FIFO_DEPTH AFFECTS_GENERATION true
set_parameter_property MSGDMA_DATA_FIFO_DEPTH HDL_PARAMETER false
set_parameter_property MSGDMA_DATA_FIFO_DEPTH AFFECTS_ELABORATION true
set_parameter_property MSGDMA_DATA_FIFO_DEPTH GROUP "Modular Scatter-Gather DMA"

add_parameter MSGDMA_DESCRIPTOR_FIFO_DEPTH INTEGER 8 "Depth of the internal descriptor FIFO."
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH DISPLAY_NAME "Descriptor FIFO Depth"
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH UNITS None
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH AFFECTS_ELABORATION true
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH AFFECTS_GENERATION true
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH DERIVED false
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH HDL_PARAMETER false
set_parameter_property MSGDMA_DESCRIPTOR_FIFO_DEPTH GROUP "Modular Scatter-Gather DMA"

add_parameter MSGDMA_MAX_BYTE INTEGER 8388608 "Maximum transfer length"
set_parameter_property MSGDMA_MAX_BYTE UNITS {None}
set_parameter_property MSGDMA_MAX_BYTE ALLOWED_RANGES {"1024:1KB" "2048:2KB" "4096:4KB" "8192:8KB" "16384:16KB" "32768:32KB" "65536:64KB" "131072:128KB" "262144:256KB" "524288:512KB" "1048576:1MB" "2097152:2MB" "4194304:4MB" "8388608:8MB" "16777216:16MB" "33554432:32MB" "67108864:64MB" "134217728:128MB" "368435456:256MB" "536870912:512MB" "1073741824:1GB" "2147483647:2GB"}
set_parameter_property MSGDMA_MAX_BYTE DISPLAY_NAME "Maximum Transfer Length"
set_parameter_property MSGDMA_MAX_BYTE AFFECTS_GENERATION true
set_parameter_property MSGDMA_MAX_BYTE DERIVED false
set_parameter_property MSGDMA_MAX_BYTE HDL_PARAMETER false
set_parameter_property MSGDMA_MAX_BYTE AFFECTS_ELABORATION true
set_parameter_property MSGDMA_MAX_BYTE GROUP "Modular Scatter-Gather DMA"

add_parameter MSGDMA_BURST_ENABLE INTEGER 1 "Burst enable will turn on the bursting capabilities of the read master.  Bursting must not be enabled when stride is also enabled."
set_parameter_property MSGDMA_BURST_ENABLE DISPLAY_NAME "Burst Enable"
set_parameter_property MSGDMA_BURST_ENABLE DISPLAY_HINT boolean
set_parameter_property MSGDMA_BURST_ENABLE AFFECTS_GENERATION true
set_parameter_property MSGDMA_BURST_ENABLE HDL_PARAMETER false
set_parameter_property MSGDMA_BURST_ENABLE DERIVED false
set_parameter_property MSGDMA_BURST_ENABLE AFFECTS_ELABORATION true
set_parameter_property MSGDMA_BURST_ENABLE GROUP "Modular Scatter-Gather DMA"

add_parameter MSGDMA_MAX_BURST_COUNT INTEGER 16 "Maximum burst count."
set_parameter_property MSGDMA_MAX_BURST_COUNT ALLOWED_RANGES {2 4 8 16 32 64 128 256 512 1024}
set_parameter_property MSGDMA_MAX_BURST_COUNT DISPLAY_NAME "Maximum Burst Count"
set_parameter_property MSGDMA_MAX_BURST_COUNT AFFECTS_GENERATION true
set_parameter_property MSGDMA_MAX_BURST_COUNT HDL_PARAMETER false
set_parameter_property MSGDMA_MAX_BURST_COUNT DERIVED false
set_parameter_property MSGDMA_MAX_BURST_COUNT AFFECTS_ELABORATION true
set_parameter_property MSGDMA_MAX_BURST_COUNT GROUP "Modular Scatter-Gather DMA"

#
# clk_out parameters
#
add_parameter CLK_OUT_CLOCK_FREQUENCY NATURAL 50000000
set_parameter_property CLK_OUT_CLOCK_FREQUENCY DISPLAY_NAME "Clock frequency"
set_parameter_property CLK_OUT_CLOCK_FREQUENCY TYPE NATURAL
set_parameter_property CLK_OUT_CLOCK_FREQUENCY UNITS hertz
set_parameter_property CLK_OUT_CLOCK_FREQUENCY DESCRIPTION "DMA clock frequency"
set_parameter_property CLK_OUT_CLOCK_FREQUENCY GROUP "DMA Clock"
