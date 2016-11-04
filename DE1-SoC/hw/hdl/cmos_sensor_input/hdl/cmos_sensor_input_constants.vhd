library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

package cmos_sensor_input_constants is
    constant CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH : positive := 32;

    -- register offsets
    constant CMOS_SENSOR_INPUT_CONFIG_OFST     : std_logic_vector(1 downto 0) := "00"; -- RW
    constant CMOS_SENSOR_INPUT_COMMAND_OFST    : std_logic_vector(1 downto 0) := "01"; -- WO
    constant CMOS_SENSOR_INPUT_STATUS_OFST     : std_logic_vector(1 downto 0) := "10"; -- RO
    constant CMOS_SENSOR_INPUT_FRAME_INFO_OFST : std_logic_vector(1 downto 0) := "11"; -- RO

    -- CONFIG register
    constant CMOS_SENSOR_INPUT_CONFIG_IRQ_BIT_OFST      : natural                                                           := 0;
    constant CMOS_SENSOR_INPUT_CONFIG_IRQ_WIDTH         : positive                                                          := 1;
    constant CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST  : natural                                                           := CMOS_SENSOR_INPUT_CONFIG_IRQ_BIT_OFST;
    constant CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST : natural                                                           := CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST + CMOS_SENSOR_INPUT_CONFIG_IRQ_WIDTH - 1;
    constant CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE       : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_IRQ_WIDTH - 1 downto 0) := "0";
    constant CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE        : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_IRQ_WIDTH - 1 downto 0) := "1";

    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BIT_OFST      : natural                                                                       := CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST + 1;
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH         : positive                                                                      := 2;
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_LOW_BIT_OFST  : natural                                                                       := CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BIT_OFST;
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_HIGH_BIT_OFST : natural                                                                       := CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_LOW_BIT_OFST + CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1;
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB          : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0) := "00";
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_BGGR          : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0) := "01";
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GRBG          : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0) := "10";
    constant CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_GBRG          : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0) := "11";

    -- COMMAND register
    constant CMOS_SENSOR_INPUT_COMMAND_BIT_OFST       : natural                                                        := 0;
    constant CMOS_SENSOR_INPUT_COMMAND_WIDTH          : positive                                                       := CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH;
    constant CMOS_SENSOR_INPUT_COMMAND_LOW_BIT_OFST   : natural                                                        := CMOS_SENSOR_INPUT_COMMAND_BIT_OFST;
    constant CMOS_SENSOR_INPUT_COMMAND_HIGH_BIT_OFST  : natural                                                        := CMOS_SENSOR_INPUT_COMMAND_LOW_BIT_OFST + CMOS_SENSOR_INPUT_COMMAND_WIDTH - 1;
    constant CMOS_SENSOR_INPUT_COMMAND_GET_FRAME_INFO : std_logic_vector(CMOS_SENSOR_INPUT_COMMAND_WIDTH - 1 downto 0) := X"00000000";
    constant CMOS_SENSOR_INPUT_COMMAND_SNAPSHOT       : std_logic_vector(CMOS_SENSOR_INPUT_COMMAND_WIDTH - 1 downto 0) := X"00000001";
    constant CMOS_SENSOR_INPUT_COMMAND_IRQ_ACK        : std_logic_vector(CMOS_SENSOR_INPUT_COMMAND_WIDTH - 1 downto 0) := X"00000002";
    constant CMOS_SENSOR_INPUT_COMMAND_STOP_AND_RESET : std_logic_vector(CMOS_SENSOR_INPUT_COMMAND_WIDTH - 1 downto 0) := X"00000003";

    -- STATUS register
    constant CMOS_SENSOR_INPUT_STATUS_STATE_BIT_OFST      : natural                                                             := 0;
    constant CMOS_SENSOR_INPUT_STATUS_STATE_WIDTH         : positive                                                            := 1;
    constant CMOS_SENSOR_INPUT_STATUS_STATE_LOW_BIT_OFST  : natural                                                             := CMOS_SENSOR_INPUT_STATUS_STATE_BIT_OFST;
    constant CMOS_SENSOR_INPUT_STATUS_STATE_HIGH_BIT_OFST : natural                                                             := CMOS_SENSOR_INPUT_STATUS_STATE_LOW_BIT_OFST + CMOS_SENSOR_INPUT_STATUS_STATE_WIDTH - 1;
    constant CMOS_SENSOR_INPUT_STATUS_STATE_IDLE          : std_logic_vector(CMOS_SENSOR_INPUT_STATUS_STATE_WIDTH - 1 downto 0) := "0";
    constant CMOS_SENSOR_INPUT_STATUS_STATE_BUSY          : std_logic_vector(CMOS_SENSOR_INPUT_STATUS_STATE_WIDTH - 1 downto 0) := "1";

    constant CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_BIT_OFST      : natural                                                                 := CMOS_SENSOR_INPUT_STATUS_STATE_HIGH_BIT_OFST + 1;
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_WIDTH         : positive                                                                := 1;
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_LOW_BIT_OFST  : natural                                                                 := CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_BIT_OFST;
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_HIGH_BIT_OFST : natural                                                                 := CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_LOW_BIT_OFST + CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_WIDTH - 1;
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_NO_OVERFLOW   : std_logic_vector(CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_WIDTH - 1 downto 0) := "0";
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW      : std_logic_vector(CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_WIDTH - 1 downto 0) := "1";

    constant CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_BIT_OFST      : natural  := CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_HIGH_BIT_OFST + 1;
    -- max fifo depth is 1024 elements (based on _hw.tcl), so need 11 bits to represent 1024
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_WIDTH         : positive := 11;
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_LOW_BIT_OFST  : natural  := CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_BIT_OFST;
    constant CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_HIGH_BIT_0FST : natural  := CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_LOW_BIT_OFST + CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_WIDTH - 1;

    -- FRAME_INFO register
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_BIT_OFST      : natural  := 0;
    -- takes up half the space of the bus width --> max frame width is 65535
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_WIDTH         : positive := CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH / 2;
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_LOW_BIT_OFST  : natural  := CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_BIT_OFST;
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_HIGH_BIT_OFST : natural  := CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_LOW_BIT_OFST + CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_WIDTH - 1;

    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_BIT_OFST      : natural  := CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_HIGH_BIT_OFST + 1;
    -- takes up half the space of the bus width --> max frame height is 65535
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_WIDTH         : positive := CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH / 2;
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_LOW_BIT_OFST  : natural  := CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_BIT_OFST;
    constant CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_HIGH_BIT_OFST : natural  := CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_LOW_BIT_OFST + CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_WIDTH - 1;

    function ceil_log2(num : positive) return natural;
    function floor_div(numerator : positive; denominator : positive) return natural;
    function bit_width(num : positive) return positive;
    function max(left : positive; right : positive) return positive;

end package cmos_sensor_input_constants;

package body cmos_sensor_input_constants is
    function ceil_log2(num : positive) return natural is
    begin
        return integer(ceil(log2(real(num))));
    end function ceil_log2;

    function floor_div(numerator : positive; denominator : positive) return natural is
    begin
        return integer(floor(real(numerator) / real(denominator)));
    end function floor_div;

    function bit_width(num : positive) return positive is
    begin
        return ceil_log2(num + 1);
    end function bit_width;

    function max(left : positive; right : positive) return positive is
    begin
        if left > right then
            return left;
        else
            return right;
        end if;
    end max;

end package body cmos_sensor_input_constants;

