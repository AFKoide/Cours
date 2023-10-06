LIBRARY IEEE;                   
USE IEEE.STD_LOGIC_1164.ALL;    
USE IEEE.std_logic_arith.all;   
USE IEEE.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity registre is
    Port ( IN   : in STD_LOGIC_VECTOR(3 DOWNTO 0);
           COM : in STD_LOGIC_VECTOR(1 DOWNTO 0);
           R   : in STD_LOGIC;
           S   : out STD_LOGIC_VECTOR(3 DOWNTO 0));
end registre;

architecture Behavioral of registre is
signal temp_s : STD_LOGIC_VECTOR(3 DOWNTO 0);
begin
    PROCESS (IN, COM, R) BEGIN
        IF R = '1' THEN
            temp_s <= "0000";
        ELSE
            CASE COM IS 
                WHEN "00" =>
                    temp_s <= S;
                WHEN "01" =>
                    temp_s(2 DOWNTO 0) <= temp_s(3 DOWNTO 1);
                    temp_s(3) <= '0';
                WHEN "10" =>
                    temp_s(3 DOWNTO 1) <= temp_s(2 DOWNTO 0);
                    temp_s(0) <= '0';
                WHEN "11" =>
                    temp_s <= temp_s;
                    END CASE;
        END IF;
    END PROCESS;
end Behavioral;
