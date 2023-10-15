library ieee;
use ieee.std_logic_1164.all;

entity tb_feu is
end tb_feu;

architecture tb of tb_feu is
    component feu
        port (
            ReqH, ReqV             : IN STD_LOGIC;
            h, reset               : IN STD_LOGIC;
            RH, YH, VH, RV, YV, VV : OUT STD_LOGIC
        );
    end component;

    SIGNAL ReqH, ReqV             : STD_LOGIC;
    SIGNAL h, reset               : STD_LOGIC;
    SIGNAL RH, YH, VH, RV, YV, VV : STD_LOGIC;

    constant TbPeriod : time := 20 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '1';
    signal TbSimEnded : std_logic := '0';

begin
    dut : feu port map (ReqH, ReqV, h, reset, RH, YH, VH,RV, YV, VV );
    reset <= '0';
    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that clk is really your main clock signal
    -- h <= TbClock;
    horloge : process begin
        h <= '1';
        wait for 10 ns;
        h <= '0';
        wait for 10 ns;
        -- wait for TbPeriod * 40;
        -- TbSimEnded <= '1';
        -- wait;
    end process;
    
    Entree : process begin
        ReqH <= '0';
        ReqV <= '0';
        wait for 20 * TbPeriod;
        ReqV <= '1';
        wait for TbPeriod;
        ReqV <= '0';
        wait for TbPeriod;
        ReqV <= '1';
        wait for TbPeriod;
        ReqV <= '0';
        wait for TbPeriod;
        ReqV <= '1';
        wait for TbPeriod;
        WAIT;
    end process;
end tb;
