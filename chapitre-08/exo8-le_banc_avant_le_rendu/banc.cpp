#include "chaine.h"

#include <cstdio>

int main() {
	ChaineEchange chaine;
	if (!chaine.Creer(3)) {
		std::printf("creation impossible\n");
		return 1;
	}

	// Six tours sur un anneau de trois : l'index revient a 0 au quatrieme.
	for (int tour = 1; tour <= 6; ++tour) {
		uint32_t index = 0;
		const bool emprunte = chaine.Acquerir(index);
		const bool rendu = chaine.Rendre();
		std::printf("tour %d : emprunte %u (%s), rendu %u (%s)\n", tour, index, emprunte ? "ok" : "REFUSE",
					chaine.DernierRendu(), rendu ? "ok" : "REFUSE");
	}

	// Les trois refus.
	uint32_t index = 0;
	chaine.Acquerir(index);
	const bool second = chaine.Acquerir(index);
	std::printf("double emprunt : %s\n", second ? "accepte" : "REFUSE");
	chaine.Rendre();

	const bool sansEmprunt = chaine.Rendre();
	std::printf("rendre sans avoir emprunte : %s\n", sansEmprunt ? "accepte" : "REFUSE");

	ChaineEchange trop;
	const bool grande = trop.Creer(ChaineEchange::kMax + 1);
	std::printf("chaine de %u images (maximum %u) : %s\n", ChaineEchange::kMax + 1, ChaineEchange::kMax,
				grande ? "acceptee" : "REFUSE");
	return 0;
}
