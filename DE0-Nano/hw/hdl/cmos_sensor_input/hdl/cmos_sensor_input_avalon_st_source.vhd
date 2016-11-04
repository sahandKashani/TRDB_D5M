library ieee;
use ieee.std_logic_1164.all;

entity cmos_sensor_input_avalon_st_source is
    generic(
        DATA_WIDTH : positive
    );
    port(
        clk                  : in  std_logic;
        reset                : in  std_logic;

        -- avalon_mm_slave
        stop_and_reset       : in  std_logic;

        -- Avalon-ST Source
        ready                : in  std_logic;
        valid                : out std_logic;
        data                 : out std_logic_vector(DATA_WIDTH - 1 downto 0);

        -- fifo
        fifo_read            : out std_logic;
        fifo_empty           : in  std_logic;
        fifo_data            : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        fifo_end_of_frame    : in  std_logic;
        fifo_overflow        : in  std_logic;

        -- sampler
        end_of_frame_out     : out std_logic;
        end_of_frame_out_ack : in  std_logic
    );
end entity cmos_sensor_input_avalon_st_source;

architecture rtl of cmos_sensor_input_avalon_st_source is
    type state_type is (STATE_IDLE, STATE_READY_CYCLE, STATE_WAIT_END_OF_FRAME_ACK);

    signal reg_state, next_reg_state : state_type;

    signal data_little_endian : std_logic_vector(data'range);
    signal data_big_endian    : std_logic_vector(data'range);

begin
    STATE_LOGIC : process(clk, reset)
    begin
        if reset = '1' then
            reg_state <= STATE_IDLE;
        elsif rising_edge(clk) then
            if stop_and_reset = '1' then
                reg_state <= STATE_IDLE;
            else
                reg_state <= next_reg_state;
            end if;
        end if;
    end process;

    NEXT_STATE_LOGIC : process(data_big_endian, end_of_frame_out_ack, fifo_empty, fifo_end_of_frame, fifo_overflow, ready, reg_state)
    begin
        fifo_read        <= '0';
        valid            <= '0';
        data             <= (others => '0');
        end_of_frame_out <= '0';

        next_reg_state <= reg_state;

        case reg_state is
            when STATE_IDLE =>
                if ready = '1' then
                    next_reg_state <= STATE_READY_CYCLE;
                end if;

            when STATE_READY_CYCLE =>
                if ready = '0' then
                    next_reg_state <= STATE_IDLE;
                end if;

                if fifo_empty = '0' and fifo_overflow = '0' then
                    fifo_read <= '1';
                    valid     <= '1';
                    data      <= data_big_endian;

                    if fifo_end_of_frame = '1' then
                        next_reg_state <= STATE_WAIT_END_OF_FRAME_ACK;
                    end if;
                end if;

            when STATE_WAIT_END_OF_FRAME_ACK =>
                end_of_frame_out <= '1';

                if end_of_frame_out_ack = '1' then
                    next_reg_state <= STATE_IDLE;
                end if;

        end case;
    end process;

    data_little_endian <= fifo_data;

    NETWORK_ORDER : process(data_little_endian)
    begin
        -- data is an Avalon-ST interface, so it needs to arrange data in
        -- network order (a.k.a big-endian).
        -- This component is specified to support 8 bits per symbol for its
        -- Avalon-ST interface, so data is flipped on 8-bit boundaries.
        for i in 0 to (DATA_WIDTH / 8) - 1 loop
            data_big_endian(8 * (i + 1) - 1 downto 8 * i) <= data_little_endian(data_little_endian'length - 8 * i - 1 downto data_little_endian'length - 8 * (i + 1));
        end loop;
    end process;

end architecture rtl;
