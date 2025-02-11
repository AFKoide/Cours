library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity ex4 is port(
    E : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    h, reset : IN STD_LOGIC;
    S : OUT STD_LOGIC);
end ex4;

architecture Behavioral of ex4 is
type StateType is (M0,M1,M2,M3,M4);
signal present_state, next_state : StateType;

begin
    clk : process(h, reset) BEGIN
        IF reset = '1' THEN 
            present_state <= M0;
        ELSIF rising_edge(h) THEN
            present_state <= next_state;
        END IF;
    END PROCESS;

    state_comb : process(h,E) BEGIN
        IF rising_edge(h) THEN
            CASE present_state is
                WHEN M0 => 
                    S <= '0';
                    IF E = "0110" THEN 
                        next_state <= M1;
                    END IF;
                    
                WHEN M1 =>
                    IF E = "1001" THEN
                        next_state <= M2;
                    ELSE 
                        next_state <= M0;
                    END IF;
                    
                WHEN M2 =>
                    IF E = "1100" THEN
                        next_state <= M3;
                    ELSE 
                        next_state <= M0;
                    END IF;

                WHEN M3 =>
                    IF E = "0011" THEN
                        next_state <= M4;
                    ELSE 
                        next_state <= M0;
                    END IF;

                WHEN M4 =>
                    S <= '1';
                    next_state <= M0;

                WHEN OTHERS =>
                    next_state <= M0;
            END CASE;
        END IF;
    END PROCESS;
end Behavioral;
