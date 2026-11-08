#include <cstdio>
#include <iostream>
#include <string>

int main() {
	int largeur = 0, hauteur = 0, n = 0;
	std::cin >> largeur >> hauteur >> n;
	int bloquantes = 0;
	for (int i = 0; i < n; ++i) {
		std::string nom;
		int cible = 0, taille = 0, lc = 0, hc = 0;
		std::cin >> nom >> cible >> taille >> lc >> hc;
		int w = largeur, h = hauteur;
		const char *verdict;
		if (cible == 0) {
			verdict = "PAS RELIEE";
		} else if (taille == 1) {
			w = lc; h = hc;
			verdict = "OK";
		} else if (lc != largeur || hc != hauteur) {
			verdict = "BLOQUE";
			++bloquantes;
		} else {
			verdict = "SANS EFFET";
		}
		std::printf("%s %d %d %s\n", nom.c_str(), w, h, verdict);
	}
	std::printf("BLOQUANTES %d\n", bloquantes);
	return 0;
}
