# Démonstration 1 — Les deux ordres

## Manipulation avec un objet réel

Je pars du même point de départ et je prends un objet tenu dans ma main.

### Tourner puis avancer

1. Je place l'objet au point de départ.
2. Je tourne l'objet de 90° vers la droite.
3. J'avance ensuite d'une distance de 1 m dans la direction obtenue après la rotation.

### Avancer puis tourner

1. Je repars exactement du même point de départ.
2. J'avance d'abord de 1 m dans la direction initiale.
3. Je tourne ensuite l'ensemble de 90° autour du même repère.

Les deux manipulations commencent au même point, mais elles n'arrivent pas au même endroit. Cela montre que les transformations ne sont pas commutatives.

## Vérification par le programme

Convention utilisée :

- avant = `(0, 0, -1)`
- rotation de 90° autour de Y
- translation de 1 m vers l'avant

```cpp
#include <iostream>
#include <cmath>
using namespace std;

struct Vecteur {
    double x, y, z;
};

struct Quaternion {
    double w, x, y, z;
};

Quaternion Multiplier(Quaternion a, Quaternion b) {
    return {
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w
    };
}

Quaternion Conjugue(Quaternion q) {
    return {q.w, -q.x, -q.y, -q.z};
}

Vecteur Tourner(Vecteur point, Quaternion rotation) {
    Quaternion pointQuaternion = {0, point.x, point.y, point.z};
    Quaternion resultat = Multiplier(Multiplier(rotation, pointQuaternion), Conjugue(rotation));
    return {resultat.x, resultat.y, resultat.z};
}

Vecteur Additionner(Vecteur a, Vecteur b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vecteur TournerPuisAvancer(Vecteur point, Quaternion rotation, Vecteur avance) {
    return Additionner(Tourner(point, rotation), avance);
}

Vecteur AvancerPuisTourner(Vecteur point, Quaternion rotation, Vecteur avance) {
    return Tourner(Additionner(point, avance), rotation);
}

int main() {
    double angle = M_PI / 2.0;
    Quaternion rotation = {
        cos(angle / 2.0),
        0,
        sin(angle / 2.0),
        0
    };

    Vecteur point = {1, 0, 0};
    Vecteur avance = {0, 0, -1};

    Vecteur premier = TournerPuisAvancer(point, rotation, avance);
    Vecteur second = AvancerPuisTourner(point, rotation, avance);

    cout << premier.x << " " << premier.y << " " << premier.z << '\n';
    cout << second.x << " " << second.y << " " << second.z << '\n';
}
```

Résultats obtenus, à l'arrondi près :

```text
0 0 -2
-1 0 -1
```

Le premier ordre applique la rotation au point puis le déplacement. Le second effectue d'abord le déplacement puis applique la rotation au résultat. Les deux résultats sont différents.

## Ce que je montre au tableau

Point de départ :

```text
P = (1, 0, 0)
```

Puis :

```text
tourner → avancer  = (0, 0, -2)
avancer → tourner  = (-1, 0, -1)
```

La manipulation avec l'objet réel et le programme donnent le même phénomène : l'ordre des transformations change le résultat.
