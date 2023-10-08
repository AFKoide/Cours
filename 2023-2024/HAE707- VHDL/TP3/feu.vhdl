LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

-- Compteur 8 bits
ENTITY feu IS
	PORT (
        ReqV, ReqH             : IN STD_LOGIC;
        h, r                   : IN STD_LOGIC;
        RH, YH, VH, RV, YV, VV : OUT STD_LOGIC
	);
END feu;

-- Programme
ARCHITECTURE Behaviour OF feu IS
SIGNAL compteur : std_logic_vector(3 DOWNTO 0) := "0000";
type StateType is (R_H, Y_H, V_H, R_V, Y_V, V_V);
signal present_state_v, next_state_v, present_state_h, next_state_h : StateType;
BEGIN
        clk : process (h,r) BEGIN
                IF r = '1' THEN
                        compteur <= "0000";

                ELSIF rising_edge(h) THEN
                        compteur <= compteur + '1';
                        present_state_v <= next_state_v;
                        present_state_h <= next_state_h;
                END if;
        END process;



        state_comb_h : process(ReqH,ReqV,compteur) BEGIN
                case present_state_h is 
                        WHEN V_H => VH <= '1';
                        IF compteur = 14 THEN next_state_h <= Y_H;
                        ELSE next_state_h <= R_H;
                        END IF;

                        WHEN Y_H => YH <= '1';
                        if compteur = 3 THEN next_state_h <= R_H;
                        ELSE next_state_h <= R_H;
                        END IF;

                        WHEN R_H => RH <= '1';
                        if compteur = 10 THEN next_state_h <= V_H;
                        ELSE next_state_h <= R_H;
                        END IF;

                END case;
        END process;


        state_comb_v : process(ReqH,ReqV,compteur) BEGIN
                case present_state_v is 
                        WHEN V_V => VV <= '1';
                        if compteur = 14 THEN next_state_v <= Y_V;
                        ELSE next_state_v <= R_H;
                        END IF;

                        WHEN Y_V => YV <= '1';
                        if compteur = 3 THEN next_state_v <= R_V;
                        ELSE next_state_v <= R_H;
                        END IF;

                        WHEN R_V => RV <= '1';
                        if compteur = 10 THEN next_state_v <= V_V;
                        ELSE next_state_v <= R_H;
                        END IF;

                END case;
        END process;
END Behaviour;