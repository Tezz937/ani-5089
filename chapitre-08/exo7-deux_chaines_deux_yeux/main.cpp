#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

struct Oeil {
	int suivant = 0, courant = 0, rendus = 0;
	bool emprunte = false;
};

int main() {
	int images = 0, n = 0;
	std::cin >> images >> n;
	Oeil yeux[2];
	int desync = 0, diff = 0;
	for (int t = 1; t <= n; ++t) {
		std::string o, c;
		std::cin >> o >> c;
		Oeil &e = yeux[o == "G" ? 0 : 1];
		if (c == "EMPRUNTE") {
			if (!e.emprunte) {
				e.emprunte = true;
				e.courant = e.suivant;
				std::printf("%d %s EMPRUNTE %d\n", t, o.c_str(), e.courant);
			} else {
				std::printf("%d %s EMPRUNTE REFUSE\n", t, o.c_str());
			}
		} else {
			if (e.emprunte) {
				e.emprunte = false;
				++e.rendus;
				std::printf("%d %s REND %d\n", t, o.c_str(), e.courant);
				e.suivant = (e.suivant + 1) % images;
			} else {
				std::printf("%d %s REND REFUSE\n", t, o.c_str());
			}
		}
		diff = std::abs(yeux[0].rendus - yeux[1].rendus);
		if (diff > 1 && desync == 0) desync = t;
	}
	std::printf("DECALAGE %d\n", diff);
	std::printf("DESYNC %d\n", desync);
	return 0;
}
