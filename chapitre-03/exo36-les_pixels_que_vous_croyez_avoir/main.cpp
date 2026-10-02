#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int lisible = 0;

    for (int i = 0; i < n; ++i) {
        long long largeur, hauteur, echelle, champ;
        cin >> largeur >> hauteur >> echelle >> champ;

        long long largeur_reelle = largeur * echelle / 100;
        long long hauteur_reelle = hauteur * echelle / 100;
        long long pixels_par_degre = (largeur_reelle + champ / 2) / champ;

        cout << largeur_reelle << ' ' << hauteur_reelle << ' ' << pixels_par_degre << '\n';
        if (pixels_par_degre >= 15) ++lisible;
    }

    cout << "LISIBLE " << lisible << '\n';
    return 0;
}
