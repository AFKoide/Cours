LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

-- Bascule JK
ENTITY flip_flop IS
	PORT (
		j, k, clk, reset : IN std_logic;
		q                : OUT std_logic
	);
END flip_flop;

-- Programme
ARCHITECTURE bascule OF flip_flop IS
SIGNAL temp_q : STD_LOGIC;
BEGIN
	memoire       : PROCESS (clk, reset)
	BEGIN
        IF RESET ='1' THEN
            temp_q <= '0'; 
		ELSIF clk'EVENT AND clk = '1' THEN
			IF j = '0' AND k = '0' THEN
				temp_q <= q;
			ELSIF j = '0' AND k = '1' THEN
				temp_q <= '0';
			ELSIF j = '1' AND k = '0' THEN
				temp_q <= '1';
			ELSIF j = '1' AND k = '1' THEN
				temp_q <= NOT(q);
			END IF;
		END IF;
	END PROCESS;
    q <= temp_q;
END bascule;    