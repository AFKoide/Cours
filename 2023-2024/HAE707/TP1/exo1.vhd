library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity exo1 is
    Port ( A : in STD_LOGIC;
           B : in STD_LOGIC;
           COM : in STD_LOGIC; 
           S : OUT STD_LOGIC);
end exo1;

architecture Behavioral of exo1 is
begin
        S<=A WHEN COM='0' ELSE S<=B;
end Behavioral;