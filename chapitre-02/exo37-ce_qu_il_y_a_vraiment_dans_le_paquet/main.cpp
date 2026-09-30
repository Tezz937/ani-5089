#include <iostream>
#include <string>

using namespace std;

int main() {
    string architecture;
    int nombre_fichiers;

    cin >> architecture;
    cin >> nombre_fichiers;

    long long total = 0;
    bool signe = false;
    bool abi = false;
    int inutile = 0;

    string chemin;
    long long taille;

    string prefixe_abi = "lib/" + architecture + "/";

    for (int i = 0; i < nombre_fichiers; ++i) {
        cin >> chemin >> taille;

        total += taille;

        if (chemin.rfind("META-INF/", 0) == 0) {
            if (chemin.size() >= 4) {
                string fin = chemin.substr(chemin.size() - 4);

                if (fin == ".RSA" || fin == ".DSA" || fin == ".EC") {
                    signe = true;
                }
            }
        }

        if (chemin.rfind(prefixe_abi, 0) == 0) {
            abi = true;
        } else if (chemin.rfind("lib/", 0) == 0) {
            inutile++;
        }
    }

    cout << total << '\n';
    cout << (signe ? "SIGNE" : "NON SIGNE") << '\n';
    cout << (abi ? "ABI OUI" : "ABI NON") << '\n';
    cout << "INUTILE " << inutile << '\n';

    return 0;
}
