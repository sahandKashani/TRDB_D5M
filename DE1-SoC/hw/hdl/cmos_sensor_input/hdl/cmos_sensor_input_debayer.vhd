library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input_debayer is
    generic(
        PIX_DEPTH_RAW : positive;
        PIX_DEPTH_RGB : positive;
        MAX_WIDTH     : positive
    );
    port(
        clk                : in  std_logic;
        reset              : in  std_logic;

        -- avalon_mm_slave
        stop_and_reset     : in  std_logic;
        debayer_pattern    : in  std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0);

        -- sampler
        valid_in           : in  std_logic;
        data_in            : in  std_logic_vector(PIX_DEPTH_RAW - 1 downto 0);
        start_of_frame_in  : in  std_logic;
        end_of_frame_in    : in  std_logic;

        -- packer / fifo
        valid_out          : out std_logic;
        data_out           : out std_logic_vector(PIX_DEPTH_RGB - 1 downto 0);
        start_of_frame_out : out std_logic;
        end_of_frame_out   : out std_logic
    );
end entity cmos_sensor_input_debayer;

architecture rtl of cmos_sensor_input_debayer is
begin
    valid_out          <= valid_in;
    data_out           <= std_logic_vector(resize(unsigned(data_in), data_out'length));
    start_of_frame_out <= start_of_frame_in;
    end_of_frame_out   <= end_of_frame_in;
end architecture rtl;
