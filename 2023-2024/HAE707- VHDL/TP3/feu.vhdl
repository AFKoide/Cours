LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

-- Compteur 8 bits
ENTITY feu IS 
	PORT (
        ReqV, ReqH             : IN STD_LOGIC;
        h                      : IN STD_LOGIC;
        RH, YH, VH, RV, YV, VV : OUT STD_LOGIC
	);
END ENTITY;

-- Programme
ARCHITECTURE Behaviour OF feu IS
<<<<<<< Updated upstream
SIGNAL compteur_h : std_logic_vector(3 DOWNTO 0) := "0000";
SIGNAL reset_h : std_logic := '0';
SIGNAL compteur_v : std_logic_vector(3 DOWNTO 0) := "0000";
SIGNAL reset_v : std_logic := '0';

type StateType is (R_H, Y_H, V_H, R_V, Y_V, V_V);
signal present_state_v, next_state_v, present_state_h, next_state_h : StateType;
BEGIN
    clk_h : process (h,reset_h) BEGIN
            IF rising_edge(reset_h) THEN
            ELSIF rising_edge(h) THEN
                    compteur_h <= compteur_h + '1';
                    present_state_h <= next_state_h;
            END if;
    END process;

    clk_v : process (h,reset_v) BEGIN
            IF rising_edge(reset_v) THEN
                compteur_v <= "0000";
            ELSIF rising_edge(h) THEN
                    compteur_v <= compteur_v + '1';
                    present_state_v <= next_state_v;
            END if;
    END process;



    state_comb_h : process(ReqH,reset_h,compteur_h) BEGIN
            case present_state_h is 
                    WHEN V_H => VH <= '1';
                    IF ReqH = '1' THEN reset_h <= '1';
                        WAIT UNTIL compteur_h = 3;
                        next_state_h <= Y_H;
                    ELSIF compteur_h = 14 THEN next_state_h <= Y_H;
                    ELSE next_state_h <= R_H;
                    END IF;
=======
SIGNAL compteur    : std_logic_vector(3 DOWNTO 0) := "0000";
SIGNAL compteur_sv : std_logic_vector(3 DOWNTO 0);
type StateType is (R_H, Y_H, V_H, R_V, Y_V, V_V);
signal present_state_v, next_state_v, present_state_h, next_state_h : StateType;
BEGIN
        clk : process (h,r) BEGIN
                IF r = '1' THEN
                        compteur <= "0000";
                        present_state_v <= R_V;
                        present_state_h <= R_H;
                ELSIF rising_edge(h) THEN
                        compteur <= compteur + '1';
                        present_state_v <= next_state_v;
                        present_state_h <= next_state_h;
                END if;
        END process;



        state_comb_h : process(ReqH,ReqV,compteur) BEGIN
                case present_state_h is 
                        WHEN V_H => VH <= '1';
                        IF ReqH = '1' THEN 
                        compteur_sv <= compteur;

                        ELSIF compteur = 14 THEN next_state_h <= Y_H;
                        ELSE next_state_h <= R_H;
                        END IF;
>>>>>>> Stashed changes

                    WHEN Y_H => YH <= '1';
                    if compteur_h = 3 THEN next_state_h <= R_H;
                    ELSE next_state_h <= R_H;
                    END IF;

                    WHEN R_H => RH <= '1';
                    if compteur_h = 10 THEN next_state_h <= V_H;
                    ELSE next_state_h <= R_H;
                    END IF;
            END case;
    END process;
END Behaviour;