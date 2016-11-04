library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input is
    generic(
        PIX_DEPTH      : positive;
        SAMPLE_EDGE    : string;
        MAX_WIDTH      : positive range 2 to 65535; -- does not support images with only 1 column (in order for start_of_frame and end_of_frame not to overlap)
        MAX_HEIGHT     : positive range 1 to 65535; -- but any height is supported
        OUTPUT_WIDTH   : positive;
        FIFO_DEPTH     : positive;
        DEVICE_FAMILY  : string;
        DEBAYER_ENABLE : boolean;
        PACKER_ENABLE  : boolean
    );
    port(
        clk         : in  std_logic;
        reset       : in  std_logic;

        -- cmos sensor
        frame_valid : in  std_logic;
        line_valid  : in  std_logic;
        data_in     : in  std_logic_vector(PIX_DEPTH - 1 downto 0);

        -- Avalon-ST Src
        ready       : in  std_logic;
        valid       : out std_logic;
        data_out    : out std_logic_vector(OUTPUT_WIDTH - 1 downto 0);

        -- Avalon-MM Slave
        addr        : in  std_logic_vector(1 downto 0);
        read        : in  std_logic;
        write       : in  std_logic;
        rddata      : out std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);
        wrdata      : in  std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);

        -- Avalon Interrupt Sender
        irq         : out std_logic
    );
end entity cmos_sensor_input;

