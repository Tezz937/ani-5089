#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

int main() {
	long long images = 0;
	int n = 0;
	std::cin >> images >> n;
	const size_t taille = images > 0 ? (size_t)images : 0;
	std::vector<bool> posee(taille, false);
	std::vector<long long> valeur(taille, 0);
	int posees = 0;
	for (int i = 0; i < n; ++i) {
		std::string c;
		std::cin >> c;
		if (c == "POSE") {
			long long idx = 0, v = 0;
			std::cin >> idx >> v;
			if (idx < 0 || idx >= images) {
				std::printf("HORS ANNEAU\n");
			} else {
				if (!posee[(size_t)idx]) { posee[(size_t)idx] = true; ++posees; }
				valeur[(size_t)idx] = v;
			}
		} else {
			long long idx = 0;
			std::cin >> idx;
			if (idx >= 0 && idx < images && posee[(size_t)idx]) std::printf("%lld\n", valeur[(size_t)idx]);
			else std::printf("VIDE\n");
		}
	}
	std::printf("POSEES %d\n", posees);
	return 0;
}
