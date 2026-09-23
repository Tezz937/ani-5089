# Exercice 5 — La composition

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

Pose Composer(Pose parent, Pose enfant) {
    Vecteur translationEnfant = Tourner(enfant.position, parent.rotation);

    return {
        {
            parent.position.x + translationEnfant.x,
            parent.position.y + translationEnfant.y,
            parent.position.z + translationEnfant.z
        },
        Multiplier(parent.rotation, enfant.rotation)
    };
}

Vecteur Appliquer(Pose pose, Vecteur point) {
    Vecteur resultat = Tourner(point, pose.rotation);
    return {
        resultat.x + pose.position.x,
        resultat.y + pose.position.y,
        resultat.z + pose.position.z
    };
}

double Distance(Vecteur a, Vecteur b) {
    double x = a.x - b.x;
    double y = a.y - b.y;
    double z = a.z - b.z;
    return sqrt(x*x + y*y + z*z);
}

int main() {
    Pose parent{{1, 0, 0}, {0.9238795, 0, 0.3826834, 0}};
    Pose enfant{{0, 1, 0}, {0.9659258, 0, 0.258819, 0}};
    Vecteur point{1, 0, 0};

    Pose composee = Composer(parent, enfant);

    Vecteur premier = Appliquer(composee, point);
    Vecteur second = Appliquer(parent, Appliquer(enfant, point));

    cout << fixed << setprecision(10);
    cout << premier.x << ' ' << premier.y << ' ' << premier.z << '\n';
    cout << second.x << ' ' << second.y << ' ' << second.z << '\n';
    cout << "Ecart : " << Distance(premier, second) << '\n';
}
```

La composition suit le principe parent puis enfant. Le résultat obtenu en appliquant la pose composée doit être identique, aux arrondis près, à l'application successive des deux poses.
