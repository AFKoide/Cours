LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

-- Compteur 8 bits
ENTITY feu IS 
	PORT (
        ReqH, ReqV : IN STD_LOGIC;
        h, reset   : IN STD_LOGIC;
        RH, YH, VH : OUT STD_LOGIC;
        RV, YV, VV : OUT std_logic);
END ENTITY;

-- Programme
ARCHITECTURE Behaviour OF feu IS
SIGNAL compteur : std_logic_vector(3 DOWNTO 0) := "0000";
SIGNAL memoire  : std_logic_vector(3 downto 0) := "0000";
signal alarme   : std_logic := '0';

type StateType is (R_V,R_J,V_R,J_R);                                                 -- [Vertical]-[Horizontal]
signal present_state, next_state : StateType;
BEGIN
    clk : process(h, reset) BEGIN                                                    -- Horloge
        IF reset = '1' THEN
            compteur <= "0000";
            present_state <= R_V;
        ELSIF h'event and h='0' THEN
            compteur <= compteur + '1';
        ELSIF present_state /= next_state THEN
            compteur <= "0000";
            present_state <= next_state;
        END IF;
    END PROCESS;


    state_comb : process(ReqH,ReqV,compteur,present_state) BEGIN
        CASE present_state is
            WHEN R_V =>
                RV <= '1'; YV <= '0'; VV <= '0';
                RH <= '0'; YH <= '0'; VH <= '1';
                IF compteur = 13 OR (alarme = '1' AND (compteur - memoire = 3)) THEN -- Prioritaire.
                    next_state <= R_J;
                    alarme <= '0';
                ELSIF ReqH = '1' AND alarme = '0' THEN                               -- A faire si appuie sur le bouton.
                    memoire <= compteur;
                    alarme <= '1';
                ELSE next_state <= present_state;
                END IF;

            WHEN R_J =>
                RV <= '1'; YV <= '0'; VV <= '0';
                RH <= '0'; YH <= '1'; VH <= '0';
                alarme <= '0';
                IF compteur = 2 THEN
                    next_state <= V_R;
                ELSE next_state <= present_state;
                END if;

            WHEN V_R =>
                RV <= '0'; YV <= '0'; VV <= '1';
                RH <= '1'; YH <= '0'; VH <= '0';
                IF compteur = 13 OR (alarme = '1' AND (compteur - memoire = 3)) THEN -- Prioritaire.
                    next_state <= J_R;
                    alarme <= '0';
                ELSIF Reqv = '1' AND alarme = '0' THEN                               -- A faire si appuie sur le bouton.
                    memoire <= compteur;
                    alarme <= '1';
                ELSE next_state <= present_state;
                END IF;

            WHEN J_R =>
                RV <= '0'; YV <= '1'; VV <= '0';
                RH <= '1'; YH <= '0'; VH <= '0';
                alarme <= '0';
                IF compteur = 2 THEN
                    next_state <= R_V;
                ELSE next_state <= present_state;
                END if;
            END CASE;
    END PROCESS;
END Behaviour;