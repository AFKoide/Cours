LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

-- Bascule D
ENTITY latch IS
	PORT (
		d, clk, reset : IN std_logic;
		q             : OUT std_logic
	);
END latch;

-- Programme
ARCHITECTURE bascule_d OF latch IS
SIGNAL temp_q : STD_LOGIC;
BEGIN
	copie : PROCESS (clk, reset) BEGIN
		IF reset = '1' THEN
			temp_q <= '0';
			IF clk'EVENT AND clk = '1' THEN
				temp_q <= NOT(q);
			ELSE
				temp_q <= q;
			END IF;
        END IF;
	END PROCESS;
    q <= temp_q;
END bascule_d;