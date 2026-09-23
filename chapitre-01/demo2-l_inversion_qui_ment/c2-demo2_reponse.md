# Démonstration 2 — L'inversion qui ment

## Une inversion générale sans contrôle de dégénérescence

Je montre ici un comportement trompeur : une procédure d'inversion générale peut laisser croire qu'elle a réussi alors que la matrice n'est pas inversible.

Je pars d'une matrice dégénérée composée uniquement de zéros.

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

const int Taille = 4;

void Afficher(double matrice[Taille][Taille]) {
    cout << fixed << setprecision(1);
    for (int ligne = 0; ligne < Taille; ligne++) {
        for (int colonne = 0; colonne < Taille; colonne++) {
            cout << matrice[ligne][colonne] << " ";
        }
        cout << '\n';
    }
}

void InverserSansControle(double matrice[Taille][Taille], double inverse[Taille][Taille]) {
    double travail[Taille][Taille];

    for (int ligne = 0; ligne < Taille; ligne++) {
        for (int colonne = 0; colonne < Taille; colonne++) {
            travail[ligne][colonne] = matrice[ligne][colonne];
            inverse[ligne][colonne] = ligne == colonne ? 1.0 : 0.0;
        }
    }

    for (int pivot = 0; pivot < Taille; pivot++) {
        if (fabs(travail[pivot][pivot]) < 1e-9) {
            continue;
        }

        double valeur = travail[pivot][pivot];

        for (int colonne = 0; colonne < Taille; colonne++) {
            travail[pivot][colonne] /= valeur;
            inverse[pivot][colonne] /= valeur;
        }

        for (int ligne = 0; ligne < Taille; ligne++) {
            if (ligne == pivot) {
                continue;
            }

            double facteur = travail[ligne][pivot];

            for (int colonne = 0; colonne < Taille; colonne++) {
                travail[ligne][colonne] -= facteur * travail[pivot][colonne];
                inverse[ligne][colonne] -= facteur * inverse[pivot][colonne];
            }
        }
    }
}

int main() {
    double matrice[Taille][Taille] = {};
    double inverse[Taille][Taille];

    InverserSansControle(matrice, inverse);
    Afficher(inverse);
}
```

Sortie :

```text
1.0 0.0 0.0 0.0
0.0 1.0 0.0 0.0
0.0 0.0 1.0 0.0
0.0 0.0 0.0 1.0
```

Le programme affiche donc l'identité, sans aucun message.

Pourtant, une matrice nulle n'a pas d'inverse. L'identité affichée n'est donc pas une inversion réussie : elle est simplement restée dans le tableau résultat parce qu'aucun pivot n'a pu être traité.

## Mise en situation dans un casque

Si cette erreur était utilisée pour calculer une matrice de vue, le système pourrait se comporter comme si l'inversion de la pose avait réussi.

La conséquence visible serait :

```text
position de caméra → origine
rotation de caméra → identité
```

La caméra reviendrait donc à l'origine, sans rotation, alors que la matrice fournie n'était pas inversible.

Le problème est particulièrement trompeur parce qu'aucun message ne signale l'échec.

## Conclusion

Une inversion générale doit vérifier qu'un pivot existe avant de poursuivre. Sinon, une matrice dégénérée peut produire une fausse identité et donner l'impression que tout fonctionne.
