// Exercice 5 : quatre filtres de reduction, 2048 -> 256, chronometres et compares.
// Usage : filtres [photo.jpg]   -- A COMPILER ET LANCER EN RELEASE.
// Ecrit mesures.txt et les quatre PNG.

#include "NKImage/Core/NkImage.h"
#include "NKTime/NkChrono.h"

#include <cstdio>

using namespace nkentseu;

static const int kRepetitions = 20;

struct Filtre {
	const char *nom;
	NkResizeFilter filtre;
	const char *fichier;
};

// Nombre de pixels qui different de la reference sur au moins un canal.
static long Differences(const NkImage &a, const NkImage &ref) {
	long n = 0;
	for (int y = 0; y < ref.Height(); ++y) {
		for (int x = 0; x < ref.Width(); ++x) {
			const math::NkColor p = a.GetPixel(x, y);
			const math::NkColor q = ref.GetPixel(x, y);
			if (p.r != q.r || p.g != q.g || p.b != q.b || p.a != q.a)
				++n;
		}
	}
	return n;
}

int main(int argc, char **argv) {
	const char *chemin = (argc > 1) ? argv[1] : "photo.jpg";

	NkImage photo;
	if (!photo.Load(chemin)) {
		printf("Load a echoue : %s\n", chemin);
		return 1;
	}

	// Source : 2048 x 2048 (bilineaire), gardee pour toutes les reductions.
	NkImage source = photo.Resize(2048, 2048, NkResizeFilter::NK_BILINEAR);
	if (!source.IsValid()) {
		printf("Resize 2048 a echoue\n");
		return 1;
	}
	printf("source : %d x %d\n", source.Width(), source.Height());

	const Filtre filtres[4] = {
		{"NK_NEAREST", NkResizeFilter::NK_NEAREST, "nearest.png"},
		{"NK_BILINEAR", NkResizeFilter::NK_BILINEAR, "bilinear.png"},
		{"NK_BICUBIC", NkResizeFilter::NK_BICUBIC, "bicubic.png"},
		{"NK_LANCZOS3", NkResizeFilter::NK_LANCZOS3, "lanczos3.png"},
	};

	NkImage resultats[4];
	double moyennes[4] = {};

	for (int i = 0; i < 4; ++i) {
		// Premiere reduction a blanc : elle paie des couts que les suivantes ne paient pas.
		resultats[i] = source.Resize(256, 256, filtres[i].filtre);

		double total = 0.0;
		for (int r = 0; r < kRepetitions; ++r) {
			NkChrono chrono;
			NkImage tmp = source.Resize(256, 256, filtres[i].filtre);
			total += chrono.Elapsed().ToMicroseconds();
			if (!tmp.IsValid()) {
				printf("Resize %s a echoue\n", filtres[i].nom);
				return 1;
			}
		}
		moyennes[i] = total / kRepetitions;

		if (!resultats[i].IsValid() || !resultats[i].SavePNG(filtres[i].fichier)) {
			printf("sauvegarde %s a echoue\n", filtres[i].fichier);
			return 1;
		}
		printf("%s : %d x %d\n", filtres[i].nom, resultats[i].Width(), resultats[i].Height());
	}

	// Comparaison pixel par pixel au bilineaire (indice 1).
	FILE *out = fopen("mesures.txt", "w");
	if (!out) {
		printf("impossible d'ecrire mesures.txt\n");
		return 1;
	}
	for (int i = 0; i < 4; ++i) {
		const long d = Differences(resultats[i], resultats[1]);
		fprintf(out, "%s : %.0f us, differences : %ld\n", filtres[i].nom, moyennes[i], d);
		printf("%s : %.0f us, differences : %ld\n", filtres[i].nom, moyennes[i], d);
	}
	fclose(out);
	return 0;
}
