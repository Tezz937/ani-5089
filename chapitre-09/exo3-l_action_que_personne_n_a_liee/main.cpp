// Exercice 3 : reperer les actions declarees mais jamais liees a un organe.
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main() {
    int nbActions = 0;
    if (!(std::cin >> nbActions)) {
        nbActions = 0;
    }

    std::vector<std::string> noms;
    std::vector<std::string> usages;
    for (int i = 0; i < nbActions; ++i) {
        std::string nom;
        std::string usage;
        std::cin >> nom >> usage;
        noms.push_back(nom);
        usages.push_back(usage);
    }

    int nbFournis = 0;
    if (!(std::cin >> nbFournis)) {
        nbFournis = 0; // aucun organe branche
    }
    std::set<std::string> fournis;
    for (int i = 0; i < nbFournis; ++i) {
        std::string usage;
        std::cin >> usage;
        fournis.insert(usage);
    }

    int mortes = 0;
    for (std::size_t i = 0; i < noms.size(); ++i) {
        // Active si l'usage reclame est fourni. Deux actions sur le meme usage sont toutes deux actives.
        const bool active = fournis.count(usages[i]) > 0;
        if (!active) {
            ++mortes;
        }
        std::cout << noms[i] << (active ? " ACTIVE" : " MORTE") << '\n';
    }

    std::cout << "MORTES " << mortes << '\n';
    return 0;
}
