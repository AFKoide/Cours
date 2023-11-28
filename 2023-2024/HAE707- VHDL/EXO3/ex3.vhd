library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ex3 is port (
    E : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    h, reset : IN STD_LOGIC;
    S : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
    retenue : OUT STD_LOGIC);
end entity;

architecture Behavioral of ex3 is
signal registre_E : STD_LOGIC_VECTOR(3 DOWNTO 0);
signal registre_S : STD_LOGIC_VECTOR(4 DOWNTO 0) := "00000";
signal flag : STD_LOGIC := '0';
signal state : STD_LOGIC_VECTOR(2 DOWNTO 0) := "00";

begin
    reg : process(h, E, reset) BEGIN
        IF rising_edge(h) THEN
            IF flag = '0' THEN
                registre_E <= E;
            ELSIF flag = '1' THEN
                CASE state is
                    WHEN "000" =>
                        registre_S(1 downto 0) <= ('0' & registre_E(1)) + ('0' & registre_E(0));
                        state <= "001";
                    WHEN "001" =>
                        registre_S(2 downto 1) <= (('0' & registre_E(2)) + ('0' & registre_E(1))) + ('0' & registre_S(1));
                        state <= "010";
                    WHEN "010" =>
                        registre_S(3 downto 2) <= (('0' & registre_E(3)) + ('0' & registre_E(2))) + ('0' & registre_S(2));
                        state <= "011";
                    WHEN "011" =>
                        registre_S(4 downto 3) <= (('0' & registre_E(3)) + ('0' & registre_E(2))) + ('0' & registre_S(3));
                        state <= "100";
                    WHEN "100" =>
                        S <= registre_S(3 DOWNTO 0);
                        retenue <= registre_S(4);
                        registre_S <= "00000";
                        state <= "000";
                        flag <= '0';
                END CASE;
            END IF;
        END IF;
    END PROCESS;
end Behavioral;
