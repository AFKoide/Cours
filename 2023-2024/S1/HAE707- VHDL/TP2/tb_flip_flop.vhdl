library ieee;
use ieee.std_logic_1164.all;

entity tb_flip_flop is
end tb_flip_flop;

architecture tb of tb_flip_flop is

    component flip_flop
        port (D   : in std_logic;
              R   : in std_logic;
              clk : in std_logic;
              q   : out std_logic;
              qb  : out std_logic);
    end component;

    signal D   : std_logic;
    signal R   : std_logic;
    signal clk : std_logic;
    signal q   : std_logic;
    signal qb  : std_logic;

    constant TbPeriod : time := 20 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin
    dut : flip_flop port map (D,R,clk,q,qb);
    
    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that clk is really your main clock signal
    clk <= TbClock;

    horloge : process begin
        wait for TbPeriod*10;
        TbSimEnded <= '1';
        
        wait;
    end process;
    
    Entree : process begin
        D <= '1';
        R <= '0';
        wait for 2 * TbPeriod;
        D <= '0';
        wait for 2 * TbPeriod;
        D <= '1';
        wait for TbPeriod;
        R <= '1';
        wait for TbPeriod;
        R <= '0';
        wait for TbPeriod;
        wait;
    end process;
end tb;
