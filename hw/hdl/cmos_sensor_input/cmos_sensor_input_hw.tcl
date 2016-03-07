package require -exact qsys 15.1


#
# module cmos_sensor_input
#
set_module_property DESCRIPTION "This component provides an Avalon-ST src interface from a CMOS sensor"
set_module_property NAME cmos_sensor_input
set_module_property VERSION 15.1
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR "Sahand Kashani-Akhavan"
set_module_property DISPLAY_NAME "cmos sensor input"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false
set_module_property VALIDATION_CALLBACK validate


proc validate {} {
    set pix_depth [get_parameter_value PIX_DEPTH]
    set output_width [get_parameter_value OUTPUT_WIDTH]
    set debayer_enable [get_parameter_value DEBAYER_ENABLE]
    set packer_enable [get_parameter_value PACKER_ENABLE]

    set min_output_width_debayer_disable_packer_disable [expr 1 * $pix_depth]

    # need to be able to pack at least 2 RAW pixels
    set min_output_width_debayer_disable_packer_enable [expr 2 * $pix_depth]

    # need to be able to output RGB pixel
    set min_output_width_debayer_enable_packer_disable [expr 3 * $pix_depth]

    # need to be able to pack at least 2 RGB pixels
    set min_output_width_debayer_enable_packer_enable [expr 6 * $pix_depth]

    if {[expr !$debayer_enable && !$packer_enable]} {
        if {[expr $output_width < $min_output_width_debayer_disable_packer_disable]} {
            send_message error "OUTPUT_WIDTH must be larger or equal to $min_output_width_debayer_disable_packer_disable"
        }
    } elseif {[expr !$debayer_enable && $packer_enable]} {
        if {[expr $output_width < $min_output_width_debayer_disable_packer_enable]} {
            send_message error "OUTPUT_WIDTH must be larger or equal to $min_output_width_debayer_disable_packer_enable"
        }
    } elseif {[expr $debayer_enable && !$packer_enable]} {
        if {[expr $output_width < $min_output_width_debayer_enable_packer_disable]} {
            send_message error "OUTPUT_WIDTH must be larger or equal to $min_output_width_debayer_enable_packer_disable"
        }
    } elseif {[expr $debayer_enable && $packer_enable]} {
        if {[expr $output_width < $min_output_width_debayer_enable_packer_enable]} {
            send_message error "OUTPUT_WIDTH must be larger or equal to $min_output_width_debayer_enable_packer_enable"
        }
    }

    set_module_assignment embeddedsw.CMacro.PIX_DEPTH $pix_depth
    set_module_assignment embeddedsw.CMacro.SAMPLE_EDGE [get_parameter_value SAMPLE_EDGE]
    set_module_assignment embeddedsw.CMacro.MAX_WIDTH [get_parameter_value MAX_WIDTH]
    set_module_assignment embeddedsw.CMacro.MAX_HEIGHT [get_parameter_value MAX_HEIGHT]
    set_module_assignment embeddedsw.CMacro.OUTPUT_WIDTH [get_parameter_value OUTPUT_WIDTH]
    set_module_assignment embeddedsw.CMacro.FIFO_DEPTH [get_parameter_value FIFO_DEPTH]
    set_module_assignment embeddedsw.CMacro.DEBAYER_ENABLE [get_parameter_value DEBAYER_ENABLE]
    set_module_assignment embeddedsw.CMacro.PACKER_ENABLE [get_parameter_value PACKER_ENABLE]
}


