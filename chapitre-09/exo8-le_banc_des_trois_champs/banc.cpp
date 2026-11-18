// banc.cpp : le banc des trois champs. Ni casque ni manette : des touches simulees, six images.
//
// Deux actions liees a deux touches, et une troisieme liee a rien (touche = -1) :
//   saisir -> touche 0, tenue de la 2e a la 4e image
//   tirer  -> touche 1, un appui d'une seule image (la 4e)
//   morte  -> aucune touche : on voit qu'elle reste a courant 0 et actif 0
#include "actions.h"

#include <cstdio>

int main() {
    const int kImages = 6;
    const int kTouches = 2;

    // touches[image][touche] : 1 = enfoncee a cette image.
    const bool touches[kImages][kTouches] = {
        {false, false}, // image 1
        {true, false},  // image 2 : debut de la saisie
        {true, false},  // image 3
        {true, true},   // image 4 : saisie tenue, et appui sur tirer
        {false, false}, // image 5 : relachement de saisir et de tirer
        {false, false}, // image 6
    };

    Action actions[3] = {
        {"saisir", 0, {}},
        {"tirer", 1, {}},
        {"morte", -1, {}}, // liee a rien
    };

    std::printf("image | saisir       | tirer        | morte\n");
    std::printf("      | cour chg act | cour chg act | cour chg act\n");

    for (int image = 0; image < kImages; ++image) {
        std::printf("%5d", image + 1);
        for (Action &a : actions) {
            MettreAJour(a, touches[image], kTouches);
            std::printf(" |  %d    %d    %d", a.etat.courant ? 1 : 0, a.etat.change ? 1 : 0, a.etat.actif ? 1 : 0);
        }
        std::printf("\n");
    }
    return 0;
}
