library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity comparateur_tb is
end comparateur_tb;

architecture Behavioral of comparateur_tb is
    component comparateur 
        port ( a : in STD_LOGIC_VECTOR(3 DOWNTO 0);
               b : in STD_LOGIC_VECTOR(3 DOWNTO 0);
               s1 : out STD_LOGIC;
               s2 : out STD_LOGIC;
               s3 : out STD_LOGIC);
    end component;

    signal a, b : STD_LOGIC_VECTOR(3 DOWNTO 0);
    signal s1, s2, s3 : STD_LOGIC;

    begin
        c1 : comparateur port map(a,b,s1,s2,s3);
        tb : process
        BEGIN
            a <= "0000";
            b <= "0000";
            WAIT FOR 20ns;
            a <= "1100";
            b <= "0010";
            WAIT FOR 20ns;
            a <= "0010";
            b <= "1111";
            WAIT FOR 20ns;
            a <= "0010";
            b <= "0100";
            WAIT FOR 20ns;
        end process tb;
end Behavioral;