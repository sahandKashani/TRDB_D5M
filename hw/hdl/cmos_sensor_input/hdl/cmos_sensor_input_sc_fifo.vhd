library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library altera_mf;
use altera_mf.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input_sc_fifo is
    generic(
        constant DATA_WIDTH    : positive;
        constant FIFO_DEPTH    : positive;
        constant DEVICE_FAMILY : string
    );
    port(
        clk      : in  std_logic;
        reset    : in  std_logic;
        clr      : in  std_logic;
        data_in  : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        data_out : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        read     : in  std_logic;
        write    : in  std_logic;
        empty    : out std_logic;
        full     : out std_logic;
        usedw    : out std_logic_vector(bit_width(FIFO_DEPTH) - 1 downto 0);
        overflow : out std_logic
    );
end cmos_sensor_input_sc_fifo;

architecture rtl of cmos_sensor_input_sc_fifo is
    component scfifo
        generic(
            add_ram_output_register : string;
            intended_device_family  : string;
            lpm_numwords            : natural;
            lpm_showahead           : string;
            lpm_type                : string;
            lpm_width               : natural;
            lpm_widthu              : natural;
            overflow_checking       : string;
            underflow_checking      : string;
            use_eab                 : string
        );
        port(
            clock : in  std_logic;
            data  : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
            rdreq : in  std_logic;
            sclr  : in  std_logic;
            wrreq : in  std_logic;
            empty : out std_logic;
            full  : out std_logic;
            q     : out std_logic_vector(DATA_WIDTH - 1 downto 0);
            usedw : out std_logic_vector(bit_width(FIFO_DEPTH) - 2 downto 0)
        );
    end component;

    signal scfifo_clock : std_logic;
    signal scfifo_data  : std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal scfifo_rdreq : std_logic;
    signal scfifo_sclr  : std_logic;
    signal scfifo_wrreq : std_logic;
    signal scfifo_empty : std_logic;
    signal scfifo_full  : std_logic;
    signal scfifo_q     : std_logic_vector(DATA_WIDTH - 1 downto 0);
    signal scfifo_usedw : std_logic_vector(bit_width(FIFO_DEPTH) - 2 downto 0);

    signal reg_overflow : std_logic;

begin
    FIFO_OVERFLOW : process(clk, reset)
    begin
        if reset = '1' then
            reg_overflow <= '0';
        elsif rising_edge(clk) then
            if clr = '1' then
                reg_overflow <= '0';
            else
                reg_overflow <= reg_overflow or (write and scfifo_full);
            end if;
        end if;
    end process;

    OUTPUTS : process(reg_overflow, scfifo_empty, scfifo_full, scfifo_q, scfifo_usedw)
    begin
        data_out <= scfifo_q;
        empty    <= scfifo_empty;
        full     <= scfifo_full;
        overflow <= reg_overflow;

        if scfifo_full = '1' then
            usedw <= std_logic_vector(to_unsigned(FIFO_DEPTH, usedw'length));
        elsif scfifo_empty = '1' then
            usedw <= std_logic_vector(to_unsigned(0, usedw'length));
        else
            usedw <= std_logic_vector(resize(unsigned(scfifo_usedw), usedw'length));
        end if;
    end process;

    -- scfifo connections ------------------------------------------------------
    scfifo_clock <= clk;
    scfifo_data  <= data_in;
    scfifo_rdreq <= read and not scfifo_empty;
    scfifo_sclr  <= clr;
    scfifo_wrreq <= write and not scfifo_full;

    scfifo_component : scfifo
        generic map(
            add_ram_output_register => "OFF",
            intended_device_family  => DEVICE_FAMILY,
            lpm_numwords            => FIFO_DEPTH,
            lpm_showahead           => "ON",
            lpm_type                => "scfifo",
            lpm_width               => DATA_WIDTH,
            lpm_widthu              => bit_width(FIFO_DEPTH) - 1,
            overflow_checking       => "OFF",
            underflow_checking      => "OFF",
            use_eab                 => "ON"
        )
        port map(
            clock => scfifo_clock,
            data  => scfifo_data,
            rdreq => scfifo_rdreq,
            sclr  => scfifo_sclr,
            wrreq => scfifo_wrreq,
            empty => scfifo_empty,
            full  => scfifo_full,
            q     => scfifo_q,
            usedw => scfifo_usedw
        );

end rtl;
