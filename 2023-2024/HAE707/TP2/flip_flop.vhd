LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.ALL;
USE IEEE.std_logic_unsigned.ALL;

-- Bascule JK
ENTITY flip_flop IS
	PORT (
		j, k, clk, reset : IN std_logic;
		q                : OUT std_logic
	);
END flip_flop;

-- Programme
ARCHITECTURE bascule OF flip_flop IS
BEGIN
	SIGNAL temp_q : std_logic;
	memoire       : PROCESS (j, k, clk, reset)
	BEGIN
		IF clk'EVENT AND clk = '1' THEN
			IF j = '0' AND k = '0' THEN
				temps_q <= q;
			ELSIF j = '0' AND k = '1' THEN
				temp_q <= '0';
			ELSIF j = '1' AND k = '0' THEN
				temp_q <= '1';
			ELSIF j = '1' AND k = '1' THEN
				temp_q <= NOT(q);
			END IF;
		END IF;
		q <= temp_q;
	END PROCESS;
END bascule;    