architecture rtl of cmos_sensor_input is
    constant PIX_DEPTH_RGB : positive := 3 * PIX_DEPTH;

    constant FIFO_DATA_WIDTH            : positive := OUTPUT_WIDTH + 1;
    constant FIFO_END_OF_FRAME_BIT_OFST : positive := OUTPUT_WIDTH; -- sc_fifo_data(FIFO_END_OF_FRAME_BIT_OFST) = end_of_frame

    -- avalon_mm_slave ---------------------------------------------------------
    signal avalon_mm_slave_clk_in              : std_logic;
    signal avalon_mm_slave_reset_in            : std_logic;
    signal avalon_mm_slave_addr_in             : std_logic_vector(1 downto 0);
    signal avalon_mm_slave_read_in             : std_logic;
    signal avalon_mm_slave_write_in            : std_logic;
    signal avalon_mm_slave_rddata_out          : std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);
    signal avalon_mm_slave_wrdata_in           : std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);
    signal avalon_mm_slave_irq_out             : std_logic;
    signal avalon_mm_slave_idle_in             : std_logic;
    signal avalon_mm_slave_snapshot_out        : std_logic;
    signal avalon_mm_slave_get_frame_info_out  : std_logic;
    signal avalon_mm_slave_irq_en_out          : std_logic;
    signal avalon_mm_slave_irq_ack_out         : std_logic;
    signal avalon_mm_slave_wait_irq_ack_in     : std_logic;
    signal avalon_mm_slave_frame_width_in      : std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);
    signal avalon_mm_slave_frame_height_in     : std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);
    signal avalon_mm_slave_debayer_pattern_out : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0);
    signal avalon_mm_slave_fifo_usedw_in       : std_logic_vector(bit_width(FIFO_DEPTH) - 1 downto 0);
    signal avalon_mm_slave_fifo_overflow_in    : std_logic;
    signal avalon_mm_slave_stop_and_reset_out  : std_logic;

    -- synchronizer ------------------------------------------------------------
    signal synchronizer_clk_in              : std_logic;
    signal synchronizer_reset_in            : std_logic;
    signal synchronizer_frame_valid_in_in   : std_logic;
    signal synchronizer_line_valid_in_in    : std_logic;
    signal synchronizer_data_in_in          : std_logic_vector(PIX_DEPTH - 1 downto 0);
    signal synchronizer_frame_valid_out_out : std_logic;
    signal synchronizer_line_valid_out_out  : std_logic;
    signal synchronizer_data_out_out        : std_logic_vector(PIX_DEPTH - 1 downto 0);

    -- sampler -----------------------------------------------------------------
    signal sampler_clk_in                  : std_logic;
    signal sampler_reset_in                : std_logic;
    signal sampler_stop_and_reset_in       : std_logic;
    signal sampler_idle_out                : std_logic;
    signal sampler_wait_irq_ack_out        : std_logic;
    signal sampler_irq_en_in               : std_logic;
    signal sampler_irq_ack_in              : std_logic;
    signal sampler_snapshot_in             : std_logic;
    signal sampler_get_frame_info_in       : std_logic;
    signal sampler_frame_width_out         : std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);
    signal sampler_frame_height_out        : std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);
    signal sampler_frame_valid_in          : std_logic;
    signal sampler_line_valid_in           : std_logic;
    signal sampler_data_in_in              : std_logic_vector(PIX_DEPTH - 1 downto 0);
    signal sampler_valid_out_out           : std_logic;
    signal sampler_data_out_out            : std_logic_vector(PIX_DEPTH - 1 downto 0);
    signal sampler_start_of_frame_out_out  : std_logic;
    signal sampler_end_of_frame_out_out    : std_logic;
    signal sampler_fifo_overflow_in        : std_logic;
    signal sampler_end_of_frame_in_in      : std_logic;
    signal sampler_end_of_frame_in_ack_out : std_logic;

    -- debayer -----------------------------------------------------------------
    signal debayer_clk_in                 : std_logic;
    signal debayer_reset_in               : std_logic;
    signal debayer_stop_and_reset_in      : std_logic;
    signal debayer_debayer_pattern_in     : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0);
    signal debayer_valid_in_in            : std_logic;
    signal debayer_data_in_in             : std_logic_vector(PIX_DEPTH - 1 downto 0);
    signal debayer_start_of_frame_in_in   : std_logic;
    signal debayer_end_of_frame_in_in     : std_logic;
    signal debayer_valid_out_out          : std_logic;
    signal debayer_data_out_out           : std_logic_vector(PIX_DEPTH_RGB - 1 downto 0);
    signal debayer_start_of_frame_out_out : std_logic;
    signal debayer_end_of_frame_out_out   : std_logic;

    -- packer_raw --------------------------------------------------------------
    signal packer_raw_clk_in               : std_logic;
    signal packer_raw_reset_in             : std_logic;
    signal packer_raw_stop_and_reset_in    : std_logic;
    signal packer_raw_valid_in_in          : std_logic;
    signal packer_raw_data_in_in           : std_logic_vector(PIX_DEPTH - 1 downto 0);
    signal packer_raw_start_of_frame_in_in : std_logic;
    signal packer_raw_end_of_frame_in_in   : std_logic;
    signal packer_raw_valid_out_out        : std_logic;
    signal packer_raw_data_out_out         : std_logic_vector(OUTPUT_WIDTH - 1 downto 0);
    signal packer_raw_end_of_frame_out_out : std_logic;

    -- packer_rgb --------------------------------------------------------------
    signal packer_rgb_clk_in               : std_logic;
    signal packer_rgb_reset_in             : std_logic;
    signal packer_rgb_stop_and_reset_in    : std_logic;
    signal packer_rgb_valid_in_in          : std_logic;
    signal packer_rgb_data_in_in           : std_logic_vector(PIX_DEPTH_RGB - 1 downto 0);
    signal packer_rgb_start_of_frame_in_in : std_logic;
    signal packer_rgb_end_of_frame_in_in   : std_logic;
    signal packer_rgb_valid_out_out        : std_logic;
    signal packer_rgb_data_out_out         : std_logic_vector(OUTPUT_WIDTH - 1 downto 0);
    signal packer_rgb_end_of_frame_out_out : std_logic;

    -- sc_fifo -----------------------------------------------------------------
    signal sc_fifo_clk_in       : std_logic;
    signal sc_fifo_reset_in     : std_logic;
    signal sc_fifo_clr_in       : std_logic;
    signal sc_fifo_data_in_in   : std_logic_vector(FIFO_DATA_WIDTH - 1 downto 0);
    signal sc_fifo_data_out_out : std_logic_vector(FIFO_DATA_WIDTH - 1 downto 0);
    signal sc_fifo_read_in      : std_logic;
    signal sc_fifo_write_in     : std_logic;
    signal sc_fifo_empty_out    : std_logic;
    signal sc_fifo_full_out     : std_logic;
    signal sc_fifo_usedw_out    : std_logic_vector(bit_width(FIFO_DEPTH) - 1 downto 0);
    signal sc_fifo_overflow_out : std_logic;

    -- avalon_st_source --------------------------------------------------------
    signal avalon_st_source_clk_in                  : std_logic;
    signal avalon_st_source_reset_in                : std_logic;
    signal avalon_st_source_stop_and_reset_in       : std_logic;
    signal avalon_st_source_ready_in                : std_logic;
    signal avalon_st_source_valid_out               : std_logic;
    signal avalon_st_source_data_out                : std_logic_vector(OUTPUT_WIDTH - 1 downto 0);
    signal avalon_st_source_fifo_read_out           : std_logic;
    signal avalon_st_source_fifo_empty_in           : std_logic;
    signal avalon_st_source_fifo_data_in            : std_logic_vector(OUTPUT_WIDTH - 1 downto 0);
    signal avalon_st_source_fifo_end_of_frame_in    : std_logic;
    signal avalon_st_source_fifo_overflow_in        : std_logic;
    signal avalon_st_source_end_of_frame_out_out    : std_logic;
    signal avalon_st_source_end_of_frame_out_ack_in : std_logic;

