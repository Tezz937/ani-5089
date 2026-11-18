// Exercice 7 : ranger les actions par type, reperer les operations qui vont dans le mauvais sens.
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    int nbActions = 0;
    if (!(std::cin >> nbActions)) {
        nbActions = 0;
    }

    // true : sortie (HAPTIC, la seule) ; false : entree (BOOL, FLOAT, VEC2, POSE).
    std::map<std::string, bool> estSortie;
    int entrees = 0;
    int sorties = 0;
    for (int i = 0; i < nbActions; ++i) {
        std::string nom;
        std::string type;
        std::cin >> nom >> type;
        const bool sortie = (type == "HAPTIC");
        estSortie[nom] = sortie;
        if (sortie) {
            ++sorties;
        } else {
            ++entrees; // POSE est une entree, meme si elle se lit par un appel a part
        }
    }

    int nbOperations = 0;
    if (!(std::cin >> nbOperations)) {
        nbOperations = 0;
    }
    for (int i = 0; i < nbOperations; ++i) {
        std::string nom;
        std::string operation;
        std::cin >> nom >> operation;

        const auto trouve = estSortie.find(nom);
        if (trouve == estSortie.end()) {
            continue; // action non declaree : ignoree
        }
        if (operation == "LIT" && trouve->second) {
            std::cout << nom << " LIT UNE SORTIE\n";
        } else if (operation == "ECRIT" && !trouve->second) {
            std::cout << nom << " ECRIT UNE ENTREE\n";
        } // les operations justes ne s'affichent pas
    }

    std::cout << "ENTREES " << entrees << '\n';
    std::cout << "SORTIES " << sorties << '\n';
    return 0;
}
