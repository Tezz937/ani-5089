# Exercice 1 — Les trois directions

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

struct Vecteur {
    double x, y, z;
};

Vecteur Avant() {
    return {0, 0, -1};
}

Vecteur Haut() {
    return {0, 1, 0};
}

Vecteur Droite() {
    return {1, 0, 0};
}

double ProduitScalaire(Vecteur a, Vecteur b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int main() {
    Vecteur point;
    cin >> point.x >> point.y >> point.z;

    cout << fixed << setprecision(4);
    cout << ProduitScalaire(point, Avant()) << '\n';
    cout << ProduitScalaire(point, Haut()) << '\n';
    cout << ProduitScalaire(point, Droite()) << '\n';
}
```

La convention du module est main droite : avant = `(0,0,-1)`, haut = `(0,1,0)` et droite = `(1,0,0)`.
