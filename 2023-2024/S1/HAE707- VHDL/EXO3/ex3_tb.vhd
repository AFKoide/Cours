library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ex3_tb is
end ex3_tb;

architecture Behavioral of ex3_tb is
component ex3 is port(
    E : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    h, reset : IN STD_LOGIC;
    S : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    retenue : OUT STD_LOGIC);
end component;

signal E, S : STD_LOGIC_VECTOR(3 DOWNTO 0);
signal h, reset, retenue : STD_LOGIC;

begin
    dut : ex3 port map (E,h,reset,S,retenue);
    clk : process
    begin
        h <= '0';
        wait for 20ns;
        h <= '1';
        wait for 20ns;
    end process;

    Comportement : process
    begin
        E <= "0101";
        wait;
    end process;
end Behavioral;