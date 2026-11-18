// Exercice 6 : chaque objet est-il attache a la bonne pose de main ?
#include <iostream>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        n = 0;
    }

    int fautes = 0;
    for (int i = 0; i < n; ++i) {
        std::string nom;
        std::string intention;
        std::string pose;
        std::cin >> nom >> intention >> pose;

        std::string verdict = "OK";
        if (intention == "TENIR" && pose == "AIM") {
            verdict = "FLOTTE"; // objet tenu attache a la visee : il flotte a quelques centimetres de la main
            ++fautes;
        } else if (intention == "POINTER" && pose == "GRIP") {
            verdict = "PART DE TRAVERS"; // rayon parti de la paume : mal oriente
            ++fautes;
        }
        std::cout << nom << ' ' << verdict << '\n';
    }

    std::cout << "FAUTES " << fautes << '\n';
    return 0;
}
