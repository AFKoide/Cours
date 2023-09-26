entity piston is port (
    start, P_D, P_G, h, reset : in bit;
    Gauche, Droite : out bit
);
end entity;

architecture state_machine for piston is 
    type StateType is (A, D, AD, G, AG)
    signal present_state, next_state : StateType;
    signal cpt : bit_vector(3 downto 0);

    begin
        state_comb : process(pret, present_state, P_D, P_G, cpt)
        begin
            case present_state is
                when A => Gauche <= '0'; Droite <= '0';
                if start = '1' then next_state <= D;
                else next_state <= A;
                end if;

                when D => Gauche <= '0'; Droite <= '1';
                if P_D = '1' then next_state <= AD;
                else next_state <= D;
                end if;

                when AD => Gauche <= '0'; Droite <= '0';
                if cpt = "11" then next_state <= G;
                else next_state <= AD;
                end if;

                when G => Gauche <= '1'; Droite <= '0';
                if P_G = '1' then next_state <= AG;
                else next_state <= G;
                end if;

                when AG => Gauche <= '0'; Droite <= '0';
                if cpt = "11" then next_state <= A;
                else next_state <= AG;
                end if;
            end case;
        end process;

    process(h, reset) begin
        cpt 
        if reset = '1' then
            present_state <= A;
            cpt <= "00";

        elsif h'event and h='1' then
            present_state <= next_state;
            elsif present_state /= next_state then cpt = "00"; -- /= est not
            else cpt <= cpt+1;

        end if;
    end process;
end state_machine;

-- Un signal ne peut etre piloté que par un seul processus. Cpt ne peut donc pas être initialisé par state_comb : il faut que ce soit l'horloge qui s'en occupe.
-- On initialisera donc cpt a chaque changement d'état.