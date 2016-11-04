library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.cmos_sensor_input_constants.all;

entity cmos_sensor_input_sampler is
    generic(
        PIX_DEPTH  : positive;
        MAX_WIDTH  : positive;
        MAX_HEIGHT : positive
    );
    port(
        clk                 : in  std_logic;
        reset               : in  std_logic;

        -- avalon_mm_slave
        stop_and_reset      : in  std_logic;
        idle                : out std_logic;
        wait_irq_ack        : out std_logic;
        irq_en              : in  std_logic;
        irq_ack             : in  std_logic;
        snapshot            : in  std_logic;
        get_frame_info      : in  std_logic;
        frame_width         : out std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);
        frame_height        : out std_logic_vector(bit_width(max(MAX_WIDTH, MAX_HEIGHT)) - 1 downto 0);

        -- synchronizer
        frame_valid         : in  std_logic;
        line_valid          : in  std_logic;
        data_in             : in  std_logic_vector(PIX_DEPTH - 1 downto 0);

        -- debayer / packer / fifo
        valid_out           : out std_logic;
        data_out            : out std_logic_vector(PIX_DEPTH - 1 downto 0);
        start_of_frame_out  : out std_logic;
        end_of_frame_out    : out std_logic;

        -- fifo
        fifo_overflow       : in  std_logic;

        -- st_source
        end_of_frame_in     : in  std_logic;
        end_of_frame_in_ack : out std_logic
    );
end entity cmos_sensor_input_sampler;

