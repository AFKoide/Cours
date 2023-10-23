library ieee;
use ieee.std_logic_1164.all;

entity tb_train is
end tb_train;

architecture tb of tb_train is
    component train
        port (
            start, h : IN std_logic;
            s        : OUT std_logic
        );
    end component;

    SIGNAL start, h, s             : STD_LOGIC;

    constant TbPeriod : time := 20 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin
    dut : train port map (start, h, s);
    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that clk is really your main clock signal
    h <= TbClock;
    
    Entree : process begin
    start <= '0';
    wait for 2*TbPeriod;
    start <= '1';
    wait for TbPeriod;
    start <= '0';
    wait for 10*TbPeriod;
    start <= '1';
    wait for TbPeriod;
    end process;
end tb;
