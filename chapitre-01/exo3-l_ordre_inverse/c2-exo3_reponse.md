# Exercice 3 — L'ordre inverse

```cpp
#include <iostream>
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

Vecteur TournerPuisTranslater(Pose pose, Vecteur point) {
    Vecteur resultat = Tourner(point, pose.rotation);
    return {
        resultat.x + pose.position.x,
        resultat.y + pose.position.y,
        resultat.z + pose.position.z
    };
}

Vecteur TranslaterPuisTourner(Pose pose, Vecteur point) {
    point.x += pose.position.x;
    point.y += pose.position.y;
    point.z += pose.position.z;
    return Tourner(point, pose.rotation);
}

int main() {
    Pose pose{{2, 0, 0}, {0.9238795, 0, 0.3826834, 0}};
    Vecteur point{1, 0, 0};

    Vecteur premier = TournerPuisTranslater(pose, point);
    Vecteur second = TranslaterPuisTourner(pose, point);

    cout << fixed << setprecision(4);
    cout << premier.x << ' ' << premier.y << ' ' << premier.z << '\n';
    cout << second.x << ' ' << second.y << ' ' << second.z << '\n';

    Pose sansTranslation{{0, 0, 0}, pose.rotation};
    Vecteur troisieme = TournerPuisTranslater(sansTranslation, point);
    Vecteur quatrieme = TranslaterPuisTourner(sansTranslation, point);

    cout << troisieme.x << ' ' << troisieme.y << ' ' << troisieme.z << '\n';
    cout << quatrieme.x << ' ' << quatrieme.y << ' ' << quatrieme.z << '\n';
}
```

Avec la pose choisie, les deux résultats principaux sont différents : environ `(2.7071, 0, -0.7071)` puis `(2.1213, 0, -2.1213)`.

Ils coïncident lorsque la translation est nulle, car les deux fonctions ne font alors plus qu'appliquer la même rotation.
