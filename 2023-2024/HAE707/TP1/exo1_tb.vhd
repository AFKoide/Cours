LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
ENTITY exo1_tb IS
END exo1_tb;

ARCHITECTURE Behavioral OF exo1_tb IS
	-- Component Declaration
	COMPONENT exo1
		PORT (
			A : IN STD_LOGIC;
			B : IN STD_LOGIC;
			COM : IN STD_LOGIC;
			S : OUT STD_LOGIC
		);
	END COMPONENT;
	
	SIGNAL A, B, COM, S : STD_LOGIC;
BEGIN
	-- Component Instantiation
	c1 : exo1
	PORT MAP(A, B, COM, S);
	tb : PROCESS
	BEGIN
		A <= '0';
		B <= '0';
		COM <= '0';
		WAIT FOR 20ns;
		A <= '1';
		B <= '0';
		COM <= '0';
		WAIT FOR 20ns;
		A <= '0';
		B <= '1';
		COM <= '1';
		WAIT FOR 20ns;
		A <= '1';
		B <= '1';
		COM <= '1';
		WAIT FOR 20ns;
	END PROCESS tb;
END Behavioral;