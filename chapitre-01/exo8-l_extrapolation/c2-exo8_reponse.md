# Exercice 8 — L'extrapolation

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

double Norme(Vecteur vecteur) {
    return sqrt(
        vecteur.x*vecteur.x +
        vecteur.y*vecteur.y +
        vecteur.z*vecteur.z
    );
}

Quaternion Multiplier(Quaternion a, Quaternion b) {
    return {
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w
    };
}

Pose Extrapoler(Pose pose, Vecteur vitesseLineaire,
                Vecteur vitesseAngulaire, double dt) {
    pose.position.x += vitesseLineaire.x * dt;
    pose.position.y += vitesseLineaire.y * dt;
    pose.position.z += vitesseLineaire.z * dt;

    double vitesse = Norme(vitesseAngulaire);

    if (vitesse > 0) {
        double angle = vitesse * dt;
        double sinus = sin(angle / 2.0);

        Quaternion delta{
            cos(angle / 2.0),
            vitesseAngulaire.x / vitesse * sinus,
            vitesseAngulaire.y / vitesse * sinus,
            vitesseAngulaire.z / vitesse * sinus
        };

        pose.rotation = Multiplier(pose.rotation, delta);
    }

    return pose;
}

int main() {
    Pose pose{{0, 1, 2}, {1, 0, 0, 0}};

    Vecteur vitesseLineaire{1, 0, 0};
    Vecteur vitesseAngulaire{0, 1, 0};

    Pose resultat = Extrapoler(pose, vitesseLineaire, vitesseAngulaire, 0.02);

    cout << fixed << setprecision(6);
    cout << resultat.position.x << ' '
         << resultat.position.y << ' '
         << resultat.position.z << '\n';

    cout << resultat.rotation.w << ' '
         << resultat.rotation.x << ' '
         << resultat.rotation.y << ' '
         << resultat.rotation.z << '\n';
}
```

Le cas d'une vitesse angulaire nulle est traité avant toute division par la norme. La vitesse angulaire est ici interprétée dans le repère local de l'orientation, d'où la composition `rotation * delta`.
