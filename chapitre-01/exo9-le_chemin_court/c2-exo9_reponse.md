# Exercice 9 — Le chemin court

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

struct Quaternion {
    double w, x, y, z;
};

double ProduitScalaire(Quaternion a, Quaternion b) {
    return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}

Quaternion Oppose(Quaternion quaternion) {
    return {
        -quaternion.w,
        -quaternion.x,
        -quaternion.y,
        -quaternion.z
    };
}

Quaternion Normaliser(Quaternion quaternion) {
    double norme = sqrt(ProduitScalaire(quaternion, quaternion));

    return {
        quaternion.w / norme,
        quaternion.x / norme,
        quaternion.y / norme,
        quaternion.z / norme
    };
}

double VitesseAngulaire(Quaternion depart, Quaternion arrivee,
                         double dt, bool cheminCourt) {
    depart = Normaliser(depart);
    arrivee = Normaliser(arrivee);

    double produit = ProduitScalaire(depart, arrivee);

    if (cheminCourt && produit < 0) {
        arrivee = Oppose(arrivee);
        produit = -produit;
    }

    produit = max(-1.0, min(1.0, produit));

    return 2.0 * acos(produit) / dt;
}

int main() {
    Quaternion depart{1, 0, 0, 0};
    Quaternion arrivee{-1, 0, 0, 0};

    cout << "Avec le chemin court : "
         << VitesseAngulaire(depart, arrivee, 1.0, true) << '\n';

    cout << "Sans le chemin court : "
         << VitesseAngulaire(depart, arrivee, 1.0, false) << '\n';
}
```

Les quaternions `(1,0,0,0)` et `(-1,0,0,0)` représentent la même orientation. Avec le chemin court, la vitesse obtenue est nulle. Sans le forçage, la formule donne `2π/dt`, soit environ `6,2832 rad/s` pour `dt = 1 s`, alors que l'orientation de départ et d'arrivée est identique. C'est précisément le résultat absurde recherché.
