import math

# ----------------------
# Constantes
## Angles
theta10 = 20            # Angle entre l'axe x du repère global et l'axe x du bras 1.
theta20 = 20            # Angle entre l'axe x du repère global et l'axe y du bras 2.

theta1 = 0                  # Angle suivant l'axe x de l'organe terminal.
theta2 = 0                  # Angle suivant l'axe y de l'organe terminal.


## Distances
O_T     = 122.5         # Longueur entre le centre du repère et l'organe terminal.

### Bras 1
O_A1    = 122.5         # Longueur entre le centre du repère et le point A1 (origine du bras 1)
A1_A2   = 73.542        # Longueur entre le point A1 ('origine' du bras) et A2 (première pliure du bras).
A1_A4   = 73.542        # Longueur entre le point A1 et A4 (deuxième pliure du bras).
# A1_A3 a déterminer    # Longueur entre le point A1 et A3 (pliure centrale du bras).
# A1_T  a déterminer    # Longueur entre le point A1 et T (organe terminal).

### Bras 2
O_B1    = 142.5         # Longueur entre le centre du repère et le point B1 (origine du bras 2)
B1_B2   = 85.549        # Longueur entre le point B1 ('origine' du bras) et B2 (première pliure du bras).
B1_B4   = 85.549        # Longueur entre le point B1 et B4 (deuxième pliure du bras).
# B1_B3 a déterminer    # Longueur entre le point B1 et B3 (pliure centrale du bras).
# B1_T  a déterminer    # Longueur entre le point B1 et T (organe terminal).


# ----------------------
# Déclaration fonctions 
def pythagore_general(a, b, angle):
  """Calcul de la longueur du côté opposé à l'angle dans un triangle quelconque."""
  return math.sqrt(a**2 + b**2 - 2 * a * b * math.cos(angle))


# ----------------------
# Modèle inverse

# Correction des angles theta1 et theta2 pour prise en compte de l'alignement sur Z
theta1_corrige = math.radians(90 - theta1 - theta10)
theta2_corrige = math.radians(90 - theta2 - theta20)

# Calcul des longueurs A1_T et B1_T basées sur la géométrie du parallélogramme
A1_T = math.sqrt(O_A1**2 + O_T**2 - 2 * O_A1 * O_T * math.cos(theta1_corrige))
B1_T = math.sqrt(O_B1**2 + O_T**2 - 2 * O_B1 * O_T * math.cos(theta2_corrige))

# Calcul des angles articulaires β1 et β2
beta1 = 2 * math.acos(A1_T / (4 * A1_A2))
beta2 = 2 * math.acos(B1_T / (4 * B1_B2))

print("beta1 =", math.degrees(beta1), "°")
print("beta2 =", math.degrees(beta2), "°")
