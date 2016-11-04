library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input_avalon_mm_slave is
    generic(
        DEBAYER_ENABLE : boolean;
        FIFO_DEPTH     : positive;
        MAX_WIDTH      : positive;
        MAX_HEIGHT     : positive
    );
    port(
        clk             : in  std_logic;
        reset           : in  std_logic;

        -- Avalon-MM Slave
        addr            : in  std_logic_vector(1 downto 0);
        read            : in  std_logic;
        write           : in  std_logic;
        rddata          : out std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);
        wrdata          : in  std_logic_vector(CMOS_SENSOR_INPUT_MM_S_DATA_WIDTH - 1 downto 0);

        -- Avalon Interrupt Sender
        irq             : out std_logic;

        -- sampler
        idle            : in  std_logic;
        snapshot        : out std_logic;
        get_frame_info  : out std_logic;
        irq_en          : out std_logic;
        irq_ack         : out std_logic;
        wait_irq_ack    : in  std_logic;
        frame_width     : in  std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);
        frame_height    : in  std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);

        -- debayer
        debayer_pattern : out std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0);

        -- fifo
        fifo_usedw      : in  std_logic_vector(bit_width(FIFO_DEPTH) - 1 downto 0);
        fifo_overflow   : in  std_logic;

        -- sampler / debayer / packer / fifo / st_source
        stop_and_reset  : out std_logic
    );
end entity cmos_sensor_input_avalon_mm_slave;

architecture rtl of cmos_sensor_input_avalon_mm_slave is

    -- MM_WRITE
    signal reg_snapshot        : std_logic;
    signal reg_get_frame_info  : std_logic;
    signal reg_irq_en          : std_logic;
    signal reg_irq_ack         : std_logic;
    signal reg_debayer_pattern : std_logic_vector(debayer_pattern'range);
    signal reg_stop_and_reset  : std_logic;

begin
    -- registered outputs
    irq             <= wait_irq_ack;
    irq_en          <= reg_irq_en;
    irq_ack         <= reg_irq_ack;
    snapshot        <= reg_snapshot;
    get_frame_info  <= reg_get_frame_info;
    debayer_pattern <= reg_debayer_pattern;
    stop_and_reset  <= reg_stop_and_reset;

    MM_WRITE : process(clk, reset)
        variable wrdata_config_irq             : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_IRQ_WIDTH - 1 downto 0);
        variable wrdata_config_debayer_pattern : std_logic_vector(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_WIDTH - 1 downto 0);
        variable wrdata_command                : std_logic_vector(CMOS_SENSOR_INPUT_COMMAND_WIDTH - 1 downto 0);
    begin
        if reset = '1' then
            reg_snapshot        <= '0';
            reg_get_frame_info  <= '0';
            reg_irq_en          <= '0';
            reg_irq_ack         <= '0';
            reg_debayer_pattern <= CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_RGGB;
            reg_stop_and_reset  <= '0';
        elsif rising_edge(clk) then
            reg_snapshot       <= '0';
            reg_get_frame_info <= '0';
            reg_irq_ack        <= '0';
            reg_stop_and_reset <= '0';

            if write = '1' then
                case addr is
                    when CMOS_SENSOR_INPUT_CONFIG_OFST =>
                        -- prevent changing config when unit is running
                        if idle = '1' then
                            wrdata_config_irq             := wrdata(CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST);
                            wrdata_config_debayer_pattern := wrdata(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_LOW_BIT_OFST);

                            -- irq
                            if wrdata_config_irq = CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE then
                                reg_irq_en <= '1';
                            elsif wrdata_config_irq = CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE then
                                reg_irq_en <= '0';
                            end if;

                            -- debayer
                            reg_debayer_pattern <= (others => '0'); -- needed to avoid latch generation if DEBAYER_ENABLE = false
                            if DEBAYER_ENABLE then
                                reg_debayer_pattern <= wrdata_config_debayer_pattern;
                            end if;
                        end if;

                    when CMOS_SENSOR_INPUT_COMMAND_OFST =>
                        wrdata_command := wrdata(CMOS_SENSOR_INPUT_COMMAND_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_COMMAND_LOW_BIT_OFST);

                        if wrdata_command = CMOS_SENSOR_INPUT_COMMAND_SNAPSHOT then
                            -- only allow state change when unit is idle
                            if idle = '1' then
                                reg_snapshot <= '1';
                            end if;
                        elsif wrdata_command = CMOS_SENSOR_INPUT_COMMAND_GET_FRAME_INFO then
                            -- only allow frame info to change when unit is idle
                            if idle = '1' then
                                reg_get_frame_info <= '1';
                            end if;
                        elsif wrdata_command = CMOS_SENSOR_INPUT_COMMAND_IRQ_ACK then
                            -- will only accept an irq acknowledgement if irq is enabled
                            if reg_irq_en = '1' then
                                reg_irq_ack <= '1';
                            end if;
                        elsif wrdata_command = CMOS_SENSOR_INPUT_COMMAND_STOP_AND_RESET then
                            reg_stop_and_reset <= '1';
                        end if;

                    when others =>
                        null;
                end case;
            end if;

        end if;
    end process;

    MM_READ : process(clk, reset)
    begin
        if reset = '1' then
            rddata <= (others => '0');

        elsif rising_edge(clk) then
            rddata <= (others => '0');

            if read = '1' then
                case addr is
                    when CMOS_SENSOR_INPUT_CONFIG_OFST =>
                        if reg_irq_en = '1' then
                            rddata(CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_CONFIG_IRQ_ENABLE;
                        else
                            rddata(CMOS_SENSOR_INPUT_CONFIG_IRQ_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_IRQ_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_CONFIG_IRQ_DISABLE;
                        end if;

                        if DEBAYER_ENABLE then
                            rddata(CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_CONFIG_DEBAYER_PATTERN_LOW_BIT_OFST) <= reg_debayer_pattern;
                        end if;

                    when CMOS_SENSOR_INPUT_STATUS_OFST =>
                        if idle = '1' then
                            rddata(CMOS_SENSOR_INPUT_STATUS_STATE_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_STATUS_STATE_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_STATUS_STATE_IDLE;
                        else
                            rddata(CMOS_SENSOR_INPUT_STATUS_STATE_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_STATUS_STATE_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_STATUS_STATE_BUSY;
                        end if;

                        if fifo_overflow = '1' then
                            rddata(CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_OVERFLOW;
                        else
                            rddata(CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_LOW_BIT_OFST) <= CMOS_SENSOR_INPUT_STATUS_FIFO_OVFL_NO_OVERFLOW;
                        end if;

                        rddata(CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_HIGH_BIT_0FST downto CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_LOW_BIT_OFST) <= std_logic_vector(resize(unsigned(fifo_usedw), CMOS_SENSOR_INPUT_STATUS_FIFO_USEDW_WIDTH));

                    when CMOS_SENSOR_INPUT_FRAME_INFO_OFST =>
                        rddata(CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_LOW_BIT_OFST)   <= std_logic_vector(resize(unsigned(frame_width), CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_WIDTH_WIDTH));
                        rddata(CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_HIGH_BIT_OFST downto CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_LOW_BIT_OFST) <= std_logic_vector(resize(unsigned(frame_height), CMOS_SENSOR_INPUT_FRAME_INFO_FRAME_HEIGHT_WIDTH));

                    when others =>
                        null;
                end case;
            end if;
        end if;
    end process;

end architecture rtl;
