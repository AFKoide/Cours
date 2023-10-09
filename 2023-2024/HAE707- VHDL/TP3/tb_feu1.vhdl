library ieee;
use ieee.std_logic_1164.all;

entity tb_flip_flop is
end tb_flip_flop;

architecture tb of tb_flip_flop is

    component flip_flop
        port (
            ReqH       : IN STD_LOGIC;
            h          : IN STD_LOGIC;
            RH, YH, VH : OUT STD_LOGIC
        );
    end component;

    SIGNAL ReqH       : STD_LOGIC;
    SIGNAL h          : STD_LOGIC;
    SIGNAL RH, YH, VH : STD_LOGIC;

    constant TbPeriod : time := 20 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin
    dut : flip_flop port map (ReqH,h,RH,YH,VH);
    
    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that clk is really your main clock signal
    h <= TbClock;
    ReqH <= '0';
    horloge : process begin
        wait for TbPeriod * 10;
        TbSimEnded <= '1';
        wait;
    end process;
    
    Entree : process begin
        wait;
    end process;
end tb;
