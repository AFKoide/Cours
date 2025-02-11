library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ex1 is port(
    j, k, h : IN STD_LOGIC;
    q, qb : OUT STD_LOGIC);
end ex1;

architecture Behavioral of ex1 is
signal q_temp : STD_LOGIC := '0';
begin
    bascule : process(j,k,h) begin
        IF h = '1' THEN             -- On choisi h=1 comme état.
            IF j='0' and k='0' THEN
                q_temp <= q_temp;
            ELSIF j='0' and k='1' THEN
                q_temp <= '0';
            ELSIF j='1' and k='0' THEN
                q_temp <= '1';
            ELSIF j='1' and k='1' THEN
                q_temp <= not(q_temp);
            END IF;
        END IF;
    END PROCESS;
    q <= q_temp;
    qb <= not(q_temp);
end Behavioral;

