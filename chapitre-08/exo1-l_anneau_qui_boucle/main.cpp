#include <cstdio>
#include <iostream>
#include <string>

int main() {
	int images = 0, n = 0;
	std::cin >> images >> n;
	int suivant = 0, courant = 0, prets = 0;
	bool emprunte = false;
	for (int t = 1; t <= n; ++t) {
		std::string c;
		std::cin >> c;
		if (c == "EMPRUNTE") {
			if (!emprunte) {
				emprunte = true;
				courant = suivant;
				++prets;
				std::printf("%d EMPRUNTE %d\n", t, courant);
			} else {
				std::printf("%d EMPRUNTE REFUSE\n", t);
			}
		} else {
			if (emprunte) {
				emprunte = false;
				std::printf("%d REND %d\n", t, courant);
				suivant = (suivant + 1) % images; // l'anneau avance au rendu
			} else {
				std::printf("%d REND REFUSE\n", t);
			}
		}
	}
	std::printf("PRETS %d\n", prets);
	return 0;
}
