# Exercice 6 — Le bras, en poses

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
    Quaternion pointQuaternion{0, point.x, point.y, point.z};
    Quaternion resultat = Multiplier(Multiplier(rotation, pointQuaternion), Conjugue(rotation));
    return {resultat.x, resultat.y, resultat.z};
}

Pose Composer(Pose parent, Pose enfant) {
    Vecteur translation = Tourner(enfant.position, parent.rotation);

    return {
        {
            parent.position.x + translation.x,
            parent.position.y + translation.y,
            parent.position.z + translation.z
        },
        Multiplier(parent.rotation, enfant.rotation)
    };
}

int main() {
    Pose epaule{{0, 0, 0}, {1, 0, 0, 0}};
    Pose coude{{1, 0, 0}, {1, 0, 0, 0}};
    Pose main{{1, 0, 0}, {1, 0, 0, 0}};

    Pose coudeMonde = Composer(epaule, coude);
    Pose mainMonde = Composer(coudeMonde, main);

    cout << "Coude : " << coudeMonde.position.x << ' '
         << coudeMonde.position.y << ' ' << coudeMonde.position.z << '\n';
    cout << "Main : " << mainMonde.position.x << ' '
         << mainMonde.position.y << ' ' << mainMonde.position.z << '\n';

    double angle = acos(-1.0) / 2.0;
    epaule.rotation = {cos(angle / 2.0), 0, 0, sin(angle / 2.0)};

    coudeMonde = Composer(epaule, coude);
    mainMonde = Composer(coudeMonde, main);

    cout << fixed << setprecision(4);
    cout << "Main apres rotation de l'epaule : "
         << mainMonde.position.x << ' '
         << mainMonde.position.y << ' '
         << mainMonde.position.z << '\n';
}
```

Avant rotation, le coude est en `(1,0,0)` et la main en `(2,0,0)`. Après une rotation de 90 degrés de l'épaule autour de Z, la main suit et se retrouve en `(0,2,0)`.
