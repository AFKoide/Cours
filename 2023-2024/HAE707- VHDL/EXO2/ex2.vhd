library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity ex2 is port (
    E : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    h, COM, reset : IN STD_LOGIC;
    S : OUT STD_LOGIC_VECTOR(3 DOWNTO 0));
end ex2;

architecture Behavioral of ex2 is
signal registre : STD_LOGIC_VECTOR(3 DOWNTO 0);
begin
    reg : process(h, E, reset) BEGIN
        IF reset = '1' THEN
            registre <= "0000";
        ELSIF rising_edge(h) THEN
            IF COM = '0' THEN
                registre <= E;
            ELSIF COM = '1' THEN
                registre(2 DOWNTO 0) <= registre(3 DOWNTO 1);
                registre(3) <= '0';
            END IF;
        END IF;
    END PROCESS;
    S <= registre;
end Behavioral;
