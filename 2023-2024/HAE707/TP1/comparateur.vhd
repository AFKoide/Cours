library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity comparateur is
    Port ( a : in STD_LOGIC_VECTOR(3 DOWNTO 0);
           b : in STD_LOGIC_VECTOR(3 DOWNTO 0);
           s1 : out STD_LOGIC;
           s2 : out STD_LOGIC;
           s3 : out STD_LOGIC
           );
end comparateur;

architecture Behavioral of comparateur is
begin
    s1<='0';
    s2<='0';
    s3<='0';
    s1 <= '1' WHEN a > b ELSE s2 <= '1' WHEN a < b ELSE s3 <= '1';
end Behavioral;