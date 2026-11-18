// Exercice 4 : a quelle main appartient chaque usage, et quelles applications exigent deux mains.
#include <iostream>
#include <set>
#include <sstream>
#include <string>

namespace {

// MENU et MOVE sont a gauche sans suffixe (schema classique : la droite agit, la gauche deplace).
// MOVE_RIGHT corrige la convention : il est a droite. Tout usage non liste ici est inconnu et ignore.
const std::set<std::string> kGauche = {"MENU",           "MOVE",           "SELECT_LEFT",
                                       "GRAB_LEFT",      "AIM_POSE_LEFT",  "GRIP_POSE_LEFT",
                                       "HAPTIC_LEFT"};
const std::set<std::string> kDroite = {"SELECT",    "GRAB",   "AIM_POSE",
                                       "GRIP_POSE", "HAPTIC", "MOVE_RIGHT"};

void EnleverRetourChariot(std::string &ligne) {
    while (!ligne.empty() && (ligne.back() == '\r' || ligne.back() == '\n')) {
        ligne.pop_back();
    }
}

} // namespace

int main() {
    std::string ligne;
    int n = 0;
    if (!std::getline(std::cin, ligne)) {
        return 0;
    }
    EnleverRetourChariot(ligne);
    std::istringstream premiere(ligne);
    if (!(premiere >> n)) {
        n = 0;
    }

    int deuxMains = 0;
    for (int i = 0; i < n; ++i) {
        if (!std::getline(std::cin, ligne)) {
            break;
        }
        EnleverRetourChariot(ligne);
        std::istringstream mots(ligne);
        std::string nom;
        mots >> nom;

        int droite = 0;
        int gauche = 0;
        std::string usage;
        while (mots >> usage) {
            if (kGauche.count(usage) > 0) {
                ++gauche;
            } else if (kDroite.count(usage) > 0) {
                ++droite;
            } // sinon : usage inconnu, ignore, ne compte dans aucune main
        }

        // Deux mains des qu'on emploie au moins un usage de chaque main ; zero et zero ne font pas deux mains.
        const bool deux = (droite > 0 && gauche > 0);
        if (deux) {
            ++deuxMains;
        }
        std::cout << nom << ' ' << droite << ' ' << gauche << (deux ? " DEUX MAINS" : " UNE MAIN") << '\n';
    }

    std::cout << "DEUX MAINS " << deuxMains << '\n';
    return 0;
}
