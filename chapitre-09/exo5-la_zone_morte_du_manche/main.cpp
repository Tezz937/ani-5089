// Exercice 5 : de la valeur brute d'un manche a une vitesse, avec zone morte (tout en entiers, en milliemes).
#include <iostream>

int main() {
    long long seuil = 0;
    long long maximum = 0;
    if (!(std::cin >> seuil >> maximum)) {
        return 0;
    }
    int n = 0;
    if (!(std::cin >> n)) {
        n = 0;
    }

    const long long course = 1000 - seuil; // la course utile va du seuil a 1000
    int morts = 0;

    for (int i = 0; i < n; ++i) {
        long long brut = 0;
        std::cin >> brut;
        const long long absolu = (brut < 0) ? -brut : brut;

        long long vitesse = 0;
        if (absolu < seuil) {
            ++morts; // strictement inferieur au seuil : zone morte
        } else if (course > 0) {
            const long long reste = absolu - seuil; // on retire le seuil AVANT de mettre au carre
            // On multiplie avant de diviser (divisions entieres) ; long long evite tout debordement.
            vitesse = maximum * reste * reste / (course * course);
            if (brut < 0) {
                vitesse = -vitesse; // la vitesse garde le signe du manche
            }
        }
        std::cout << vitesse << '\n';
    }

    std::cout << "MORTS " << morts << '\n';
    return 0;
}
