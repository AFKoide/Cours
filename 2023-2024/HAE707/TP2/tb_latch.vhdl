-- Testbench automatically generated online
-- at https://vhdl.lapinoo.net
-- Generation date : 26.9.2023 13:47:52 UTC

library ieee;
use ieee.std_logic_1164.all;

entity tb_latch is
end tb_latch;

architecture tb of tb_latch is

    component latch
        port (d     : in std_logic;
              clk   : in std_logic;
              reset : in std_logic;
              q     : out std_logic);
    end component;

    signal d     : std_logic;
    signal clk   : std_logic;
    signal reset : std_logic;
    signal q     : std_logic;

    constant TbPeriod : time := 1000 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin

    dut : latch
    port map (d     => d,
              clk   => clk,
              reset => reset,
              q     => q);

    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that clk is really your main clock signal
    clk <= TbClock;

    stimuli : process
    begin
        -- EDIT Adapt initialization as needed
        d <= '0';

        -- Reset generation
        -- EDIT: Check that reset is really your reset signal
        reset <= '1';
        wait for 100 ns;
        reset <= '0';
        wait for 100 ns;

        -- EDIT Add stimuli here
        wait for 100 * TbPeriod;

        -- Stop the clock and hence terminate the simulation
        TbSimEnded <= '1';
        wait;
    end process;

end tb;

-- Configuration block below is required by some simulators. Usually no need to edit.

configuration cfg_tb_latch of tb_latch is
    for tb
    end for;
end cfg_tb_latch;
