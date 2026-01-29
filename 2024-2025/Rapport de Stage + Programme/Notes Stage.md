# Que Mettre dans le Rapport :
## J'ai fait quoi pendant ce Stage :

- Stage au sein de l'équipe ICAR du LIRMM :
  - L'équipe ICAR est un laboratoire dédié à l'imagerie par ordinateur.

<br>

- Le Stage portait sur la base d'un algorithme de notation. Ce dernier doit être capable de privilégier a partir de critère deux objets A et B. Il doit dire si A > B (préférence) ou A ~ B (indifférence).
  - Le Stage consistait à comparer la méthode MacSum et MacSum IMPRÉCIS développé par le prof avec la méthode Linéaire et la méthode GAME.
    - Problème de la méthode Linéaire est qu'elle ne fonctionne qu'avec des données linéaires.
    - Problèmes de la méthode GAME est que l'on doit : 
      - avec le GAME complet, créer 2^n critères à partir de n critère de base.
      - avec le GAME 2-additives, créer un certain nombre de critère, inférieur au game complet, mais tout de même très important.
  - Avantage de MacSum :
    - N'a besoin que de N critères, 
    - Précision doit être entre le Game et le Linéaire.
  - Avantage du MacSum IMPRÉCIS :
    - Doit être aussi précis que MacSum,
    - A besoin d'un paramètre en moins, le seuil.


## L'Algorithme

- L'algorithme a plusieurs paramètres :
  - Le `learning_rate`, la vitesse d'apprentissage de l'algorithme,
  - Le `delta`, un seuil qui permet de différencier si l'on a une préférence ou une indifférence
  - Enfin, le `Gamma`, un coefficient uniquement utilisé pour la méthode *L1*.

<BR>

- Pour chaque méthode, il existe deux manières :
  - La manière des erreurs absolues, aussi appelées L1,
  - La manière des pertes quadratiques, aussi appelées L2.

- Chaque manière a une mise à jour et un calcul de l'erreur différent.


# Explication des termes techniques :

- Capacité

- Game

- 