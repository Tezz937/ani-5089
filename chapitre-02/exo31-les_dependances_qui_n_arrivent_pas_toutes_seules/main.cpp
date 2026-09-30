#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using namespace std;

int main() {
    int nombre_modules;
    cin >> nombre_modules;
    cin.ignore();

    map<string, vector<string>> dependances;

    for (int i = 0; i < nombre_modules; ++i) {
        string ligne;
        getline(cin, ligne);

        stringstream flux(ligne);

        string module;
        flux >> module;

        string besoin;
        while (flux >> besoin) {
            dependances[module].push_back(besoin);
        }
    }

    int nombre_directs;
    cin >> nombre_directs;

    set<string> vus;
    vector<string> a_traiter;

    for (int i = 0; i < nombre_directs; ++i) {
        string module;
        cin >> module;

        if (vus.insert(module).second) {
            a_traiter.push_back(module);
        }
    }

    for (size_t i = 0; i < a_traiter.size(); ++i) {
        const string& module = a_traiter[i];

        auto it = dependances.find(module);

        if (it == dependances.end()) {
            continue;
        }

        for (const string& besoin : it->second) {
            if (vus.insert(besoin).second) {
                a_traiter.push_back(besoin);
            }
        }
    }

    for (const string& module : vus) {
        cout << module << '\n';
    }

    return 0;
}
