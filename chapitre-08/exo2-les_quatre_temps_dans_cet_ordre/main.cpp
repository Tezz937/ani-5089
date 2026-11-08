#include <cstdio>
#include <iostream>
#include <string>

int main() {
	int n = 0;
	std::cin >> n;
	bool acquise = false, attente = false;
	int fautes = 0;
	for (int t = 1; t <= n; ++t) {
		std::string c;
		std::cin >> c;
		const char *raison = nullptr;
		if (c == "ACQUIERT") {
			if (acquise) raison = "DEJA ACQUISE"; else acquise = true;
		} else if (c == "DESSINE") {
			if (!acquise) raison = "PAS ACQUISE";
		} else if (c == "REND") {
			if (!acquise) raison = "RIEN ACQUIS"; else { acquise = false; attente = true; }
		} else if (c == "SOUMET") {
			if (!attente) raison = "PAS RENDUE"; else attente = false;
		}
		if (raison) ++fautes;
		std::printf("%d %s %s\n", t, c.c_str(), raison ? raison : "OK");
	}
	std::printf("FAUTES %d\n", fautes);
	return 0;
}
