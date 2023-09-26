library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

ENTITY additionneur1 IS
	PORT (
		COM : IN STD_LOGIC;
		A : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		B : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		R : OUT STD_LOGIC;
		S : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END additionneur1;

ARCHITECTURE Behavioral OF additionneur1 IS
BEGIN
            S <= (A XOR B) WHEN COM='0' ELSE S <= (A  B);
        R <= ;
END Behavioral;