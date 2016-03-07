library ieee;
use ieee.std_logic_1164.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input_synchronizer is
    generic(
        PIX_DEPTH   : positive;
        SAMPLE_EDGE : string
    );
    port(
        clk             : in  std_logic;
        reset           : in  std_logic;

        -- cmos sensor
        frame_valid_in  : in  std_logic;
        line_valid_in   : in  std_logic;
        data_in         : in  std_logic_vector(PIX_DEPTH - 1 downto 0);

        -- sampler
        frame_valid_out : out std_logic;
        line_valid_out  : out std_logic;
        data_out        : out std_logic_vector(PIX_DEPTH - 1 downto 0)
    );
end entity cmos_sensor_input_synchronizer;

architecture rtl of cmos_sensor_input_synchronizer is
    signal reg_frame_valid_in : std_logic;
    signal reg_line_valid_in  : std_logic;
    signal reg_data_in        : std_logic_vector(data_in'range);

    -- registered outputs
    signal reg_frame_valid_out : std_logic;
    signal reg_line_valid_out  : std_logic;
    signal reg_data_out        : std_logic_vector(data_out'range);

begin
    -- registered outputs
    frame_valid_out <= reg_frame_valid_out;
    line_valid_out  <= reg_line_valid_out;
    data_out        <= reg_data_out;

    process(clk, reset)
    begin
        if SAMPLE_EDGE = "RISING" then
            if reset = '1' then
                reg_frame_valid_in <= '0';
                reg_line_valid_in  <= '0';
                reg_data_in        <= (others => '0');
            elsif rising_edge(clk) then
                reg_frame_valid_in <= frame_valid_in;
                reg_line_valid_in  <= line_valid_in;
                reg_data_in        <= data_in;
            end if;
        elsif SAMPLE_EDGE = "FALLING" then
            if reset = '1' then
                reg_frame_valid_in <= '0';
                reg_line_valid_in  <= '0';
                reg_data_in        <= (others => '0');
            elsif falling_edge(clk) then
                reg_frame_valid_in <= frame_valid_in;
                reg_line_valid_in  <= line_valid_in;
                reg_data_in        <= data_in;
            end if;
        end if;
    end process;

    process(clk, reset)
    begin
        if reset = '1' then
            reg_frame_valid_out <= '0';
            reg_line_valid_out  <= '0';
            reg_data_out        <= (others => '0');
        elsif rising_edge(clk) then
            reg_frame_valid_out <= reg_frame_valid_in;
            reg_line_valid_out  <= reg_line_valid_in;
            reg_data_out        <= reg_data_in;
        end if;
    end process;

end architecture rtl;