#
# file sets
#
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL cmos_sensor_input
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file cmos_sensor_input_constants.vhd VHDL PATH hdl/cmos_sensor_input_constants.vhd
add_fileset_file cmos_sensor_input_avalon_mm_slave.vhd VHDL PATH hdl/cmos_sensor_input_avalon_mm_slave.vhd
add_fileset_file cmos_sensor_input_synchronizer.vhd VHDL PATH hdl/cmos_sensor_input_synchronizer.vhd
add_fileset_file cmos_sensor_input_sampler.vhd VHDL PATH hdl/cmos_sensor_input_sampler.vhd
add_fileset_file cmos_sensor_input_sc_fifo.vhd VHDL PATH hdl/cmos_sensor_input_sc_fifo.vhd
add_fileset_file cmos_sensor_input_debayer.vhd VHDL PATH hdl/cmos_sensor_input_debayer.vhd
add_fileset_file cmos_sensor_input_packer.vhd VHDL PATH hdl/cmos_sensor_input_packer.vhd
add_fileset_file cmos_sensor_input_avalon_st_source.vhd VHDL PATH hdl/cmos_sensor_input_avalon_st_source.vhd
add_fileset_file cmos_sensor_input.vhd VHDL PATH hdl/cmos_sensor_input.vhd TOP_LEVEL_FILE

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL cmos_sensor_input
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VHDL ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file cmos_sensor_input_constants.vhd VHDL PATH hdl/cmos_sensor_input_constants.vhd
add_fileset_file cmos_sensor_input_avalon_mm_slave.vhd VHDL PATH hdl/cmos_sensor_input_avalon_mm_slave.vhd
add_fileset_file cmos_sensor_input_synchronizer.vhd VHDL PATH hdl/cmos_sensor_input_synchronizer.vhd
add_fileset_file cmos_sensor_input_sampler.vhd VHDL PATH hdl/cmos_sensor_input_sampler.vhd
add_fileset_file cmos_sensor_input_sc_fifo.vhd VHDL PATH hdl/cmos_sensor_input_sc_fifo.vhd
add_fileset_file cmos_sensor_input_debayer.vhd VHDL PATH hdl/cmos_sensor_input_debayer.vhd
add_fileset_file cmos_sensor_input_packer.vhd VHDL PATH hdl/cmos_sensor_input_packer.vhd
add_fileset_file cmos_sensor_input_avalon_st_source.vhd VHDL PATH hdl/cmos_sensor_input_avalon_st_source.vhd
add_fileset_file cmos_sensor_input.vhd VHDL PATH hdl/cmos_sensor_input.vhd


#
# parameters
#
add_parameter PIX_DEPTH POSITIVE 8 "Depth of each pixel sample"
set_parameter_property PIX_DEPTH DISPLAY_NAME "Pixel Depth"
set_parameter_property PIX_DEPTH TYPE POSITIVE
set_parameter_property PIX_DEPTH UNITS bits
set_parameter_property PIX_DEPTH ALLOWED_RANGES {1:32}
set_parameter_property PIX_DEPTH DESCRIPTION "Depth of each pixel sample"
set_parameter_property PIX_DEPTH HDL_PARAMETER true

add_parameter SAMPLE_EDGE STRING RISING "Clock edge used for sampling data from the CMOS sensor"
set_parameter_property SAMPLE_EDGE DISPLAY_NAME "Edge Type"
set_parameter_property SAMPLE_EDGE TYPE STRING
set_parameter_property SAMPLE_EDGE UNITS None
set_parameter_property SAMPLE_EDGE ALLOWED_RANGES {RISING FALLING}
set_parameter_property SAMPLE_EDGE DESCRIPTION "Clock edge used for sampling data from the CMOS sensor"
set_parameter_property SAMPLE_EDGE HDL_PARAMETER true

add_parameter MAX_WIDTH POSITIVE 1920 "Maximum frame width"
set_parameter_property MAX_WIDTH DISPLAY_NAME "Maximum Frame Width"
set_parameter_property MAX_WIDTH TYPE POSITIVE
set_parameter_property MAX_WIDTH UNITS None
set_parameter_property MAX_WIDTH ALLOWED_RANGES {2:65535}
set_parameter_property MAX_WIDTH DESCRIPTION "Maximum frame width"
set_parameter_property MAX_WIDTH HDL_PARAMETER true

