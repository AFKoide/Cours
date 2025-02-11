LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

entity train is port
    (
        start, h : IN std_logic;
        s        : OUT std_logic
    );
end entity;

-- architecture Behavioral of train is 
--     signal compteur : std_logic_vector(2 downto 0) := "000";
--     signal processus : std_logic := '0';
--     begin
--         process(h, start, compteur) begin
--             if compteur = 7 then
--                 compteur <= compteur + '1'; 
--                 processus <= '0';
--                 s <= '0';
--             elsif processus = '1' and rising_edge(h) then
--                 compteur <= compteur + '1';
--                 s <= '1';
--             elsif start = '1' then
--                 processus <= '1';
--             end if;
--         end process;
-- end architecture;

architecture Behavioral of train is 
    signal compteur : std_logic_vector(2 downto 0) := "000";
    signal processus : std_logic := '0';
    begin
        process(h, start, compteur) begin
            if compteur = 7 then
                compteur <= compteur + '1'; 
                processus <= '0';
                s <= '0';
            elsif processus = '1' and h'event then 
                if h = '1' then
                    compteur <= compteur + '1';
                end if;
                s <= h;
            elsif start = '1' then
                processus <= '1';
            end if;
        end process;
end architecture;