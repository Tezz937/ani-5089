#include <cstdio>
#include <iostream>
#include <string>

int main() {
	int vol = 0, n = 0;
	std::cin >> vol >> n;
	int enVol = 0, bloque = 0;
	for (int t = 1; t <= n; ++t) {
		std::string c;
		std::cin >> c;
		bool attend = false;
		if (c == "SOUMET") {
			if (enVol >= vol) attend = true; else ++enVol;
		} else if (c == "FINI") {
			if (enVol > 0) --enVol;
		}
		if (attend && bloque == 0) bloque = t; // le PREMIER blocage
		std::printf("%d %s %d %s\n", t, c.c_str(), enVol, attend ? "ATTEND" : "PASSE");
	}
	std::printf("BLOQUE %d\n", bloque);
	return 0;
}
