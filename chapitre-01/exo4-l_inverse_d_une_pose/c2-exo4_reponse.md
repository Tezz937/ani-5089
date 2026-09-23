# Exercice 4 — L'inverse d'une pose

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

struct Vecteur {
    double x, y, z;
};

struct Quaternion {
    double w, x, y, z;
};

struct Pose {
    Vecteur position;
    Quaternion rotation;
};

Quaternion Conjugue(Quaternion q) {
    return {q.w, -q.x, -q.y, -q.z};
}

Quaternion Multiplier(Quaternion a, Quaternion b) {
    return {
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w
    };
}

Vecteur Tourner(Vecteur point, Quaternion rotation) {
    Quaternion pointQuaternion{0, point.x, point.y, point.z};
    Quaternion resultat = Multiplier(Multiplier(rotation, pointQuaternion), Conjugue(rotation));
    return {resultat.x, resultat.y, resultat.z};
}

Vecteur Appliquer(Pose pose, Vecteur point) {
    Vecteur resultat = Tourner(point, pose.rotation);
    return {
        resultat.x + pose.position.x,
        resultat.y + pose.position.y,
        resultat.z + pose.position.z
    };
}

Pose Inverser(Pose pose) {
    Quaternion rotationInverse = Conjugue(pose.rotation);
    Vecteur positionInverse{
        -pose.position.x,
        -pose.position.y,
        -pose.position.z
    };
    Vecteur positionFinale = Tourner(positionInverse, rotationInverse);
    return {positionFinale, rotationInverse};
}

double Distance(Vecteur a, Vecteur b) {
    double x = a.x - b.x;
    double y = a.y - b.y;
    double z = a.z - b.z;
    return sqrt(x*x + y*y + z*z);
}

int main() {
    Pose pose{{2, 1, -3}, {0.9238795, 0, 0.3826834, 0}};
    Vecteur point{1, 2, 0};

    Vecteur transforme = Appliquer(pose, point);
    Vecteur retour = Appliquer(Inverser(pose), transforme);

    cout << fixed << setprecision(10);
    cout << "Ecart : " << Distance(point, retour) << '\n';
}
```

L'inverse utilise le conjugué du quaternion et la position opposée tournée par ce conjugué. Après application de la pose puis de son inverse, l'écart doit être nul aux erreurs d'arrondi près.
