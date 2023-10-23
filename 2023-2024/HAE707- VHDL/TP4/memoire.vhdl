LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

entity memoire is port (
        entree, reset : in STD_LOGIC_VECTOR(3 downto 0);
        mode          : in STD_LOGIC_VECTOR(1 downto 0);
        adresse, h    : in STD_LOGIC;
        sortie        : out STD_LOGIC_VECTOR(3 downto 0)
    );
end entity;

architecture Behavioral of memoire is 
signal R1, R2 : STD_LOGIC_VECTOR(3 downto 0);
begin
    process(h, reset)
    begin
        IF reset = '1' then
            R1 <= "0000";
            R2 <= "0000";
        ELSIF rising_edge(h) then
            CASE mode is
                WHEN "01" =>
                    if adresse = '0' then
                        R1 <= entree;
                    else 
                        R2 <= entree;
                    end if;

                WHEN "10" =>
                    if adresse = '0' then
                        sortie <= R1;
                    else 
                        sortie <= R2;
                    end if;
                
                WHEN "11" =>
            END CASE;
        END IF;
    END PROCESS;
END architecture;