begin
    valid    <= avalon_st_source_valid_out;
    data_out <= avalon_st_source_data_out;
    rddata   <= avalon_mm_slave_rddata_out;
    irq      <= avalon_mm_slave_irq_out;

    cmos_sensor_input_avalon_mm_slave_inst : entity work.cmos_sensor_input_avalon_mm_slave
        generic map(DEBAYER_ENABLE => DEBAYER_ENABLE,
                    FIFO_DEPTH     => FIFO_DEPTH,
                    MAX_WIDTH      => MAX_WIDTH,
                    MAX_HEIGHT     => MAX_HEIGHT)
        port map(clk             => avalon_mm_slave_clk_in,
                 reset           => avalon_mm_slave_reset_in,
                 addr            => avalon_mm_slave_addr_in,
                 read            => avalon_mm_slave_read_in,
                 write           => avalon_mm_slave_write_in,
                 rddata          => avalon_mm_slave_rddata_out,
                 wrdata          => avalon_mm_slave_wrdata_in,
                 irq             => avalon_mm_slave_irq_out,
                 idle            => avalon_mm_slave_idle_in,
                 snapshot        => avalon_mm_slave_snapshot_out,
                 get_frame_info  => avalon_mm_slave_get_frame_info_out,
                 irq_en          => avalon_mm_slave_irq_en_out,
                 irq_ack         => avalon_mm_slave_irq_ack_out,
                 wait_irq_ack    => avalon_mm_slave_wait_irq_ack_in,
                 frame_width     => avalon_mm_slave_frame_width_in,
                 frame_height    => avalon_mm_slave_frame_height_in,
                 debayer_pattern => avalon_mm_slave_debayer_pattern_out,
                 fifo_usedw      => avalon_mm_slave_fifo_usedw_in,
                 fifo_overflow   => avalon_mm_slave_fifo_overflow_in,
                 stop_and_reset  => avalon_mm_slave_stop_and_reset_out);

    cmos_sensor_input_synchronizer_inst : entity work.cmos_sensor_input_synchronizer
        generic map(PIX_DEPTH   => PIX_DEPTH,
                    SAMPLE_EDGE => SAMPLE_EDGE)
        port map(clk             => synchronizer_clk_in,
                 reset           => synchronizer_reset_in,
                 frame_valid_in  => synchronizer_frame_valid_in_in,
                 line_valid_in   => synchronizer_line_valid_in_in,
                 data_in         => synchronizer_data_in_in,
                 frame_valid_out => synchronizer_frame_valid_out_out,
                 line_valid_out  => synchronizer_line_valid_out_out,
                 data_out        => synchronizer_data_out_out);

    cmos_sensor_input_sampler_inst : entity work.cmos_sensor_input_sampler
        generic map(PIX_DEPTH  => PIX_DEPTH,
                    MAX_WIDTH  => MAX_WIDTH,
                    MAX_HEIGHT => MAX_HEIGHT)
        port map(clk                 => sampler_clk_in,
                 reset               => sampler_reset_in,
                 stop_and_reset      => sampler_stop_and_reset_in,
                 idle                => sampler_idle_out,
                 wait_irq_ack        => sampler_wait_irq_ack_out,
                 irq_en              => sampler_irq_en_in,
                 irq_ack             => sampler_irq_ack_in,
                 snapshot            => sampler_snapshot_in,
                 get_frame_info      => sampler_get_frame_info_in,
                 frame_width         => sampler_frame_width_out,
                 frame_height        => sampler_frame_height_out,
                 frame_valid         => sampler_frame_valid_in,
                 line_valid          => sampler_line_valid_in,
                 data_in             => sampler_data_in_in,
                 valid_out           => sampler_valid_out_out,
                 data_out            => sampler_data_out_out,
                 start_of_frame_out  => sampler_start_of_frame_out_out,
                 end_of_frame_out    => sampler_end_of_frame_out_out,
                 fifo_overflow       => sampler_fifo_overflow_in,
                 end_of_frame_in     => sampler_end_of_frame_in_in,
                 end_of_frame_in_ack => sampler_end_of_frame_in_ack_out);

    debayer_inst : if DEBAYER_ENABLE generate
        cmos_sensor_input_debayer_inst : entity work.cmos_sensor_input_debayer
            generic map(PIX_DEPTH_RAW => PIX_DEPTH,
                        PIX_DEPTH_RGB => PIX_DEPTH_RGB,
                        MAX_WIDTH     => MAX_WIDTH)
            port map(clk                => debayer_clk_in,
                     reset              => debayer_reset_in,
                     stop_and_reset     => debayer_stop_and_reset_in,
                     debayer_pattern    => debayer_debayer_pattern_in,
                     valid_in           => debayer_valid_in_in,
                     data_in            => debayer_data_in_in,
                     start_of_frame_in  => debayer_start_of_frame_in_in,
                     end_of_frame_in    => debayer_end_of_frame_in_in,
                     valid_out          => debayer_valid_out_out,
                     data_out           => debayer_data_out_out,
                     start_of_frame_out => debayer_start_of_frame_out_out,
                     end_of_frame_out   => debayer_end_of_frame_out_out);
    end generate debayer_inst;

    packer_inst : if PACKER_ENABLE generate
        packer_raw : if not DEBAYER_ENABLE generate
            cmos_sensor_input_packer_inst : entity work.cmos_sensor_input_packer
                generic map(PIX_DEPTH  => PIX_DEPTH,
                            PACK_WIDTH => OUTPUT_WIDTH)
                port map(clk               => packer_raw_clk_in,
                         reset             => packer_raw_reset_in,
                         stop_and_reset    => packer_raw_stop_and_reset_in,
                         valid_in          => packer_raw_valid_in_in,
                         data_in           => packer_raw_data_in_in,
                         start_of_frame_in => packer_raw_start_of_frame_in_in,
                         end_of_frame_in   => packer_raw_end_of_frame_in_in,
                         valid_out         => packer_raw_valid_out_out,
                         data_out          => packer_raw_data_out_out,
                         end_of_frame_out  => packer_raw_end_of_frame_out_out);
        end generate packer_raw;

        packer_rgb : if DEBAYER_ENABLE generate
            cmos_sensor_input_packer_inst : entity work.cmos_sensor_input_packer
                generic map(PIX_DEPTH  => PIX_DEPTH_RGB,
                            PACK_WIDTH => OUTPUT_WIDTH)
                port map(clk               => packer_rgb_clk_in,
                         reset             => packer_rgb_reset_in,
                         stop_and_reset    => packer_rgb_stop_and_reset_in,
                         valid_in          => packer_rgb_valid_in_in,
                         data_in           => packer_rgb_data_in_in,
                         start_of_frame_in => packer_rgb_start_of_frame_in_in,
                         end_of_frame_in   => packer_rgb_end_of_frame_in_in,
                         valid_out         => packer_rgb_valid_out_out,
                         data_out          => packer_rgb_data_out_out,
                         end_of_frame_out  => packer_rgb_end_of_frame_out_out);
        end generate packer_rgb;
    end generate packer_inst;

    cmos_sensor_input_sc_fifo_inst : entity work.cmos_sensor_input_sc_fifo
        generic map(DATA_WIDTH    => FIFO_DATA_WIDTH,
                    FIFO_DEPTH    => FIFO_DEPTH,
                    DEVICE_FAMILY => DEVICE_FAMILY)
        port map(clk      => sc_fifo_clk_in,
                 reset    => sc_fifo_reset_in,
                 clr      => sc_fifo_clr_in,
                 data_in  => sc_fifo_data_in_in,
                 data_out => sc_fifo_data_out_out,
                 read     => sc_fifo_read_in,
                 write    => sc_fifo_write_in,
                 empty    => sc_fifo_empty_out,
                 full     => sc_fifo_full_out,
                 usedw    => sc_fifo_usedw_out,
                 overflow => sc_fifo_overflow_out);

    cmos_sensor_input_avalon_st_source_inst : entity work.cmos_sensor_input_avalon_st_source
        generic map(DATA_WIDTH => OUTPUT_WIDTH)
        port map(clk                  => avalon_st_source_clk_in,
                 reset                => avalon_st_source_reset_in,
                 stop_and_reset       => avalon_st_source_stop_and_reset_in,
                 ready                => avalon_st_source_ready_in,
                 valid                => avalon_st_source_valid_out,
                 data                 => avalon_st_source_data_out,
                 fifo_read            => avalon_st_source_fifo_read_out,
                 fifo_empty           => avalon_st_source_fifo_empty_in,
                 fifo_data            => avalon_st_source_fifo_data_in,
                 fifo_end_of_frame    => avalon_st_source_fifo_end_of_frame_in,
                 fifo_overflow        => avalon_st_source_fifo_overflow_in,
                 end_of_frame_out     => avalon_st_source_end_of_frame_out_out,
                 end_of_frame_out_ack => avalon_st_source_end_of_frame_out_ack_in);

    TOP_LEVEL_INTERNALS_CONNECTIONS : process(addr, avalon_mm_slave_debayer_pattern_out, avalon_mm_slave_get_frame_info_out, avalon_mm_slave_irq_ack_out, avalon_mm_slave_irq_en_out, avalon_mm_slave_snapshot_out, avalon_mm_slave_stop_and_reset_out, avalon_st_source_end_of_frame_out_out, avalon_st_source_fifo_read_out, clk, data_in, debayer_data_out_out, debayer_end_of_frame_out_out, debayer_start_of_frame_out_out, debayer_valid_out_out, frame_valid, line_valid, packer_raw_data_out_out, packer_raw_end_of_frame_out_out, packer_raw_valid_out_out, packer_rgb_data_out_out, packer_rgb_end_of_frame_out_out, packer_rgb_valid_out_out, read, ready, reset, sampler_data_out_out, sampler_end_of_frame_in_ack_out, sampler_end_of_frame_out_out, sampler_frame_height_out, sampler_frame_width_out, sampler_idle_out, sampler_start_of_frame_out_out, sampler_valid_out_out, sampler_wait_irq_ack_out, sc_fifo_data_out_out, sc_fifo_empty_out, sc_fifo_overflow_out, sc_fifo_usedw_out, synchronizer_data_out_out, synchronizer_frame_valid_out_out, synchronizer_line_valid_out_out, wrdata, write)
    begin
        -- always existing top-level connections -------------------------------
        avalon_mm_slave_clk_in           <= clk;
        avalon_mm_slave_reset_in         <= reset;
        avalon_mm_slave_addr_in          <= addr;
        avalon_mm_slave_read_in          <= read;
        avalon_mm_slave_write_in         <= write;
        avalon_mm_slave_wrdata_in        <= wrdata;
        avalon_mm_slave_idle_in          <= sampler_idle_out;
        avalon_mm_slave_wait_irq_ack_in  <= sampler_wait_irq_ack_out;
        avalon_mm_slave_frame_width_in   <= sampler_frame_width_out;
        avalon_mm_slave_frame_height_in  <= sampler_frame_height_out;
        avalon_mm_slave_fifo_usedw_in    <= sc_fifo_usedw_out;
        avalon_mm_slave_fifo_overflow_in <= sc_fifo_overflow_out;

        synchronizer_clk_in            <= clk;
        synchronizer_reset_in          <= reset;
        synchronizer_frame_valid_in_in <= frame_valid;
        synchronizer_line_valid_in_in  <= line_valid;
        synchronizer_data_in_in        <= data_in;

        sampler_clk_in             <= clk;
        sampler_reset_in           <= reset;
        sampler_stop_and_reset_in  <= avalon_mm_slave_stop_and_reset_out;
        sampler_irq_en_in          <= avalon_mm_slave_irq_en_out;
        sampler_irq_ack_in         <= avalon_mm_slave_irq_ack_out;
        sampler_snapshot_in        <= avalon_mm_slave_snapshot_out;
        sampler_get_frame_info_in  <= avalon_mm_slave_get_frame_info_out;
        sampler_frame_valid_in     <= synchronizer_frame_valid_out_out;
        sampler_line_valid_in      <= synchronizer_line_valid_out_out;
        sampler_data_in_in         <= synchronizer_data_out_out;
        sampler_fifo_overflow_in   <= sc_fifo_overflow_out;
        sampler_end_of_frame_in_in <= avalon_st_source_end_of_frame_out_out;

        debayer_clk_in             <= clk;
        debayer_reset_in           <= reset;
        debayer_stop_and_reset_in  <= avalon_mm_slave_stop_and_reset_out;
        debayer_debayer_pattern_in <= avalon_mm_slave_debayer_pattern_out;

        packer_raw_clk_in            <= clk;
        packer_raw_reset_in          <= reset;
        packer_raw_stop_and_reset_in <= avalon_mm_slave_stop_and_reset_out;

        packer_rgb_clk_in            <= clk;
        packer_rgb_reset_in          <= reset;
        packer_rgb_stop_and_reset_in <= avalon_mm_slave_stop_and_reset_out;

        sc_fifo_clk_in   <= clk;
        sc_fifo_reset_in <= reset;
        sc_fifo_clr_in   <= avalon_mm_slave_stop_and_reset_out;
        sc_fifo_read_in  <= avalon_st_source_fifo_read_out;

        avalon_st_source_clk_in                  <= clk;
        avalon_st_source_reset_in                <= reset;
        avalon_st_source_stop_and_reset_in       <= avalon_mm_slave_stop_and_reset_out;
        avalon_st_source_ready_in                <= ready;
        avalon_st_source_fifo_empty_in           <= sc_fifo_empty_out;
        avalon_st_source_fifo_data_in            <= sc_fifo_data_out_out(avalon_st_source_fifo_data_in'range);
        avalon_st_source_fifo_end_of_frame_in    <= sc_fifo_data_out_out(FIFO_END_OF_FRAME_BIT_OFST);
        avalon_st_source_fifo_overflow_in        <= sc_fifo_overflow_out;
        avalon_st_source_end_of_frame_out_ack_in <= sampler_end_of_frame_in_ack_out;

        -- default values for "configurable" signals ---------------------------
        debayer_valid_in_in          <= '0';
        debayer_data_in_in           <= (others => '0');
        debayer_start_of_frame_in_in <= '0';
        debayer_end_of_frame_in_in   <= '0';

        packer_raw_valid_in_in          <= '0';
        packer_raw_data_in_in           <= (others => '0');
        packer_raw_start_of_frame_in_in <= '0';
        packer_raw_end_of_frame_in_in   <= '0';

        packer_rgb_valid_in_in          <= '0';
        packer_rgb_data_in_in           <= (others => '0');
        packer_rgb_start_of_frame_in_in <= '0';
        packer_rgb_end_of_frame_in_in   <= '0';

        sc_fifo_write_in   <= '0';
        sc_fifo_data_in_in <= (others => '0');

        if not DEBAYER_ENABLE and not PACKER_ENABLE then
            sc_fifo_write_in                               <= sampler_valid_out_out;
            sc_fifo_data_in_in                             <= std_logic_vector(resize(unsigned(sampler_data_out_out), FIFO_DATA_WIDTH));
            sc_fifo_data_in_in(FIFO_END_OF_FRAME_BIT_OFST) <= sampler_end_of_frame_out_out;

        elsif not DEBAYER_ENABLE and PACKER_ENABLE then
            packer_raw_valid_in_in          <= sampler_valid_out_out;
            packer_raw_data_in_in           <= sampler_data_out_out;
            packer_raw_start_of_frame_in_in <= sampler_start_of_frame_out_out;
            packer_raw_end_of_frame_in_in   <= sampler_end_of_frame_out_out;

            sc_fifo_write_in                               <= packer_raw_valid_out_out;
            sc_fifo_data_in_in                             <= std_logic_vector(resize(unsigned(packer_raw_data_out_out), FIFO_DATA_WIDTH));
            sc_fifo_data_in_in(FIFO_END_OF_FRAME_BIT_OFST) <= packer_raw_end_of_frame_out_out;

        elsif DEBAYER_ENABLE and not PACKER_ENABLE then
            debayer_valid_in_in          <= sampler_valid_out_out;
            debayer_data_in_in           <= sampler_data_out_out;
            debayer_start_of_frame_in_in <= sampler_start_of_frame_out_out;
            debayer_end_of_frame_in_in   <= sampler_end_of_frame_out_out;

            sc_fifo_write_in                               <= debayer_valid_out_out;
            sc_fifo_data_in_in                             <= std_logic_vector(resize(unsigned(debayer_data_out_out), FIFO_DATA_WIDTH));
            sc_fifo_data_in_in(FIFO_END_OF_FRAME_BIT_OFST) <= debayer_end_of_frame_out_out;

        elsif DEBAYER_ENABLE and PACKER_ENABLE then
            debayer_valid_in_in          <= sampler_valid_out_out;
            debayer_data_in_in           <= sampler_data_out_out;
            debayer_start_of_frame_in_in <= sampler_start_of_frame_out_out;
            debayer_end_of_frame_in_in   <= sampler_end_of_frame_out_out;

            packer_rgb_valid_in_in          <= debayer_valid_out_out;
            packer_rgb_data_in_in           <= debayer_data_out_out;
            packer_rgb_start_of_frame_in_in <= debayer_start_of_frame_out_out;
            packer_rgb_end_of_frame_in_in   <= debayer_end_of_frame_out_out;

            sc_fifo_write_in                               <= packer_rgb_valid_out_out;
            sc_fifo_data_in_in                             <= std_logic_vector(resize(unsigned(packer_rgb_data_out_out), FIFO_DATA_WIDTH));
            sc_fifo_data_in_in(FIFO_END_OF_FRAME_BIT_OFST) <= packer_rgb_end_of_frame_out_out;

        end if;
    end process;

end architecture rtl;