architecture rtl of cmos_sensor_input_sampler is
    -- GFI = get_frame_info
    -- SNPSHT  = snapshot
    type state_type is (STATE_IDLE,
                        STATE_WAIT_END_FRAME_GFI, STATE_WAIT_START_FRAME_GFI, STATE_DATA_SKIP, STATE_LINE_LINE_BLANK_OR_LINE_FRAME_BLANK_GFI,
                        STATE_WAIT_END_FRAME_SNPSHT, STATE_WAIT_START_FRAME_SNPSHT, STATE_START_OF_FRAME_OUT, STATE_DATA_VALID, STATE_LINE_LINE_BLANK_SNPSHT, STATE_END_OF_FRAME_OUT, STATE_WAIT_END_OF_FRAME_IN, STATE_END_OF_FRAME_IN_ACK,
                        STATE_WAIT_IRQ_ACK);

    signal reg_state, next_reg_state : state_type;

    signal reg_frame_width_config, next_reg_frame_width_config     : unsigned(frame_width'range);
    signal reg_frame_height_config, next_reg_frame_height_config   : unsigned(frame_height'range);
    signal reg_frame_width_counter, next_reg_frame_width_counter   : unsigned(frame_width'range);
    signal reg_frame_height_counter, next_reg_frame_height_counter : unsigned(frame_height'range);

    signal reg_data_in, next_reg_data_in : std_logic_vector(data_in'range);

begin
    process(clk, reset)
    begin
        if reset = '1' then
            reg_state                <= STATE_IDLE;
            reg_frame_width_config   <= (others => '0');
            reg_frame_height_config  <= (others => '0');
            reg_frame_width_counter  <= (others => '0');
            reg_frame_height_counter <= (others => '0');
            reg_data_in              <= (others => '0');
        elsif rising_edge(clk) then
            if stop_and_reset = '1' then
                reg_state                <= STATE_IDLE;
                reg_frame_width_config   <= (others => '0');
                reg_frame_height_config  <= (others => '0');
                reg_frame_width_counter  <= (others => '0');
                reg_frame_height_counter <= (others => '0');
                reg_data_in              <= (others => '0');
            else
                reg_state                <= next_reg_state;
                reg_frame_width_config   <= next_reg_frame_width_config;
                reg_frame_height_config  <= next_reg_frame_height_config;
                reg_frame_width_counter  <= next_reg_frame_width_counter;
                reg_frame_height_counter <= next_reg_frame_height_counter;
                reg_data_in              <= next_reg_data_in;
            end if;
        end if;
    end process;

    process(data_in, end_of_frame_in, fifo_overflow, frame_valid, get_frame_info, irq_ack, irq_en, line_valid, reg_data_in, reg_frame_height_config, reg_frame_height_counter, reg_frame_width_config, reg_frame_width_counter, reg_state, snapshot)
    begin
        idle                <= '0';
        wait_irq_ack        <= '0';
        frame_width         <= std_logic_vector(reg_frame_width_config);
        frame_height        <= std_logic_vector(reg_frame_height_config);
        valid_out           <= '0';
        data_out            <= (others => '0');
        start_of_frame_out  <= '0';
        end_of_frame_out    <= '0';
        end_of_frame_in_ack <= '0';

        next_reg_state                <= reg_state;
        next_reg_frame_width_config   <= reg_frame_width_config;
        next_reg_frame_height_config  <= reg_frame_height_config;
        next_reg_frame_width_counter  <= reg_frame_width_counter;
        next_reg_frame_height_counter <= reg_frame_height_counter;
        next_reg_data_in              <= data_in;

        case reg_state is
            when STATE_IDLE =>
                idle <= '1';

                if get_frame_info = '1' then
                    if frame_valid = '0' then
                        next_reg_state <= STATE_WAIT_START_FRAME_GFI;
                    elsif frame_valid = '1' then
                        next_reg_state <= STATE_WAIT_END_FRAME_GFI;
                    end if;
                elsif snapshot = '1' then
                    if frame_valid = '0' then
                        next_reg_state <= STATE_WAIT_START_FRAME_SNPSHT;
                    elsif frame_valid = '1' then
                        next_reg_state <= STATE_WAIT_END_FRAME_SNPSHT;
                    end if;
                end if;

            when STATE_WAIT_END_FRAME_GFI =>
                if frame_valid = '0' then
                    next_reg_state <= STATE_WAIT_START_FRAME_GFI;
                end if;

            when STATE_WAIT_START_FRAME_GFI =>
                if frame_valid = '1' and line_valid = '1' then
                    next_reg_state               <= STATE_DATA_SKIP;
                    next_reg_frame_width_config  <= to_unsigned(1, next_reg_frame_width_config'length);
                    next_reg_frame_height_config <= to_unsigned(1, next_reg_frame_height_config'length);
                end if;

            when STATE_DATA_SKIP =>
                if frame_valid = '0' then
                    if line_valid = '0' then
                        if irq_en = '0' then
                            next_reg_state <= STATE_IDLE;
                        elsif irq_en = '1' then
                            next_reg_state <= STATE_WAIT_IRQ_ACK;
                        end if;
                    end if;
                elsif frame_valid = '1' then
                    if line_valid = '0' then
                        next_reg_state <= STATE_LINE_LINE_BLANK_OR_LINE_FRAME_BLANK_GFI;
                    elsif line_valid = '1' then
                        next_reg_frame_width_config <= reg_frame_width_config + 1;
                    end if;
                end if;

            when STATE_LINE_LINE_BLANK_OR_LINE_FRAME_BLANK_GFI =>
                if frame_valid = '0' and line_valid = '0' then
                    if irq_en = '0' then
                        next_reg_state <= STATE_IDLE;
                    elsif irq_en = '1' then
                        next_reg_state <= STATE_WAIT_IRQ_ACK;
                    end if;
                elsif frame_valid = '1' and line_valid = '1' then
                    next_reg_state               <= STATE_DATA_SKIP;
                    next_reg_frame_width_config  <= to_unsigned(1, next_reg_frame_width_config'length);
                    next_reg_frame_height_config <= reg_frame_height_config + 1;
                end if;

            when STATE_WAIT_END_FRAME_SNPSHT =>
                if frame_valid = '0' then
                    next_reg_state <= STATE_WAIT_START_FRAME_SNPSHT;
                end if;

            when STATE_WAIT_START_FRAME_SNPSHT =>
                if frame_valid = '1' and line_valid = '1' then
                    next_reg_state                <= STATE_START_OF_FRAME_OUT;
                    next_reg_frame_width_counter  <= to_unsigned(1, next_reg_frame_width_counter'length);
                    next_reg_frame_height_counter <= to_unsigned(1, next_reg_frame_height_counter'length);
                end if;

            when STATE_START_OF_FRAME_OUT =>
                if fifo_overflow = '1' then
                    if irq_en = '0' then
                        next_reg_state <= STATE_IDLE;
                    elsif irq_en = '1' then
                        next_reg_state <= STATE_WAIT_IRQ_ACK;
                    end if;
                elsif fifo_overflow = '0' then
                    valid_out          <= '1';
                    data_out           <= reg_data_in;
                    start_of_frame_out <= '1';

                    if reg_frame_width_counter < reg_frame_width_config - 1 then
                        next_reg_state               <= STATE_DATA_VALID;
                        next_reg_frame_width_counter <= reg_frame_width_counter + 1;
                    elsif reg_frame_width_counter = reg_frame_width_config - 1 then
                        if reg_frame_height_counter = reg_frame_height_config then
                            next_reg_state <= STATE_END_OF_FRAME_OUT;
                        end if;
                    end if;
                end if;

            when STATE_DATA_VALID =>
                if fifo_overflow = '1' then
                    if irq_en = '0' then
                        next_reg_state <= STATE_IDLE;
                    elsif irq_en = '1' then
                        next_reg_state <= STATE_WAIT_IRQ_ACK;
                    end if;
                elsif fifo_overflow = '0' then
                    valid_out <= '1';
                    data_out  <= reg_data_in;

                    next_reg_frame_width_counter <= reg_frame_width_counter + 1;

                    if reg_frame_height_counter < reg_frame_height_config then
                        if reg_frame_width_counter = reg_frame_width_config then
                            next_reg_state <= STATE_LINE_LINE_BLANK_SNPSHT;
                        end if;
                    elsif reg_frame_height_counter = reg_frame_height_config then
                        if reg_frame_width_counter = reg_frame_width_config - 1 then
                            next_reg_state               <= STATE_END_OF_FRAME_OUT;
                            next_reg_frame_width_counter <= reg_frame_width_counter + 1;
                        end if;
                    end if;
                end if;

            when STATE_LINE_LINE_BLANK_SNPSHT =>
                if fifo_overflow = '1' then
                    if irq_en = '0' then
                        next_reg_state <= STATE_IDLE;
                    elsif irq_en = '1' then
                        next_reg_state <= STATE_WAIT_IRQ_ACK;
                    end if;
                elsif fifo_overflow = '0' then
                    if frame_valid = '1' and line_valid = '1' then
                        next_reg_state                <= STATE_DATA_VALID;
                        next_reg_frame_width_counter  <= to_unsigned(1, next_reg_frame_width_counter'length);
                        next_reg_frame_height_counter <= reg_frame_height_counter + 1;
                    end if;
                end if;

            when STATE_END_OF_FRAME_OUT =>
                if fifo_overflow = '1' then
                    if irq_en = '0' then
                        next_reg_state <= STATE_IDLE;
                    elsif irq_en = '1' then
                        next_reg_state <= STATE_WAIT_IRQ_ACK;
                    end if;
                elsif fifo_overflow = '0' then
                    valid_out        <= '1';
                    data_out         <= reg_data_in;
                    end_of_frame_out <= '1';

                    next_reg_state <= STATE_WAIT_END_OF_FRAME_IN;
                end if;

            when STATE_WAIT_END_OF_FRAME_IN =>
                if fifo_overflow = '1' then
                    if irq_en = '0' then
                        next_reg_state <= STATE_IDLE;
                    elsif irq_en = '1' then
                        next_reg_state <= STATE_WAIT_IRQ_ACK;
                    end if;
                elsif fifo_overflow = '0' then
                    if end_of_frame_in = '1' then
                        next_reg_state <= STATE_END_OF_FRAME_IN_ACK;
                    end if;
                end if;

            when STATE_END_OF_FRAME_IN_ACK =>
                end_of_frame_in_ack <= '1';

                if irq_en = '0' then
                    next_reg_state <= STATE_IDLE;
                elsif irq_en = '1' then
                    next_reg_state <= STATE_WAIT_IRQ_ACK;
                end if;

            when STATE_WAIT_IRQ_ACK =>
                wait_irq_ack <= '1';

                if irq_ack = '1' then
                    next_reg_state <= STATE_IDLE;
                end if;

        end case;
    end process;

end architecture rtl;
