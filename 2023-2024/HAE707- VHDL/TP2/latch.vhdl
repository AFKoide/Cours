LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

-- Verrou D
ENTITY latch IS
	PORT (
		d, clk, reset : IN std_logic; -- Protéger contre écriture
		q, qb         : OUT std_logic -- Protéger contre lecture
	);
END latch;

-- Programme
ARCHITECTURE bascule_d OF latch IS
SIGNAL temp_q : STD_LOGIC;
BEGIN
	copie : PROCESS (clk, reset) BEGIN
		IF reset = '1' THEN
			temp_q <= '0';
		ELSIF clk = '1' THEN
			temp_q <= d;
		ELSE
			temp_q <= temp_q;
		END IF;
	END PROCESS;
    q <= temp_q;
    qb <= not(temp_q);
END bascule_d;
