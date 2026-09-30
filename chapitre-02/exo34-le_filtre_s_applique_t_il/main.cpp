#include <iostream>
#include <string>
#include <map>
#include <sstream>

using namespace std;

int main() {
    int nombre_valeurs;
    cin >> nombre_valeurs;
    cin.ignore();

    map<string, string> machine;

    for (int i = 0; i < nombre_valeurs; ++i) {
        string ligne;
        getline(cin, ligne);

        size_t egal = ligne.find('=');
        if (egal != string::npos) {
            machine[ligne.substr(0, egal)] = ligne.substr(egal + 1);
        }
    }

    int nombre_filtres;
    cin >> nombre_filtres;
    cin.ignore();

    for (int i = 0; i < nombre_filtres; ++i) {
        string ligne;
        getline(cin, ligne);

        stringstream flux(ligne);
        string morceau;
        bool ok = true;

        while (flux >> morceau) {
            if (morceau == "&&") {
                continue;
            }

            bool inverse = false;

            if (!morceau.empty() && morceau[0] == '!') {
                inverse = true;
                morceau = morceau.substr(1);
            }

            size_t egal = morceau.find('=');

            if (egal == string::npos) {
                ok = false;
                continue;
            }

            string cle = morceau.substr(0, egal);
            string valeur = morceau.substr(egal + 1);

            auto it = machine.find(cle);
            bool vrai = it != machine.end() && it->second == valeur;

            if (inverse) {
                vrai = !vrai;
            }

            if (!vrai) {
                ok = false;
            }
        }

        cout << (ok ? "OUI" : "NON") << '\n';
    }

    return 0;
}
