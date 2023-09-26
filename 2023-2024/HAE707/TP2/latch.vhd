library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- Bascule D
entity latch is port (
    d, clk, reset : in std_logic;
    q : out std_logic
);
end latch;

-- Programme
architecture bascule_d OF latch is 
begin
    signal temp_q: std_logic;

    copie: process(d, clk, reset) begin
        if clk'event and clk='1' then temp_q <= not(q);
        else temp_d <= q;
        end if;

    q <= temp_d;
    end process;
end bascule_d;