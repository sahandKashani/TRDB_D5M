library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library osvvm;
use osvvm.RandomPkg.all;

use work.cmos_sensor_input_constants.all;
use work.cmos_sensor_output_generator_constants.all;

entity tb_cmos_sensor_input is
end tb_cmos_sensor_input;

architecture test of tb_cmos_sensor_input is
    -- 10 MHz -> 100 ns period. Duty cycle = 1/2.
    constant CLK_PERIOD      : time := 100 ns;
    constant CLK_HIGH_PERIOD : time := 50 ns;
    constant CLK_LOW_PERIOD  : time := 50 ns;

    signal clk   : std_logic;
    signal reset : std_logic;

    signal sim_finished : boolean := false;

    -- simulation parameters ---------------------------------------------------
    constant PIX_DEPTH       : positive                                                                      := 8;
    constant SAMPLE_EDGE     : string                                                                        := "RISING";
    constant MAX_WIDTH       : positive                                                                      := 1920;
    constant MAX_HEIGHT      : positive                                                                      := 1080;
    constant OUTPUT_WIDTH    : positive                                                                      := 32;
    constant FIFO_DEPTH      : positive                                                                      := 32;
    constant DEVICE_FAMILY   : string                                                                        := "Cyclone V";
    constant DEBAYER_ENABLE  : boolean                                                                       := false;
    constant PACKER_ENABLE   : boolean                                                                       := false;
    constant DEBAYER_PATTERN : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0) := CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB;

    constant FRAME_WIDTH       : positive := 5;
    constant FRAME_HEIGHT      : positive := 4;
    constant FRAME_FRAME_BLANK : positive := 1;
    constant FRAME_LINE_BLANK  : natural  := 1;
    constant LINE_LINE_BLANK   : positive := 1;
    constant LINE_FRAME_BLANK  : natural  := 1;

    constant BUS_BUSY_THRESHOLD : natural range 0 to 100 := 100;

    -- cmos_sensor_output_generator --------------------------------------------
    signal cmos_sensor_output_generator_addr        : std_logic_vector(2 downto 0);
    signal cmos_sensor_output_generator_read        : std_logic;
    signal cmos_sensor_output_generator_write       : std_logic;
    signal cmos_sensor_output_generator_rddata      : std_logic_vector(CMOS_SENSOR_OUTPUT_GENERATOR_MM_S_DATA_WIDTH - 1 downto 0);
    signal cmos_sensor_output_generator_wrdata      : std_logic_vector(CMOS_SENSOR_OUTPUT_GENERATOR_MM_S_DATA_WIDTH - 1 downto 0);
    signal cmos_sensor_output_generator_frame_valid : std_logic;
    signal cmos_sensor_output_generator_line_valid  : std_logic;
    signal cmos_sensor_output_generator_data        : std_logic_vector(PIX_DEPTH - 1 downto 0);

    -- cmos_sensor_input -------------------------------------------------------
    signal cmos_sensor_input_ready    : std_logic;
    signal cmos_sensor_input_valid    : std_logic;
    signal cmos_sensor_input_data_out : std_logic_vector(OUTPUT_WIDTH - 1 downto 0);
    signal cmos_sensor_input_addr     : std_logic_vector(1 downto 0);
    signal cmos_sensor_input_read     : std_logic;
    signal cmos_sensor_input_write    : std_logic;
    signal cmos_sensor_input_rddata   : std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);
    signal cmos_sensor_input_wrdata   : std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);
    signal cmos_sensor_input_irq      : std_logic;

