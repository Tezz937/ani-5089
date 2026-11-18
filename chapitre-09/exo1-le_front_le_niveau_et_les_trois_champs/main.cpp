// Exercice 1 : les trois champs d'un etat d'action (courant, change, actif), image par image.
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        n = 0;
    }

    int precedent = 0; // a la premiere image, on compare a 0
    int fronts = 0;
    int niveaux = 0;

    for (int image = 1; image <= n; ++image) {
        int presse = 0;
        int lie = 0;
        std::cin >> presse >> lie;

        const int actif = (lie != 0) ? 1 : 0;
        // Une action inactive rend toujours courant faux, quoi que dise la source.
        const int courant = (actif != 0 && presse != 0) ? 1 : 0;
        // change se calcule sur courant (l'etat), pas sur presse (la touche).
        const int change = (courant != precedent) ? 1 : 0;
        precedent = courant;

        if (courant == 1 && change == 1) {
            ++fronts; // front de saisie ; le relachement (courant 0, change 1) n'est pas compte
        }
        if (courant == 1) {
            ++niveaux; // niveau : tenu, quel que soit change
        }

        std::cout << image << ' ' << courant << ' ' << change << ' ' << actif << '\n';
    }

    std::cout << "FRONTS " << fronts << '\n';
    std::cout << "NIVEAUX " << niveaux << '\n';
    return 0;
}
