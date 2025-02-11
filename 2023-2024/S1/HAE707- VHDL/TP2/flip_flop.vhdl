LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

-- Bascule JK
ENTITY flip_flop IS
	PORT (
		D, R, clk : IN std_logic;
		q, qb     : OUT std_logic
	);
END flip_flop;

-- Programme
ARCHITECTURE bascule OF flip_flop IS
SIGNAL temp_q : STD_LOGIC;
BEGIN
	asynchrone : PROCESS (D,R,clk)
	BEGIN
        IF R ='1' THEN -- Asynchrone car le reset est prioritaire
            temp_q <= '0'; 
		ELSIF clk'EVENT AND clk = '1' THEN
            temp_q <= d;
        ELSE
            temp_q <= temp_q;
            END IF;
	END PROCESS;
    q <= temp_q;
    qb <= NOT(temp_q);
END bascule;

--ARCHITECTURE bascule OF flip_flop IS
--SIGNAL temp_q : STD_LOGIC;
--BEGIN
--	synchrone : PROCESS (D,R,clk)
--	BEGIN
--	IF clk'EVENT AND clk = '1' THEN -- Synchrone car l'horloge est prioritaire
--	   IF R ='1' THEN 
--	       temp_q <= '0';
--	   ELSE
--	       temp_q <= d;
--       END IF;
--    END IF;
--	END PROCESS;
--    q <= temp_q;
--    qb <= NOT(temp_q);
--END bascule;
