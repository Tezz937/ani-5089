# Exercice 7 — La matrice de vue

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

struct Matrice {
    double m[4][4];
};

Matrice Rotation(Quaternion q) {
    return {{
        {1-2*(q.y*q.y+q.z*q.z), 2*(q.x*q.y-q.z*q.w), 2*(q.x*q.z+q.y*q.w), 0},
        {2*(q.x*q.y+q.z*q.w), 1-2*(q.x*q.x+q.z*q.z), 2*(q.y*q.z-q.x*q.w), 0},
        {2*(q.x*q.z-q.y*q.w), 2*(q.y*q.z+q.x*q.w), 1-2*(q.x*q.x+q.y*q.y), 0},
        {0, 0, 0, 1}
    }};
}

Matrice VueDirecte(Vecteur position, Quaternion rotation) {
    Quaternion conjugue{rotation.w, -rotation.x, -rotation.y, -rotation.z};
    Matrice vue = Rotation(conjugue);

    vue.m[0][3] = -(vue.m[0][0]*position.x + vue.m[0][1]*position.y + vue.m[0][2]*position.z);
    vue.m[1][3] = -(vue.m[1][0]*position.x + vue.m[1][1]*position.y + vue.m[1][2]*position.z);
    vue.m[2][3] = -(vue.m[2][0]*position.x + vue.m[2][1]*position.y + vue.m[2][2]*position.z);

    return vue;
}

Matrice InverseGenerale(Matrice entree) {
    double a[4][8];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            a[i][j] = entree.m[i][j];
        }
        for (int j = 0; j < 4; ++j) {
            a[i][j+4] = (i == j);
        }
    }

    for (int colonne = 0; colonne < 4; ++colonne) {
        int pivot = colonne;

        for (int ligne = colonne + 1; ligne < 4; ++ligne) {
            if (fabs(a[ligne][colonne]) > fabs(a[pivot][colonne])) {
                pivot = ligne;
            }
        }

        if (fabs(a[pivot][colonne]) < 1e-12) {
            return {};
        }

        for (int j = 0; j < 8; ++j) {
            swap(a[colonne][j], a[pivot][j]);
        }

        double valeur = a[colonne][colonne];

        for (int j = 0; j < 8; ++j) {
            a[colonne][j] /= valeur;
        }

        for (int ligne = 0; ligne < 4; ++ligne) {
            if (ligne == colonne) continue;

            double facteur = a[ligne][colonne];

            for (int j = 0; j < 8; ++j) {
                a[ligne][j] -= facteur * a[colonne][j];
            }
        }
    }

    Matrice resultat{};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            resultat.m[i][j] = a[i][j+4];
        }
    }

    return resultat;
}

int main() {
    Vecteur position{2, 1, -3};
    Quaternion rotation{0.9238795, 0, 0.3826834, 0};

    Matrice pose = Rotation(rotation);
    pose.m[0][3] = position.x;
    pose.m[1][3] = position.y;
    pose.m[2][3] = position.z;

    Matrice generale = InverseGenerale(pose);
    Matrice directe = VueDirecte(position, rotation);

    double ecartMax = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ecartMax = max(ecartMax, fabs(generale.m[i][j] - directe.m[i][j]));
        }
    }

    cout << fixed << setprecision(10);
    cout << "Ecart maximal : " << ecartMax << '\n';

    Matrice degenerate{};
    Matrice resultatDegenerate = InverseGenerale(degenerate);

    cout << "Premiere valeur de l'inverse degeneree : "
         << resultatDegenerate.m[0][0] << '\n';
}
```

La première méthode réalise réellement une inversion générale par élimination de Gauss-Jordan. La seconde construit directement l'inverse analytique d'une pose rigide avec le conjugué du quaternion et la translation opposée tournée. Pour une pose valide, les seize coefficients doivent coïncider aux arrondis près.

Avec une matrice dégénérée, l'inversion générale ne possède pas d'inverse valide ; la fonction retourne ici une matrice nulle au lieu de prétendre qu'une inverse existe.
