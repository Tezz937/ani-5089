# Démonstration 3 — Le tour complet à l'envers

## Vitesse angulaire sans chemin court

Deux quaternions opposés représentent la même orientation. Un petit écart autour de cette orientation peut donc être interprété de deux façons si le chemin court n'est pas imposé.

Je prends un delta très petit autour de l'opposé de l'identité :

```text
départ = (1, 0, 0, 0)
arrivée = (-0.9999995, 0.001, 0, 0)
```

Après normalisation, le produit scalaire est presque égal à `-1`.

Avec la formule :

```text
angle = 2 × acos(dot)
vitesse angulaire = angle / durée
```

le programme sans chemin court interprète ce petit delta comme une rotation presque complète.

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

struct Quaternion {
    double w, x, y, z;
};

double Norme(Quaternion q) {
    return sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
}

Quaternion Normaliser(Quaternion q) {
    double norme = Norme(q);
    return {q.w / norme, q.x / norme, q.y / norme, q.z / norme};
}

double ProduitScalaire(Quaternion a, Quaternion b) {
    return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}

double VitesseAngulaire(Quaternion depart, Quaternion arrive, double duree) {
    depart = Normaliser(depart);
    arrive = Normaliser(arrive);

    double produit = ProduitScalaire(depart, arrive);
    produit = max(-1.0, min(1.0, produit));

    double angle = 2.0 * acos(produit);
    return angle / duree;
}

double VitesseAngulaireCheminCourt(Quaternion depart, Quaternion arrive, double duree) {
    depart = Normaliser(depart);
    arrive = Normaliser(arrive);

    double produit = ProduitScalaire(depart, arrive);

    if (produit < 0.0) {
        arrive.w = -arrive.w;
        arrive.x = -arrive.x;
        arrive.y = -arrive.y;
        arrive.z = -arrive.z;
        produit = -produit;
    }

    produit = max(-1.0, min(1.0, produit));

    double angle = 2.0 * acos(produit);
    return angle / duree;
}

int main() {
    Quaternion depart = {1, 0, 0, 0};
    Quaternion arrive = {-0.9999995, 0.001, 0, 0};
    double duree = 0.01;

    cout << fixed << setprecision(6);
    cout << VitesseAngulaire(depart, arrive, duree) << '\n';
    cout << VitesseAngulaireCheminCourt(depart, arrive, duree) << '\n';
}
```

Résultat obtenu à l'arrondi près :

```text
628.118531
0.200000
```

La première valeur correspond à une rotation presque complète pendant seulement `10 ms`. Le mouvement paraît donc énorme alors que le delta réel est minuscule.

## Les trois lignes qui corrigent le problème

Il suffit d'ajouter le contrôle du signe du produit scalaire avant de calculer l'angle :

```cpp
if (produit < 0.0) {
    arrive.w = -arrive.w;
    arrive.x = -arrive.x;
    arrive.y = -arrive.y;
    arrive.z = -arrive.z;
}
```

On force ainsi le choix du chemin court.

Après correction, la vitesse angulaire devient petite et correspond au petit mouvement réellement effectué.

## Conclusion

Sans chemin court, deux quaternions représentant pratiquement la même orientation peuvent produire une vitesse angulaire artificiellement énorme. Le signe du quaternion doit donc être traité avant le calcul de l'angle.
