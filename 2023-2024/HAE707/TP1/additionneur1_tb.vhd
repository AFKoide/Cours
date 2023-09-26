library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity additionneur1_tb is
end additionneur1_tb;

architecture Behavioral of additionneur1_tb is
    component additionneur1 
        port ( A : in STD_LOGIC_VECTOR(7 DOWNTO 0);
               B : in STD_LOGIC_VECTOR(7 DOWNTO 0);
               R : out STD_LOGIC;
               S : out STD_LOGIC_VECTOR(7 DOWNTO 0);
               COM : IN STD_LOGIC
               );
    end component;

    signal A, B, S : STD_LOGIC_VECTOR(7 DOWNTO 0);
    signal R, COM : STD_LOGIC;

    begin
        c1 : additionneur1
        port map(A, B, COM, S, R);
        tb : process
        BEGIN
            COM <= '1';
            A <= "00000000";
            B <= "00000000";
            WAIT FOR 20ns;
            A <= "00100001";
            B <= "00010001";
            WAIT FOR 20ns;
            A <= "11111111";
            B <= "11111111";
            WAIT FOR 40ns;

            COM <= '0';
            A <= "00000000";
            B <= "00000000";
            WAIT FOR 20ns;
            A <= "00100001";
            B <= "00010001";
            WAIT FOR 20ns;
            A <= "11111111";
            B <= "11111111";
        end process tb;
end Behavioral;