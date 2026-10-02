#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Rappel {
    int id;
    string type;
};

int main() {
    int n;
    cin >> n;

    vector<Rappel> registre;
    string commande;

    for (int i = 0; i < n; ++i) {
        cin >> commande;

        if (commande == "poser") {
            int id;
            string type;
            cin >> id >> type;

            for (auto it = registre.begin(); it != registre.end(); ++it) {
                if (it->id == id) {
                    registre.erase(it);
                    break;
                }
            }
            registre.push_back({id, type});
        } else if (commande == "retirer") {
            int id;
            cin >> id;

            for (auto it = registre.begin(); it != registre.end(); ++it) {
                if (it->id == id) {
                    registre.erase(it);
                    break;
                }
            }
        } else if (commande == "envoyer") {
            string type;
            cin >> type;
            bool trouve = false;

            for (const auto& rappel : registre) {
                if (rappel.type == type) {
                    if (trouve) cout << ' ';
                    cout << rappel.id;
                    trouve = true;
                }
            }

            if (!trouve) cout << "AUCUN";
            cout << '\n';
        }
    }

    return 0;
}
