#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    int n;
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; ++i) {
        string ligne;
        getline(cin, ligne);
        stringstream flux(ligne);
        string touche;
        bool avant = false, arriere = false, gauche = false, droite = false;

        while (flux >> touche) {
            if (touche == "W" || touche == "Z") avant = true;
            else if (touche == "S") arriere = true;
            else if (touche == "A" || touche == "Q") gauche = true;
            else if (touche == "D") droite = true;
        }

        int avance = (avant ? 1 : 0) - (arriere ? 1 : 0);
        int cote = (droite ? 1 : 0) - (gauche ? 1 : 0);
        cout << avance << ' ' << cote << '\n';
    }

    return 0;
}
