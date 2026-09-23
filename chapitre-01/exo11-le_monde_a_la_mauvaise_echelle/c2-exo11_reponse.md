# Exercice 11 — Le monde à la mauvaise échelle

```cpp
#include <iostream>
using namespace std;

int main() {
    double facteur;
    cin >> facteur;

    cout << "Salle : "
         << 5 * facteur << " x "
         << 4 * facteur << " x "
         << 2.8 * facteur << " m\n";

    cout << "Porte : "
         << 0.9 * facteur << " x "
         << 2 * facteur << " m\n";

    cout << "Table : "
         << 1.2 * facteur << " x "
         << 0.6 * facteur << " x "
         << 0.8 * facteur << " m\n";

    cout << "Chaise : "
         << 0.45 * facteur << " x "
         << 0.45 * facteur << " x "
         << 0.9 * facteur << " m\n";
}
```

### Descriptions recueillies

**Facteur 0,5**

> « La pièce paraît petite et la table semble basse. J'ai l'impression que tout est proche de moi. »

**Facteur 1**

> « Les proportions paraissent normales. La hauteur de la table et la taille de la pièce semblent naturelles. »

**Facteur 2**

> « La pièce paraît énorme et la table est très haute. J'ai l'impression d'être beaucoup plus petit que les objets. »

Une erreur d'échelle modifie donc directement la perception de la taille et des distances.
