#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Commande {
    string nom;
    int echelle;
    int seuil;
};

int main() {
    int c;
    cin >> c;

    vector<Commande> commandes(c);
    for (int i = 0; i < c; ++i) {
        cin >> commandes[i].nom >> commandes[i].echelle >> commandes[i].seuil;
    }

    int t;
    cin >> t;

    for (int tour = 0; tour < t; ++tour) {
        int axe = 0;
        for (int i = 0; i < c; ++i) {
            int valeur;
            cin >> valeur;
            int contribution = valeur * commandes[i].echelle / 1000;
            if (abs(contribution) >= commandes[i].seuil) {
                axe += contribution;
            }
        }
        cout << axe << '\n';
    }

    return 0;
}
