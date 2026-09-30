#include <iostream>
#include <string>

using namespace std;

int main() {
    long long budget;
    int nombre_scenes;

    cin >> budget >> nombre_scenes;

    int trompe = 0;

    for (int i = 0; i < nombre_scenes; ++i) {
        string nom;
        long long debug;
        long long release;

        cin >> nom >> debug >> release;

        long long facteur = (debug + release / 2) / release;

        if (release <= budget) {
            cout << nom << ' ' << facteur << " TIENT\n";

            if (debug > budget) {
                trompe++;
            }
        } else {
            cout << nom << ' ' << facteur << " DEPASSE\n";
        }
    }

    cout << "TROMPE " << trompe << '\n';

    return 0;
}
