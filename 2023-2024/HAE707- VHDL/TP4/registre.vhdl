LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;
USE IEEE.MATH_REAL.all;
USE ieee.numeric_std.all;

entity registre is port (
        h : IN std_logic;
        config : in std_logic;
        nbre_de_top : OUT std_logic_vector(7 downto 0)
    );
end entity;

architecture Behavioral of registre is 
signal start, esp_vide : std_logic := 0;
variable entree : integer := 5;

begin
    process
    begin
        if start = '0' then
            for k in 0 to entree loop
                nbre_de_top <= shift_right(unsigned(nbre_de_top),'1');
            end loop;
            start <= '1';

            
        elsif start = '1' then
            case config is
                when '0' => 
                    for k in 0 to 7 loop
                        if nbre_de_top(k) = '0' then
                            esp_vide <= esp_vide + '1';
                        else
                            esp_vide <= esp_vide;
                        end if;
                    end loop;
                when '1' =>
                    if rising_edge(h) then
                        nbre_de_top <= shift_right(unsigned(nbre_de_top),'1');
                        compteur <= compteur + '1';
            end case;
        end if;
    end process;
END architecture;