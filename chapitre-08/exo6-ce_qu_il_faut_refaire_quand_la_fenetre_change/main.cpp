#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

int main() {
	int n = 0;
	std::cin >> n;
	std::string ligne;
	std::getline(std::cin, ligne); // fin de la ligne de N
	int survivants = 0;
	for (int i = 0; i < n;) {
		if (!std::getline(std::cin, ligne)) break;
		std::istringstream in(ligne);
		std::string nom;
		if (!(in >> nom)) continue; // ligne vide
		++i;
		int posDetruit = -1, posRecree = -1, k = 0;
		bool reenregistre = false;
		std::string mot;
		while (in >> mot) {
			if (mot == "DETRUIT" && posDetruit < 0) posDetruit = k;
			else if (mot == "RECREE" && posRecree < 0) posRecree = k;
			else if (mot == "REENREGISTRE") reenregistre = true;
			++k;
		}
		const char *verdict;
		if (posDetruit >= 0 && posRecree >= 0 && posRecree < posDetruit) verdict = "MAUVAIS ORDRE";
		else if (posDetruit < 0 || posRecree < 0) verdict = "CHAINE PERIMEE";
		else if (!reenregistre) verdict = "POIGNEES PERDUES";
		else { verdict = "SURVIT"; ++survivants; }
		std::printf("%s %s\n", nom.c_str(), verdict);
	}
	std::printf("SURVIVANTS %d\n", survivants);
	return 0;
}
