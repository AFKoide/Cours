LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

-- Bascule D
ENTITY latch IS
	PORT (
		e, d, reset : IN std_logic; -- Protéger contre écriture
		q           : OUT std_logic -- Protéger contre lecture
	);
END latch;

-- Programme
ARCHITECTURE bascule_d OF latch IS
SIGNAL temp_q : STD_LOGIC;
BEGIN
	latch : PROCESS (e,d,reset) BEGIN
		IF reset = '1' THEN
			temp_q <= '0';
		ELSIF e = '1' THEN
			temp_q <= d;
		ELSE
			temp_q <= temp_q;
		END IF;
	END PROCESS;
    q <= temp_q;
    qb <= NOT(temp-q);
END bascule_d;
