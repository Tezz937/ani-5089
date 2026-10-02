#include <iostream>
#include <string>
using namespace std;

int main() {
    int p, f;
    cin >> p >> f;

    int compteur = 0;
    int premier = 0;

    for (int tour = 1; tour <= f; ++tour) {
        string action;
        cin >> action;

        if (action == "releve") compteur = 0;
        else if (action == "travaille") ++compteur;

        if (compteur >= p) {
            cout << compteur << " MORTE\n";
            if (premier == 0) premier = tour;
        } else {
            cout << compteur << " VIVANTE\n";
        }
    }

    cout << "PREMIER " << premier << '\n';
    return 0;
}
