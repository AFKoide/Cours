entity exercice_1 is port (
    pret, R_W, h, reset : in bit;
    OE, WE : out bit
);

architecture state_machine for example is 
    type StateType is (Attente, Lecture, Ecriture, Decision);
    signal present_state, next_state : StateType;
begin
state_comb : process(pret, present_state, R_W)
begin
    case present_state is
        when Attente => OE <= '0';  WE <= '0';
        if pret = '1' then next_state <= Decision;
        else next_state <= Attente;
        end if;

        when Decision => OE <= '0';  WE <= '0';
        if R_W = '0' then next_state <= Lecture;
        else next_state <= Ecriture;
        end if;

        when Lecture => OE <= '1';  WE <= '0';
        if pret = '1' then next_state <= Attente;
        else next_state <= Lecture;
        end if;

        when Ecriture => OE <= '0';  WE <= '1';
        if pret = '1' then next_state <= Attente;
        else next_state <= Ecriture;
        end if;

        when others ;
    end case;
end process;

-- SOLUTION SYNCHRONE
state_clocked  : process(h,reset) begin
    if (reset = '1') then
        present_state <= Attente;
    elsif(h'event and h='1') then
        present_state <= next_state;
    end if;

-- SOLUTION ASYNCHRONE
-- On ajoute dans le reset dans le case et il initialisera 



