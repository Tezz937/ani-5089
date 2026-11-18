// Exercice 2 : meme geste, compte selon que le jeu teste le front ou le niveau.
#include <iostream>

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        n = 0;
    }

    int precedent = 0; // a la premiere image, on compare a 0
    int fronts = 0;
    int niveaux = 0;

    for (int image = 0; image < n; ++image) {
        int presse = 0;
        std::cin >> presse;
        const int courant = (presse != 0) ? 1 : 0;

        if (courant == 1 && precedent == 0) {
            ++fronts; // front de montee seulement : un relachement ne compte pas
        }
        if (courant == 1) {
            ++niveaux;
        }
        precedent = courant;
    }

    // Un jeu qui tire sur le front tire une balle par front ; sur le niveau, une par image tenue.
    // Un jeu qui tient sur le niveau tient a chaque image de niveau ; sur le front, a l'image du front seulement.
    std::cout << "BALLES FRONT " << fronts << '\n';
    std::cout << "BALLES NIVEAU " << niveaux << '\n';
    std::cout << "TENU NIVEAU " << niveaux << '\n';
    std::cout << "TENU FRONT " << fronts << '\n';
    return 0;
}
