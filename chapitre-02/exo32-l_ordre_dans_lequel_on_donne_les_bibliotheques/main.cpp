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

    set<string> modules;
    vector<string> a_traiter;

    for (int i = 0; i < nombre_directs; ++i) {
        string module;
        cin >> module;

        if (modules.insert(module).second) {
            a_traiter.push_back(module);
        }
    }

    for (size_t i = 0; i < a_traiter.size(); ++i) {
        string module = a_traiter[i];

        auto it = dependances.find(module);

        if (it == dependances.end()) {
            continue;
        }

        for (const string& besoin : it->second) {
            if (modules.insert(besoin).second) {
                a_traiter.push_back(besoin);
            }
        }
    }

    map<string, int> compteur;

    for (const string& module : modules) {
        compteur[module] = 0;
    }

    for (const string& module : modules) {
        auto it = dependances.find(module);

        if (it == dependances.end()) {
            continue;
        }

        set<string> besoins_uniques(it->second.begin(), it->second.end());

        for (const string& besoin : besoins_uniques) {
            if (modules.count(besoin)) {
                compteur[besoin]++;
            }
        }
    }

    set<string> disponibles;

    for (const auto& element : compteur) {
        if (element.second == 0) {
            disponibles.insert(element.first);
        }
    }

    vector<string> ordre;

    while (!disponibles.empty()) {
        auto it = disponibles.begin();
        string module = *it;
        disponibles.erase(it);

        ordre.push_back(module);

        auto dependance = dependances.find(module);

        if (dependance == dependances.end()) {
            continue;
        }

        set<string> besoins_uniques(
            dependance->second.begin(),
            dependance->second.end()
        );

        for (const string& besoin : besoins_uniques) {
            compteur[besoin]--;

            if (compteur[besoin] == 0) {
                disponibles.insert(besoin);
            }
        }
    }

    if (ordre.size() != modules.size()) {
        cout << "CYCLE\n";
        return 0;
    }

    for (const string& module : ordre) {
        cout << module << '\n';
    }

    return 0;
}
