library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ex2_tb is
end ex2_tb;

architecture Behavioral of ex2_tb is
component ex2 is port(
    E : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    h, COM, reset : IN STD_LOGIC;
    S : OUT STD_LOGIC_VECTOR(3 DOWNTO 0));
end component;

signal E, S : STD_LOGIC_VECTOR(3 DOWNTO 0);
signal h, COM, reset : STD_LOGIC;

begin
    dut : ex2 port map (E, h, COM, reset, S);
    clk : process
    begin
        h <= '0';
        wait for 20ns;
        h <= '1';
        wait for 20ns;
    end process;
    
    Comportement : process
    begin
        reset <= '1';
        wait for 20 ns;
        reset <= '0';
        COM <= '0';
        E <= "1001";
        wait for 20ns;
        COM <= '1';
        E <= "1111";
        wait for 160ns;
        COM <= '0';
        wait for 40ns;
        COM <= '1';
        wait;
    end process;
end Behavioral;