begin
    clk_generation : process
    begin
        if not sim_finished then
            clk <= '1';
            wait for CLK_HIGH_PERIOD;
            clk <= '0';
            wait for CLK_LOW_PERIOD;
        else
            wait;
        end if;
    end process clk_generation;

    cmos_sensor_input_ready_generation : process
        variable rand_gen : RandomPType;
        variable rint     : integer;
    begin
        rand_gen.InitSeed(rand_gen'instance_name);
        rand_gen.SetRandomParm(UNIFORM);

        while true loop
            if not sim_finished then
                wait until falling_edge(clk);
                rint := rand_gen.RandInt(0, 100);

                if rint < BUS_BUSY_THRESHOLD then
                    cmos_sensor_input_ready <= '0';
                else
                    cmos_sensor_input_ready <= '1';
                end if;
            else
                wait;
            end if;
        end loop;

    end process cmos_sensor_input_ready_generation;

    cmos_sensor_output_generator_inst : entity work.cmos_sensor_output_generator
        generic map(PIX_DEPTH  => PIX_DEPTH,
                    MAX_WIDTH  => MAX_WIDTH,
                    MAX_HEIGHT => MAX_HEIGHT)
        port map(clk         => clk,
                 reset       => reset,
                 addr        => cmos_sensor_output_generator_addr,
                 read        => cmos_sensor_output_generator_read,
                 write       => cmos_sensor_output_generator_write,
                 rddata      => cmos_sensor_output_generator_rddata,
                 wrdata      => cmos_sensor_output_generator_wrdata,
                 frame_valid => cmos_sensor_output_generator_frame_valid,
                 line_valid  => cmos_sensor_output_generator_line_valid,
                 data        => cmos_sensor_output_generator_data);

    cmos_sensor_input_inst : entity work.cmos_sensor_input
        generic map(PIX_DEPTH      => PIX_DEPTH,
                    SAMPLE_EDGE    => SAMPLE_EDGE,
                    MAX_WIDTH      => MAX_WIDTH,
                    MAX_HEIGHT     => MAX_HEIGHT,
                    OUTPUT_WIDTH   => OUTPUT_WIDTH,
                    FIFO_DEPTH     => FIFO_DEPTH,
                    DEVICE_FAMILY  => DEVICE_FAMILY,
                    DEBAYER_ENABLE => DEBAYER_ENABLE,
                    PACKER_ENABLE  => PACKER_ENABLE)
        port map(clk         => clk,
                 reset       => reset,
                 frame_valid => cmos_sensor_output_generator_frame_valid,
                 line_valid  => cmos_sensor_output_generator_line_valid,
                 data_in     => cmos_sensor_output_generator_data,
                 ready       => cmos_sensor_input_ready,
                 valid       => cmos_sensor_input_valid,
                 data_out    => cmos_sensor_input_data_out,
                 addr        => cmos_sensor_input_addr,
                 read        => cmos_sensor_input_read,
                 write       => cmos_sensor_input_write,
                 rddata      => cmos_sensor_input_rddata,
                 wrdata      => cmos_sensor_input_wrdata,
                 irq         => cmos_sensor_input_irq);

    sim : process
        function configuration_valid return boolean is
            constant MIN_OUTPUT_WIDTH_DEBAYER_DISABLE_PACKER_DISABLE : positive := 1 * PIX_DEPTH;
            constant MIN_OUTPUT_WIDTH_DEBAYER_DISABLE_PACKER_ENABLE  : positive := 2 * PIX_DEPTH;
            constant MIN_OUTPUT_WIDTH_DEBAYER_ENABLE_PACKER_DISABLE  : positive := 3 * PIX_DEPTH;
            constant MIN_OUTPUT_WIDTH_DEBAYER_ENABLE_PACKER_ENABLE   : positive := 6 * PIX_DEPTH;

        begin
            if not ((1 <= PIX_DEPTH) and (PIX_DEPTH <= 32)) then
                assert false
                    report "PIX_DEPTH must be in range {1:32}"
                    severity error;

                return false;
            end if;

            if not ((SAMPLE_EDGE = "RISING") or (SAMPLE_EDGE = "FALLING")) then
                assert false
                    report "SAMPLE_EDGE must be {RISING, FALLING}"
                    severity error;

                return false;
            end if;

            if not ((2 <= MAX_WIDTH) and (MAX_WIDTH <= 65535)) then
                assert false
                    report "MAX_WIDTH must be in range {2:65535}"
                    severity error;

                return false;
            end if;

            if not ((1 <= MAX_HEIGHT) and (MAX_HEIGHT <= 65535)) then
                assert false
                    report "MAX_HEIGHT must be in range {1:65535}"
                    severity error;

                return false;
            end if;

            if not ((OUTPUT_WIDTH = 8) or (OUTPUT_WIDTH = 16) or (OUTPUT_WIDTH = 32) or (OUTPUT_WIDTH = 64) or (OUTPUT_WIDTH = 128) or (OUTPUT_WIDTH = 256) or (OUTPUT_WIDTH = 512) or (OUTPUT_WIDTH = 1024)) then
                assert false
                    report "OUTPUT_WIDTH must be {8, 16, 32, 64, 128, 256, 512, 1024}"
                    severity error;

                return false;
            end if;

            if not ((FIFO_DEPTH = 8) or (FIFO_DEPTH = 16) or (FIFO_DEPTH = 32) or (FIFO_DEPTH = 64) or (FIFO_DEPTH = 128) or (FIFO_DEPTH = 256) or (FIFO_DEPTH = 512) or (FIFO_DEPTH = 1024)) then
                assert false
                    report "FIFO_DEPTH must be {8, 16, 32, 64, 128, 256, 512, 1024}"
                    severity error;

                return false;
            end if;

            if not ((DEVICE_FAMILY = "Cyclone IV E") or (DEVICE_FAMILY = "Cyclone V")) then
                assert false
                    report "DEVICE_FAMILY must be {Cyclone IV E, Cyclone V}"
                    severity error;

                return false;
            end if;

            if not DEBAYER_ENABLE and not PACKER_ENABLE then
                if OUTPUT_WIDTH < MIN_OUTPUT_WIDTH_DEBAYER_DISABLE_PACKER_DISABLE then
                    assert false
                        report "OUTPUT_WIDTH must be larger or equal to " & integer'image(MIN_OUTPUT_WIDTH_DEBAYER_DISABLE_PACKER_DISABLE)
                        severity error;

                    return false;
                end if;
            elsif not DEBAYER_ENABLE and PACKER_ENABLE then
                if OUTPUT_WIDTH < MIN_OUTPUT_WIDTH_DEBAYER_DISABLE_PACKER_ENABLE then
                    assert false
                        report "OUTPUT_WIDTH must be larger or equal to " & integer'image(MIN_OUTPUT_WIDTH_DEBAYER_DISABLE_PACKER_ENABLE)
                        severity error;

                    return false;
                end if;
            elsif DEBAYER_ENABLE and not PACKER_ENABLE then
                if OUTPUT_WIDTH < MIN_OUTPUT_WIDTH_DEBAYER_ENABLE_PACKER_DISABLE then
                    assert false
                        report "OUTPUT_WIDTH must be larger or equal to " & integer'image(MIN_OUTPUT_WIDTH_DEBAYER_ENABLE_PACKER_DISABLE)
                        severity error;

                    return false;
                end if;
            elsif DEBAYER_ENABLE and PACKER_ENABLE then
                if OUTPUT_WIDTH < MIN_OUTPUT_WIDTH_DEBAYER_ENABLE_PACKER_ENABLE then
                    assert false
                        report "OUTPUT_WIDTH must be larger or equal to " & integer'image(MIN_OUTPUT_WIDTH_DEBAYER_ENABLE_PACKER_ENABLE)
                        severity error;

                    return false;
                end if;
            end if;

            if not ((2 <= FRAME_WIDTH) and (FRAME_WIDTH <= MAX_WIDTH)) then
                assert false
                    report "FRAME_WIDTH must be in range {1:" & integer'image(MAX_WIDTH) & "}"
                    severity error;

                return false;
            end if;

            if not ((2 <= FRAME_HEIGHT) and (FRAME_HEIGHT <= MAX_HEIGHT)) then
                assert false
                    report "FRAME_HEIGHT must be in range {1:" & integer'image(MAX_HEIGHT) & "}"
                    severity error;

                return false;
            end if;

            return true;

        end function configuration_valid;

        procedure async_reset is
        begin
            wait until rising_edge(clk);
            wait for CLK_PERIOD / 4;
            reset <= '1';

            wait for CLK_PERIOD / 2;
            reset <= '0';
        end procedure async_reset;

        procedure setup_cmos_sensor_output_generator is
            procedure write_register(constant ofst : in std_logic_vector;
                                     constant val  : in natural) is
            begin
                wait until falling_edge(clk);
                cmos_sensor_output_generator_addr   <= ofst;
                cmos_sensor_output_generator_write  <= '1';
                cmos_sensor_output_generator_wrdata <= std_logic_vector(to_unsigned(val, cmos_sensor_output_generator_wrdata'length));

                wait until falling_edge(clk);
                cmos_sensor_output_generator_addr   <= (others => '0');
                cmos_sensor_output_generator_write  <= '0';
                cmos_sensor_output_generator_wrdata <= (others => '0');
            end procedure write_register;

            procedure write_register(constant ofst : in std_logic_vector;
                                     constant val  : in std_logic_vector) is
            begin
                wait until falling_edge(clk);
                cmos_sensor_output_generator_addr   <= ofst;
                cmos_sensor_output_generator_write  <= '1';
                cmos_sensor_output_generator_wrdata <= std_logic_vector(resize(unsigned(val), cmos_sensor_output_generator_wrdata'length));

                wait until falling_edge(clk);
                cmos_sensor_output_generator_addr   <= (others => '0');
                cmos_sensor_output_generator_write  <= '0';
                cmos_sensor_output_generator_wrdata <= (others => '0');
            end procedure write_register;

        begin
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_CONFIG_FRAME_WIDTH_OFST, FRAME_WIDTH);
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_CONFIG_FRAME_HEIGHT_OFST, FRAME_HEIGHT);
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_CONFIG_FRAME_FRAME_BLANK_OFST, FRAME_FRAME_BLANK);
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_CONFIG_FRAME_LINE_BLANK_OFST, FRAME_LINE_BLANK);
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_CONFIG_LINE_LINE_BLANK_OFST, LINE_LINE_BLANK);
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_CONFIG_LINE_FRAME_BLANK_OFST, LINE_FRAME_BLANK);
            write_register(CMOS_SENSOR_OUTPUT_GENERATOR_COMMAND_OFST, CMOS_SENSOR_OUTPUT_GENERATOR_COMMAND_START);
        end procedure setup_cmos_sensor_output_generator;

        procedure sim_cmos_sensor_input is
            procedure write_command_register(constant data : in std_logic_vector) is
            begin
                wait until falling_edge(clk);
                cmos_sensor_input_addr                                                                                          <= CMOS_SENSOR_INPUT_COMMAND_OFST;
                cmos_sensor_input_write                                                                                         <= '1';
                cmos_sensor_input_wrdata(CMOS_SENSOR_INPUT_COMMAND_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_COMMAND_LOW_BIT_OFST) <= data;

                wait until falling_edge(clk);
                cmos_sensor_input_addr   <= (others => '0');
                cmos_sensor_input_write  <= '0';
                cmos_sensor_input_wrdata <= (others => '0');
            end procedure write_command_register;

            procedure write_config_register(constant irq             : in boolean;
                                            constant debayer_pattern : in std_logic_vector) is
            begin
                wait until falling_edge(clk);
                cmos_sensor_input_addr   <= CMOS_SENSOR_INPUT_CONFIG_OFST;
                cmos_sensor_input_write  <= '1';
                cmos_sensor_input_wrdata <= (others => '0');

                if irq then
                    cmos_sensor_input_wrdata(CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE;
                else
                    cmos_sensor_input_wrdata(CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE;
                end if;

                cmos_sensor_input_wrdata(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_LOW_BIT_OFST) <= debayer_pattern;

                wait until falling_edge(clk);
                cmos_sensor_input_addr   <= (others => '0');
                cmos_sensor_input_write  <= '0';
                cmos_sensor_input_wrdata <= (others => '0');
            end procedure write_config_register;

            procedure read_frame_info_register is
            begin
                wait until falling_edge(clk);
                cmos_sensor_input_addr <= CMOS_SENSOR_INPUT_FRAME_INFO_OFST;
                cmos_sensor_input_read <= '1';

                wait until falling_edge(clk);
                cmos_sensor_input_addr <= (others => '0');
                cmos_sensor_input_read <= '0';
            end procedure read_frame_info_register;

            procedure read_status_register is
            begin
                wait until falling_edge(clk);
                cmos_sensor_input_addr <= CMOS_SENSOR_INPUT_STATUS_OFST;
                cmos_sensor_input_read <= '1';

                wait until falling_edge(clk);
                cmos_sensor_input_addr <= (others => '0');
                cmos_sensor_input_read <= '0';
            end procedure read_status_register;

            procedure wait_until_idle is
                variable end_loop : boolean := false;
            begin
                while not end_loop loop
                    wait until falling_edge(clk);
                    cmos_sensor_input_addr <= CMOS_SENSOR_INPUT_STATUS_OFST;
                    cmos_sensor_input_read <= '1';

                    wait until falling_edge(clk);
                    cmos_sensor_input_addr <= (others => '0');
                    cmos_sensor_input_read <= '0';
                    if cmos_sensor_input_rddata(CMOS_SENSOR_INPUT_STATUS_STATE_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_STATUS_STATE_LOW_BIT_OFST) = CMOS_SENSOR_INPUT_STATUS_STATE_IDLE then
                        end_loop := true;
                    end if;
                end loop;
            end procedure wait_until_idle;

            procedure wait_clock_cycles(constant count : in positive) is
            begin
                wait for count * CLK_PERIOD;
            end procedure wait_clock_cycles;

            procedure noIrq is
            begin
                write_command_register(CMOS_SENSOR_INPUT_COMMAND_STOP_AND_RESET);
                wait_until_idle;

                write_config_register(false, DEBAYER_PATTERN);
                wait_until_idle;

                write_command_register(CMOS_SENSOR_INPUT_COMMAND_GET_FRAME_INFO);
                wait_until_idle;

                read_frame_info_register;

                write_command_register(CMOS_SENSOR_INPUT_COMMAND_SNAPSHOT);
                wait_until_idle;
            end procedure noIrq;

            procedure withIrq is
            begin
                write_command_register(CMOS_SENSOR_INPUT_COMMAND_STOP_AND_RESET);
                wait_until_idle;

                write_config_register(true, DEBAYER_PATTERN);
                wait_until_idle;

                write_command_register(CMOS_SENSOR_INPUT_COMMAND_GET_FRAME_INFO);
                if cmos_sensor_input_irq = '0' then
                    wait until cmos_sensor_input_irq = '1';
                end if;
                write_command_register(CMOS_SENSOR_INPUT_COMMAND_IRQ_ACK);
                wait_until_idle;

                read_frame_info_register;

                write_command_register(CMOS_SENSOR_INPUT_COMMAND_SNAPSHOT);
                if cmos_sensor_input_irq = '0' then
                    wait until cmos_sensor_input_irq = '1';
                end if;
                write_command_register(CMOS_SENSOR_INPUT_COMMAND_IRQ_ACK);
                wait_until_idle;
            end procedure withIrq;

        begin
            --noIrq;
            withIrq;

        end procedure sim_cmos_sensor_input;

    begin
        if configuration_valid then
            async_reset;
            setup_cmos_sensor_output_generator;
            sim_cmos_sensor_input;
        else
        end if;

        sim_finished <= true;
        wait;
    end process sim;

end architecture test;
