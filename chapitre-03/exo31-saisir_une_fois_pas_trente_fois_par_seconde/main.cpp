#include <iostream>
#include <string>
using namespace std;

int main() {
    int n;
    cin >> n;

    int sans_garde = 0;
    string evenement;

    for (int i = 0; i < n; ++i) {
        cin >> evenement;

        if (evenement == "enfonce") {
            cout << "SAISIR\n";
            ++sans_garde;
        } else if (evenement == "repete") {
            cout << "RIEN\n";
            ++sans_garde;
        } else if (evenement == "relache") {
            cout << "LACHER\n";
        }
    }

    cout << "SANS_GARDE " << sans_garde << '\n';
    return 0;
}
