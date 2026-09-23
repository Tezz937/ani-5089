# Exercice 12 — Vingt millisecondes, senties

J'ai testé un programme qui fait suivre à un objet la position de la souris avec un retard réglable.

| Personne | Retard à partir duquel une différence est ressentie |
|---|---:|
| Personne 1 | 27 ms |
| Personne 2 | 34 ms |
| Personne 3 | 41 ms |
| Personne 4 | 29 ms |
| Personne 5 | 36 ms |

Les seuils observés sont donc tous supérieurs à 20 ms, avec une moyenne de **33,4 ms**.

Le budget de 20 ms du chapitre ne signifie pas qu'une personne ne peut absolument rien percevoir au-dessus ou en dessous de cette valeur. Il correspond à un budget technique pour limiter le décalage entre le mouvement réel et l'image affichée.

Dans un casque, le seuil perceptible peut être plus bas parce que la chaîne ne contient pas seulement le retard volontaire du programme : il faut aussi tenir compte des capteurs, de la transmission, de la simulation, du rendu, du compositeur et de l'affichage. De plus, le mouvement visuel concerne directement le champ de vision et est comparé aux informations de l'équilibre et du mouvement réel de la tête.
