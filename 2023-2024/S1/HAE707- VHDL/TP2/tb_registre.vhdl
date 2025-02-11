-- Testbench automatically generated online
-- at https://vhdl.lapinoo.net
-- Generation date : 5.10.2023 16:41:57 UTC

library ieee;
use ieee.std_logic_1164.all;

entity tb_compteur is
end tb_compteur;

architecture tb of tb_compteur is
    component compteur
        port (clk    : in std_logic;
              reset  : in std_logic;
              config : in std_logic;
              s      : out std_logic_vector (7 downto 0));
    end component;

    signal clk    : std_logic;
    signal reset  : std_logic;
    signal config : std_logic;
    signal s      : std_logic_vector (7 downto 0);

    constant TbPeriod : time := 10 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin
    dut : compteur
    port map (clk, reset, config, s);

    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';
    -- EDIT: Check that clk is really your main clock signal
    clk <= TbClock;

    Horloge : process
    begin
        wait for 100 * TbPeriod;
        TbSimEnded <= '1';
        wait;
    end process;
    Entree : process
    begin
        reset <= '1';
        wait for 1 ns;
        reset <= '0';
        config <= '0';
        wait for 20 * TbPeriod;
        reset <= '1';
        wait for TbPeriod;
        config <= '1';
    end process;
end tb;
