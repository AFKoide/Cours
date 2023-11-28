library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ex4_tb is
end ex4_tb;

architecture Behavioral of ex4_tb is
component ex4 is port(
    E : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    h, reset : IN STD_LOGIC;
    S : OUT STD_LOGIC);
end component;

signal E : STD_LOGIC_VECTOR(3 DOWNTO 0);
signal S, h, reset : STD_LOGIC;


begin
    dut : ex4 port map(E,h,reset,S);
    clk : process
    begin
        h <= '1';
        wait for 30ns;
        h <= '0';
        wait for 30ns;
    end process;

    Comportement : process
    begin
        reset <= '0';
        E <= "0000";
        wait for 120ns;
        E <= "0110";
        wait for 120ns;
        E <= "1000";
        wait for 120ns;

        E <= "0110";
        wait for 120ns;
        E <= "1001";
        wait for 120ns;
        E <= "1100";
        wait for 120ns;
        E <= "0011";
        wait for 120ns;
        E <= "0000";
        wait;
    end process;
end Behavioral;