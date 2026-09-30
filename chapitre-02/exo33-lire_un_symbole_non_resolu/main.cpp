#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

struct Correspondance {
    string prefixe;
    string module;
};

int main() {
    int nombre_prefixes;
    cin >> nombre_prefixes;

    vector<Correspondance> correspondances;

    for (int i = 0; i < nombre_prefixes; ++i) {
        Correspondance correspondance;
        cin >> correspondance.prefixe >> correspondance.module;
        correspondances.push_back(correspondance);
    }

    int nombre_lignes;
    cin >> nombre_lignes;
    cin.ignore();

    set<string> modules;
    int nombre_inconnus = 0;

    for (int i = 0; i < nombre_lignes; ++i) {
        string ligne;
        getline(cin, ligne);

        string marqueur = "undefined reference to '";
        size_t debut = ligne.find(marqueur);

        if (debut == string::npos) {
            continue;
        }

        debut += marqueur.size();

        size_t fin = ligne.find('\'', debut);

        if (fin == string::npos) {
            continue;
        }

        string symbole = ligne.substr(debut, fin - debut);

        string meilleur_module;
        size_t longueur_maximale = 0;

        for (const Correspondance& correspondance : correspondances) {
            if (symbole.rfind(correspondance.prefixe, 0) == 0) {
                if (correspondance.prefixe.size() > longueur_maximale) {
                    longueur_maximale = correspondance.prefixe.size();
                    meilleur_module = correspondance.module;
                }
            }
        }

        if (longueur_maximale == 0) {
            nombre_inconnus++;
        } else {
            modules.insert(meilleur_module);
        }
    }

    for (const string& module : modules) {
        cout << module << '\n';
    }

    if (nombre_inconnus > 0) {
        cout << "INCONNU " << nombre_inconnus << '\n';
    }

    return 0;
}
