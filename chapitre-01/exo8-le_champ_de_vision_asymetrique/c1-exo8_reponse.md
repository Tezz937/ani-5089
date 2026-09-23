# Exercice 8 — Le champ de vision asymétrique

Casque étudié : **Oculus Rift DK1**

Pour l'œil gauche, le guide développeur Oculus donne les quatre demi-angles suivants :

- Gauche / vers l'extérieur : **58,7°**
- Droite / vers le nez : **50,3°**
- Haut : **53,6°**
- Bas : **58,9°**

La documentation précise qu'il s'agit de demi-angles mesurés depuis l'axe de vision et que le champ horizontal total est donc de 109,0°, tandis que le champ vertical total est de 112,5°.

**Source : Oculus Rift Developer Guide, chapitre “Advanced Rendering Configuration”.**

Si on remplaçait ce champ asymétrique par un champ symétrique de même surface, on déplacerait la répartition de la visibilité autour de l'axe de vision et on ne conserverait donc pas exactement les mêmes zones visibles sur les quatre côtés.