add_parameter MAX_HEIGHT POSITIVE 1080 "Maximum frame height"
set_parameter_property MAX_HEIGHT DISPLAY_NAME "Maximum Frame Height"
set_parameter_property MAX_HEIGHT TYPE POSITIVE
set_parameter_property MAX_HEIGHT UNITS None
set_parameter_property MAX_HEIGHT ALLOWED_RANGES {1:65535}
set_parameter_property MAX_HEIGHT DESCRIPTION "Maximum frame height"
set_parameter_property MAX_HEIGHT HDL_PARAMETER true

add_parameter OUTPUT_WIDTH POSITIVE 32 "Pixel width used as output from the component"
set_parameter_property OUTPUT_WIDTH DISPLAY_NAME "Pixel Output Data Width"
set_parameter_property OUTPUT_WIDTH TYPE POSITIVE
set_parameter_property OUTPUT_WIDTH UNITS bits
set_parameter_property OUTPUT_WIDTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property OUTPUT_WIDTH DESCRIPTION "Pixel width used as output from the component"
set_parameter_property OUTPUT_WIDTH HDL_PARAMETER true

add_parameter FIFO_DEPTH POSITIVE 32 "Output FIFO depth"
set_parameter_property FIFO_DEPTH DISPLAY_NAME "Output FIFO Depth"
set_parameter_property FIFO_DEPTH TYPE POSITIVE
set_parameter_property FIFO_DEPTH UNITS None
set_parameter_property FIFO_DEPTH ALLOWED_RANGES {8 16 32 64 128 256 512 1024}
set_parameter_property FIFO_DEPTH DESCRIPTION "Output FIFO depth"
set_parameter_property FIFO_DEPTH HDL_PARAMETER true

add_parameter DEVICE_FAMILY STRING "Cyclone V" "Target FPGA device family (only used for efficient FIFO instantiation)"
set_parameter_property DEVICE_FAMILY DISPLAY_NAME "FPGA Device Family"
set_parameter_property DEVICE_FAMILY TYPE STRING
set_parameter_property DEVICE_FAMILY UNITS None
set_parameter_property DEVICE_FAMILY ALLOWED_RANGES {"Cyclone V" "Cyclone IV E"}
set_parameter_property DEVICE_FAMILY DESCRIPTION "Target FPGA device family (only used for efficient FIFO instantiation)"
set_parameter_property DEVICE_FAMILY HDL_PARAMETER true

add_parameter DEBAYER_ENABLE BOOLEAN FALSE "Enable Debayering"
set_parameter_property DEBAYER_ENABLE DISPLAY_NAME "Enable Debayering"
set_parameter_property DEBAYER_ENABLE TYPE BOOLEAN
set_parameter_property DEBAYER_ENABLE UNITS None
set_parameter_property DEBAYER_ENABLE ALLOWED_RANGES {}
set_parameter_property DEBAYER_ENABLE DESCRIPTION "Enable Debayering"
set_parameter_property DEBAYER_ENABLE HDL_PARAMETER true
set_parameter_property DEBAYER_ENABLE ENABLED false

add_parameter PACKER_ENABLE BOOLEAN FALSE "Enable packing of multiple pixels into a single output word of size OUTPUT_WIDTH"
set_parameter_property PACKER_ENABLE DISPLAY_NAME "Enable Pixel Packer"
set_parameter_property PACKER_ENABLE TYPE BOOLEAN
set_parameter_property PACKER_ENABLE UNITS None
set_parameter_property PACKER_ENABLE ALLOWED_RANGES {}
set_parameter_property PACKER_ENABLE DESCRIPTION "Enable packing of multiple pixels into a single output word of size OUTPUT_WIDTH"
set_parameter_property PACKER_ENABLE HDL_PARAMETER true


#
# display items
#


#
# connection point clock
#
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


#
# connection point reset
#
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset reset Input 1


