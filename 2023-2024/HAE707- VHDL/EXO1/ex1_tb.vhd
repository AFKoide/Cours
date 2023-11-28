library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ex1_tb is
--  Port ( );
end ex1_tb;

architecture Behavioral of ex1_tb is
component ex1 is port(
    j, k, h : IN STD_LOGIC;
    q, qb : OUT STD_LOGIC);
end component;

signal j, k, h, q, qb : STD_LOGIC;

begin
    dut : ex1 port map (j, k, h, q, qb);

    Comportement : process
    begin
        h <= '1';
        j <= '0';
        k <= '0';
        wait for 50ns;
        j <= '1';
        k <= '0';
        wait for 50ns;
        j <= '0';
        k <= '0';
        wait for 50ns;
        j <= '0';
        k <= '1';
        wait for 50ns;
        j <= '0';
        k <= '0';
        wait for 50ns;
        j <= '1';
        k <= '1';
        wait for 50ns;
        j <= '0';
        k <= '0';
        wait;
        end process;
end Behavioral;