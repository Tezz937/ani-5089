# Exercice 2 — La pose appliquée

```cpp
#include <iostream>
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
    Vecteur tourne = Tourner(point, pose.rotation);
    return {
        tourne.x + pose.position.x,
        tourne.y + pose.position.y,
        tourne.z + pose.position.z
    };
}

int main() {
    Pose pose;
    Vecteur point;

    cin >> pose.position.x >> pose.position.y >> pose.position.z;
    cin >> pose.rotation.w >> pose.rotation.x >> pose.rotation.y >> pose.rotation.z;
    cin >> point.x >> point.y >> point.z;

    Vecteur resultat = Appliquer(pose, point);
    cout << resultat.x << ' ' << resultat.y << ' ' << resultat.z << '\n';
}
```

La pose applique d'abord la rotation puis la translation.
