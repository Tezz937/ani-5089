# Exercice 7 — Le coût du doublement

Sur le même programme que l'exercice précédent, j'ai mesuré le temps consacré au rendu seul, sans la logique.

- Rendu d'une image : **4,8 ms**
- Estimation pour deux yeux : **4,8 × 2 = 9,6 ms**
- Budget à 90 Hz : **11,1 ms**
- Temps restant après deux rendus : **11,1 - 9,6 = 1,5 ms**

Le double rendu prend donc presque tout le budget disponible. Il faudrait surtout réduire le coût du rendu : nombre de pixels, effets graphiques trop lourds, shaders coûteux ou géométrie inutile. Il faut aussi garder une marge pour la logique, les mises à jour et le reste de la chaîne VR.
