// Exercice 10 : deux chaines d'echange, une par oeil, qui survivent a un redimensionnement.
// Pas de fenetre ici : le redimensionnement est un evenement simule, les cibles hors ecran
// sont representees par leurs poignees dans un ensemble de poignees "vivantes".
#include <cstdint>
#include <cstdio>
#include <set>
#include <string>

static const uint32_t kMax = 4;

struct Chaine {
	std::string nom; // sert aux outils de mise au point : "oeil gauche", "oeil droit"
	uint32_t nombre = 0, suivant = 0;
	bool emprunte = false, cree = false;
	uint64_t poignees[kMax] = { 0, 0, 0, 0 };

	bool Creer(const char *n, uint32_t nb) {
		if (nb == 0 || nb > kMax) return false;
		nom = n; nombre = nb; suivant = 0; emprunte = false; cree = true;
		for (uint32_t i = 0; i < kMax; ++i) poignees[i] = 0; // chaine neuve : tableau plein de zeros
		return true;
	}
	void Detruire() {
		cree = false; nombre = 0; emprunte = false;
		for (uint32_t i = 0; i < kMax; ++i) poignees[i] = 0;
	}
	bool Acquerir(uint32_t &index) {
		if (!cree || emprunte) return false;
		emprunte = true;
		index = suivant;
		return true;
	}
	bool Rendre() {
		if (!cree || !emprunte) return false;
		emprunte = false;
		suivant = (suivant + 1) % nombre; // avance au rendu
		return true;
	}
	void Enregistrer(uint32_t index, uint64_t poignee) {
		if (index < nombre) poignees[index] = poignee;
	}
	// La derniere image relachee : on recule d'un cran.
	uint64_t Soumettre() const {
		if (!cree || nombre == 0) return 0;
		return poignees[(suivant + nombre - 1u) % nombre];
	}
};

// Une poignee de cible : oeil (0 gauche, 1 droit), generation (change au redimensionnement), image.
static uint64_t Poignee(int oeil, int generation, uint32_t image) {
	return (oeil ? 0x200000ull : 0x100000ull) + (uint64_t)generation * 0x100ull + image;
}

// Les quatre temps pour un oeil : acquerir, dessiner (dans cette image seulement), rendre, soumettre.
// Vrai si tout passe et si la poignee soumise designe une cible qui existe encore.
static bool Image(Chaine &c, const std::set<uint64_t> &vivantes, uint32_t &index) {
	if (!c.Acquerir(index)) return false;
	if (!c.Rendre()) return false;
	const uint64_t h = c.Soumettre();
	return h != 0 && vivantes.count(h) > 0;
}

int main() {
	std::set<uint64_t> vivantes;
	Chaine gauche, droit;
	gauche.Creer("oeil gauche", 3);
	droit.Creer("oeil droit", 3);
	for (uint32_t i = 0; i < 3; ++i) {
		gauche.Enregistrer(i, Poignee(0, 1, i));
		droit.Enregistrer(i, Poignee(1, 1, i));
		vivantes.insert(Poignee(0, 1, i));
		vivantes.insert(Poignee(1, 1, i));
	}

	int invalides = 0;
	for (int n = 1; n <= 30; ++n) {
		uint32_t ig = 0, id = 0;
		const bool a = Image(gauche, vivantes, ig);
		const bool b = Image(droit, vivantes, id);
		if (!a || !b) ++invalides;
		std::printf("image : %d, gauche : %u, droit : %u\n", n, ig, id);
	}
	std::printf("controle : %d soumission(s) invalide(s) avant le redimensionnement\n", invalides);

	// La fenetre passe de 1280x720 a 1920x1080 : les anciennes cibles n'existent plus.
	vivantes.clear();
	for (uint32_t i = 0; i < 3; ++i) {
		vivantes.insert(Poignee(0, 2, i));
		vivantes.insert(Poignee(1, 2, i));
	}

	{ // erreur 1 : redimensionner les cibles sans refaire la chaine
		Chaine c = gauche;
		uint32_t i = 0;
		const bool ok = Image(c, vivantes, i);
		std::printf("controle : chaine non recreee, poignee soumise 0x%llx, designe une cible vivante : %s\n",
					(unsigned long long)c.Soumettre(), ok ? "oui" : "non");
	}
	{ // erreur 2 : recreer sans reenregistrer
		Chaine c = gauche;
		c.Detruire();
		c.Creer("oeil gauche", 3);
		uint32_t i = 0;
		const bool ok = Image(c, vivantes, i);
		std::printf("controle : chaine recreee sans reenregistrer, poignee soumise 0x%llx, designe une cible vivante : %s\n",
					(unsigned long long)c.Soumettre(), ok ? "oui" : "non");
	}

	// Ce qu'on fait vraiment, dans l'ordre.
	std::printf("resize : detruit la chaine oeil gauche\n");
	gauche.Detruire();
	std::printf("resize : detruit la chaine oeil droit\n");
	droit.Detruire();
	std::printf("resize : cree les nouvelles cibles hors ecran en 1920x1080\n");
	std::printf("resize : recree la chaine oeil gauche (3 images)\n");
	gauche.Creer("oeil gauche", 3);
	std::printf("resize : recree la chaine oeil droit (3 images)\n");
	droit.Creer("oeil droit", 3);
	std::printf("resize : reenregistre les 3 images de l'oeil gauche\n");
	for (uint32_t i = 0; i < 3; ++i) gauche.Enregistrer(i, Poignee(0, 2, i));
	std::printf("resize : reenregistre les 3 images de l'oeil droit\n");
	for (uint32_t i = 0; i < 3; ++i) droit.Enregistrer(i, Poignee(1, 2, i));

	int invalidesApres = 0;
	for (int n = 31; n <= 36; ++n) {
		uint32_t ig = 0, id = 0;
		const bool a = Image(gauche, vivantes, ig);
		const bool b = Image(droit, vivantes, id);
		if (!a || !b) ++invalidesApres;
		std::printf("apres : image %d, gauche : %u, droit : %u\n", n, ig, id);
	}
	std::printf("controle : %d soumission(s) invalide(s) apres le redimensionnement\n", invalidesApres);
	return 0;
}
