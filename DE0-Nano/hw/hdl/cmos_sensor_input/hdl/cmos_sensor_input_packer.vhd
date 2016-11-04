library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input_packer is
    generic(
        PIX_DEPTH  : positive;
        PACK_WIDTH : positive
    );
    port(
        clk               : in  std_logic;
        reset             : in  std_logic;

        -- avalon_mm_slave
        stop_and_reset    : in  std_logic;

        -- sampler / debayer
        valid_in          : in  std_logic;
        data_in           : in  std_logic_vector(PIX_DEPTH - 1 downto 0);
        start_of_frame_in : in  std_logic;
        end_of_frame_in   : in  std_logic;

        -- fifo
        valid_out         : out std_logic;
        data_out          : out std_logic_vector(PACK_WIDTH - 1 downto 0);
        end_of_frame_out  : out std_logic
    );
end entity cmos_sensor_input_packer;

architecture rtl of cmos_sensor_input_packer is
    constant COMPRESSED_PIX_COUNT : positive := floor_div(data_out'length, PIX_DEPTH);

    signal reg_count    : unsigned(bit_width(COMPRESSED_PIX_COUNT) - 1 downto 0);
    signal reg_data_out : std_logic_vector((COMPRESSED_PIX_COUNT - 1) * PIX_DEPTH - 1 downto 0);

begin
    process(clk, reset)
    begin
        if reset = '1' then
            reg_count    <= (others => '0');
            reg_data_out <= (others => '0');

        elsif rising_edge(clk) then
            valid_out        <= '0';
            data_out         <= (others => '0');
            end_of_frame_out <= '0';

            if stop_and_reset = '1' then
                reg_count    <= to_unsigned(0, reg_count'length);
                reg_data_out <= (others => '0');
            else
                if valid_in = '1' then
                    if start_of_frame_in = '1' then
                        reg_count                                              <= to_unsigned(1, reg_count'length);
                        reg_data_out(PIX_DEPTH - 1 downto 0)                   <= data_in;
                        reg_data_out(reg_data_out'length - 1 downto PIX_DEPTH) <= (others => '0');

                    elsif end_of_frame_in = '1' then
                        valid_out                                                      <= '1';
                        data_out(PIX_DEPTH - 1 downto 0)                               <= data_in;
                        data_out(reg_data_out'length + PIX_DEPTH - 1 downto PIX_DEPTH) <= reg_data_out;
                        end_of_frame_out                                               <= '1';

                        reg_count <= to_unsigned(0, reg_count'length);

                    elsif reg_count < COMPRESSED_PIX_COUNT - 1 then
                        reg_count                                              <= reg_count + 1;
                        reg_data_out(PIX_DEPTH - 1 downto 0)                   <= data_in;
                        reg_data_out(reg_data_out'length - 1 downto PIX_DEPTH) <= reg_data_out(reg_data_out'length - PIX_DEPTH - 1 downto 0);

                    elsif reg_count = COMPRESSED_PIX_COUNT - 1 then
                        valid_out                                                      <= '1';
                        data_out(PIX_DEPTH - 1 downto 0)                               <= data_in;
                        data_out(reg_data_out'length + PIX_DEPTH - 1 downto PIX_DEPTH) <= reg_data_out;

                        reg_count    <= to_unsigned(0, reg_count'length);
                        reg_data_out <= (others => '0');
                    end if;
                end if;
            end if;
        end if;
    end process;

end architecture rtl;
