### **Dérivation mathématique complète de l'intersection de trois sphères**

Nous allons maintenant détailler **chaque étape mathématique** permettant de trouver l'intersection de trois sphères en 3D. Nous expliquerons pourquoi il est essentiel de connaître le **centre du cercle d'intersection** et son **rayon**, avant de trouver les points finaux.

---

## **1. Équation des sphères**
Chaque sphère \( S_i \) est définie par son équation quadratique :

\[
(x - x_i)^2 + (y - y_i)^2 + (z - z_i)^2 = r_i^2
\]

où :
- \( (x_i, y_i, z_i) \) est le centre de la sphère,
- \( r_i \) est le rayon de la sphère,
- \( (x, y, z) \) représente un point sur la sphère.

Lorsque **deux sphères** s'intersectent, elles définissent un **cercle** commun. L’intersection d’une **troisième sphère** avec ce cercle donne **deux points distincts**.

---

## **2. Intersection de deux sphères : Détermination du cercle**
L’intersection de deux sphères \( S_1 \) et \( S_2 \) est un **cercle** dans l’espace.

### **2.1 Équation du plan contenant le cercle**
On commence par soustraire les équations des sphères \( S_1 \) et \( S_2 \) :

\[
(x - x_1)^2 + (y - y_1)^2 + (z - z_1)^2 = r_1^2
\]

\[
(x - x_2)^2 + (y - y_2)^2 + (z - z_2)^2 = r_2^2
\]

En soustrayant ces deux équations :

\[
(x^2 - 2x x_1 + x_1^2 + y^2 - 2y y_1 + y_1^2 + z^2 - 2z z_1 + z_1^2) - (x^2 - 2x x_2 + x_2^2 + y^2 - 2y y_2 + y_2^2 + z^2 - 2z z_2 + z_2^2)
\]

\[
= r_1^2 - r_2^2
\]

Ce qui donne l'équation d'un plan :

\[
-2x (x_1 - x_2) - 2y (y_1 - y_2) - 2z (z_1 - z_2) = r_1^2 - r_2^2 + x_2^2 - x_1^2 + y_2^2 - y_1^2 + z_2^2 - z_1^2
\]

Ce plan contient **le cercle d’intersection des deux sphères**.

### **2.2 Détermination du centre du cercle**
Le centre du cercle \( P \) se trouve sur l’axe joignant \( (x_1, y_1, z_1) \) et \( (x_2, y_2, z_2) \), à une distance donnée par :

\[
a = \frac{r_1^2 - r_2^2 + d^2}{2d}
\]

où :
- \( d \) est la distance entre les centres des sphères :

\[
d = \sqrt{(x_2 - x_1)^2 + (y_2 - y_1)^2 + (z_2 - z_1)^2}
\]

Le point \( P \), centre du cercle, est obtenu par :

\[
P = (x_1, y_1, z_1) + a \cdot \frac{(x_2 - x_1, y_2 - y_1, z_2 - z_1)}{d}
\]

C’est un point du plan d’intersection qui représente le **centre du cercle**.

### **2.3 Détermination du rayon du cercle**
Le rayon du cercle est donné par :

\[
h = \sqrt{r_1^2 - a^2}
\]

où \( a \) est la distance entre le centre de la sphère 1 et \( P \).

**Pourquoi avons-nous besoin de connaître le centre et le rayon du cercle ?**
- **Le centre \( P \) nous permet de localiser le cercle dans l'espace.**
- **Le rayon \( h \) est nécessaire pour déterminer les points d’intersection avec la troisième sphère.**
- **Sans ces informations, il est impossible de localiser correctement les points finaux d'intersection.**

---

## **3. Intersection du cercle avec la troisième sphère**
La troisième sphère \( S_3 \) intercepte ce cercle en **deux points distincts**.

Nous devons maintenant résoudre l’équation suivante :

\[
(x - x_3)^2 + (y - y_3)^2 + (z - z_3)^2 = r_3^2
\]

Sachant que \( (x, y, z) \) appartient déjà au plan défini par \( S_1 \) et \( S_2 \), nous exprimons ses coordonnées en fonction de ce plan.

Nous définissons un **système de coordonnées local** avec :
- Un axe **ex** le long de \( S_1 S_2 \).
- Un axe **ey** perpendiculaire dans le plan.
- Un axe **ez** perpendiculaire au plan.

Ces axes sont définis par :

```python
ex = (B - A) / d12  # Vecteur unitaire sur l'axe x
i = np.dot(ex, (C - A))  # Projection de C sur x
ey = (C - A - i * ex) / np.linalg.norm(C - A - i * ex)  # Axe y dans le plan
ez = np.cross(ex, ey)  # Axe z orthogonal au plan
```

Nous obtenons alors un système d'équations :

\[
x = \frac{r_1^2 - r_2^2 + d_{12}^2}{2 d_{12}}
\]

\[
y = \frac{r_1^2 - r_3^2 + d_{13}^2 - 2 i x}{2 \cdot ey \cdot (C - A)}
\]

\[
z = \pm \sqrt{r_1^2 - x^2 - y^2}
\]

Si \( z^2 < 0 \), alors **aucune intersection réelle** n’existe, ce qui signifie que les sphères ne se croisent pas.

Les **deux solutions finales** sont :

```python
z = np.sqrt(z_squared)
intersection1 = A + x * ex + y * ey + z * ez
intersection2 = A + x * ex + y * ey - z * ez
```

Elles correspondent aux **deux points d'intersection des trois sphères**.

---

## **4. Conclusion**
L'intersection de trois sphères est obtenue en trois étapes :
1. **Déterminer le plan d'intersection des deux premières sphères.**
2. **Trouver le centre et le rayon du cercle d'intersection.**
3. **Résoudre l'intersection de la troisième sphère avec ce cercle.**

La connaissance du **centre du cercle** et de son **rayon** est essentielle pour localiser correctement les points finaux, car **sans cela, il est impossible de restreindre la solution à deux points distincts**.

Si tu as besoin de détails supplémentaires sur une étape, fais-moi signe ! 🚀