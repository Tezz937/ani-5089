#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int nombre_appareils;
    cin >> nombre_appareils;

    vector<string> series;
    vector<string> etats;

    for (int i = 0; i < nombre_appareils; ++i) {
        string serie;
        string etat;
        string modele;

        cin >> serie >> etat >> modele;

        series.push_back(serie);
        etats.push_back(etat);
    }

    string cible;
    cin >> cible;

    if (cible != "-") {
        for (int i = 0; i < nombre_appareils; ++i) {
            if (series[i] == cible) {
                if (etats[i] == "device") {
                    cout << series[i] << '\n';
                } else {
                    cout << "ERREUR " << series[i] << " est " << etats[i] << '\n';
                }
                return 0;
            }
        }

        cout << "ERREUR cible introuvable\n";
        return 0;
    }

    vector<string> prets;

    for (int i = 0; i < nombre_appareils; ++i) {
        if (etats[i] == "device") {
            prets.push_back(series[i]);
        }
    }

    sort(prets.begin(), prets.end());

    if (prets.empty()) {
        cout << "ERREUR aucun appareil\n";
    } else if (prets.size() == 1) {
        cout << prets[0] << '\n';
    } else {
        cout << "ERREUR plusieurs appareils\n";

        for (const string& serie : prets) {
            cout << serie << '\n';
        }
    }

    return 0;
}
