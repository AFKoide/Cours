LIBRARY IEEE;                   
USE IEEE.STD_LOGIC_1164.ALL;    
USE IEEE.std_logic_arith.all;   
USE IEEE.std_logic_unsigned.all;

-- Compteur 8 bits
ENTITY compteur IS
	PORT (
        clk, reset, config : in std_logic;
        s                  : out std_logic_vector(7 downto 0)
	);
END compteur;

-- Programme
ARCHITECTURE Behaviour OF compteur IS
SIGNAL temp_s : std_logic_vector(7 downto 0);
BEGIN
	PROCESS (clk, reset, temp_s, config) BEGIN
        IF RESET ='1' THEN
            temp_s <= "00000000"; 
        ELSIF clk'EVENT AND clk = '1' AND config = '0' THEN 
            temp_s <= temp_s + '1';
        ELSIF clk'EVENT AND clk = '1' AND config = '1' THEN
            temp_s <= temp_s - '1';
        END if;
	END PROCESS;
    s <= temp_s;
END Behaviour;
