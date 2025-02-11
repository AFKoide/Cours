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
signal temp_S : std_logic(8 downto 0);
BEGIN
	temp_S <= (A xor B) when COM='0' else temp_S = A-B;
	S <= temp_S(7 downto 0);
	R <= temp_S(8);
END Behavioral;