#
# connection point avalon_slave
#
add_interface avalon_slave avalon end
set_interface_property avalon_slave addressUnits WORDS
set_interface_property avalon_slave associatedClock clock
set_interface_property avalon_slave associatedReset reset
set_interface_property avalon_slave bitsPerSymbol 8
set_interface_property avalon_slave burstOnBurstBoundariesOnly false
set_interface_property avalon_slave burstcountUnits WORDS
set_interface_property avalon_slave explicitAddressSpan 0
set_interface_property avalon_slave holdTime 0
set_interface_property avalon_slave linewrapBursts false
set_interface_property avalon_slave maximumPendingReadTransactions 0
set_interface_property avalon_slave maximumPendingWriteTransactions 0
set_interface_property avalon_slave readLatency 0
set_interface_property avalon_slave readWaitTime 1
set_interface_property avalon_slave setupTime 0
set_interface_property avalon_slave timingUnits Cycles
set_interface_property avalon_slave writeWaitTime 0
set_interface_property avalon_slave ENABLED true
set_interface_property avalon_slave EXPORT_OF ""
set_interface_property avalon_slave PORT_NAME_MAP ""
set_interface_property avalon_slave CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave read read Input 1
add_interface_port avalon_slave write write Input 1
add_interface_port avalon_slave rddata readdata Output 32
add_interface_port avalon_slave wrdata writedata Input 32
add_interface_port avalon_slave addr address Input 2
set_interface_assignment avalon_slave embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave embeddedsw.configuration.isPrintableDevice 0


#
# connection point avalon_streaming_source
#
add_interface avalon_streaming_source avalon_streaming start
set_interface_property avalon_streaming_source associatedClock clock
set_interface_property avalon_streaming_source associatedReset reset
set_interface_property avalon_streaming_source dataBitsPerSymbol 8
set_interface_property avalon_streaming_source errorDescriptor ""
set_interface_property avalon_streaming_source firstSymbolInHighOrderBits true
set_interface_property avalon_streaming_source maxChannel 0
set_interface_property avalon_streaming_source readyLatency 1
set_interface_property avalon_streaming_source ENABLED true
set_interface_property avalon_streaming_source EXPORT_OF ""
set_interface_property avalon_streaming_source PORT_NAME_MAP ""
set_interface_property avalon_streaming_source CMSIS_SVD_VARIABLES ""
set_interface_property avalon_streaming_source SVD_ADDRESS_GROUP ""

add_interface_port avalon_streaming_source ready ready Input 1
add_interface_port avalon_streaming_source valid valid Output 1
add_interface_port avalon_streaming_source data_out data Output output_width


#
# connection point cmos_sensor
#
add_interface cmos_sensor conduit end
set_interface_property cmos_sensor associatedClock ""
set_interface_property cmos_sensor associatedReset ""
set_interface_property cmos_sensor ENABLED true
set_interface_property cmos_sensor EXPORT_OF ""
set_interface_property cmos_sensor PORT_NAME_MAP ""
set_interface_property cmos_sensor CMSIS_SVD_VARIABLES ""
set_interface_property cmos_sensor SVD_ADDRESS_GROUP ""

add_interface_port cmos_sensor frame_valid frame_valid Input 1
add_interface_port cmos_sensor line_valid line_valid Input 1
add_interface_port cmos_sensor data_in data Input pix_depth


#
# connection point interrupt_sender
#
add_interface interrupt_sender interrupt end
set_interface_property interrupt_sender associatedAddressablePoint ""
set_interface_property interrupt_sender associatedClock clock
set_interface_property interrupt_sender associatedReset reset
set_interface_property interrupt_sender bridgedReceiverOffset ""
set_interface_property interrupt_sender bridgesToReceiver ""
set_interface_property interrupt_sender ENABLED true
set_interface_property interrupt_sender EXPORT_OF ""
set_interface_property interrupt_sender PORT_NAME_MAP ""
set_interface_property interrupt_sender CMSIS_SVD_VARIABLES ""
set_interface_property interrupt_sender SVD_ADDRESS_GROUP ""

add_interface_port interrupt_sender irq irq Output 1
