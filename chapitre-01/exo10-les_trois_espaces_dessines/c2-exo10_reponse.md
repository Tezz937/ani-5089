# Exercice 10 — Les trois espaces, dessinés

```text
                    PIECE VUE DE COTE

        plafond
  +-----------------------------------------+
  |                                         |
  |              O tête                     |
  |              │                          |
  |              ● origine VIEW             |
  |              │                          |
  |              ● origine LOCAL            |
  |                                         |
  |                    ┌───────────┐        |
  |                    │   TABLE   │ 0,80 m |
  |                    └───────────┘        |
  |                                         |
  |              ● origine STAGE            |
  +-----------------------------------------+
                    sol : y = 0
```

- **VIEW** : l'origine est entre les deux yeux et suit la tête.
- **LOCAL** : l'origine correspond à la pose de la tête au démarrage et reste stable.
- **STAGE** : l'origine est au sol, au centre de la zone de jeu ; c'est le seul des trois où `y = 0` représente le plancher.

La table est représentée à 0,80 m dans chacun des trois espaces. Les trois positions numériques peuvent donc être identiques dans leur repère respectif, sans représenter le même emplacement physique, puisque les origines ne sont pas les mêmes.

Le choix de STAGE est celui qui convient pour placer physiquement une table dans la salle, car son axe vertical est référencé au sol.
