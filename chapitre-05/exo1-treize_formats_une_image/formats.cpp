// Exercice 1 : une photo, cinq formats ecrits par le moteur, puis relus.
// Usage : formats [photo.jpg]   (par defaut "photo.jpg", dossier courant)
// Le programme ecrit formats.txt : une ligne par format, issue d'une RELECTURE.

#include "NKImage/Core/NkImage.h"

#include <cstdio>

using namespace nkentseu;

// Taille d'un fichier sur le disque, en octets (0 si absent).
static long TailleFichier(const char *chemin) {
	FILE *f = fopen(chemin, "rb");
	if (!f)
		return 0;
	fseek(f, 0, SEEK_END);
	const long n = ftell(f);
	fclose(f);
	return n;
}

struct Format {
	const char *nom;
	const char *fichier;
};

int main(int argc, char **argv) {
	const char *chemin = (argc > 1) ? argv[1] : "photo.jpg";

	NkImage photo;
	if (!photo.Load(chemin)) {
		printf("Load a echoue : %s\n", chemin);
		return 1;
	}
	printf("photo : %d x %d, %d canaux, %ld octets sur disque\n",
		   photo.Width(), photo.Height(), photo.Channels(), TailleFichier(chemin));

	// Les cinq formats choisis : un sans perte classique, un avec perte,
	// deux sans perte simples, un sans perte moderne.
	const Format formats[5] = {
		{"png", "sortie.png"}, {"jpeg", "sortie.jpg"}, {"bmp", "sortie.bmp"},
		{"tga", "sortie.tga"}, {"qoi", "sortie.qoi"},
	};

	const bool ecrit[5] = {
		photo.SavePNG(formats[0].fichier),
		photo.SaveJPEG(formats[1].fichier, 90),
		photo.SaveBMP(formats[2].fichier),
		photo.SaveTGA(formats[3].fichier),
		photo.SaveQOI(formats[4].fichier),
	};

	FILE *out = fopen("formats.txt", "w");
	if (!out) {
		printf("impossible d'ecrire formats.txt\n");
		return 1;
	}

	for (int i = 0; i < 5; ++i) {
		// Relecture dans un NOUVEAU NkImage : c'est elle qui fait foi.
		NkImage relue;
		const long octets = TailleFichier(formats[i].fichier);
		const bool ok = ecrit[i] && relue.Load(formats[i].fichier) && relue.IsValid();

		if (ok) {
			fprintf(out, "%s : %ld octets, %d x %d\n", formats[i].nom, octets, relue.Width(), relue.Height());
			const bool memeTaille = relue.Width() == photo.Width() && relue.Height() == photo.Height();
			printf("%-5s : %ld octets, %d x %d  (dimensions %s)\n", formats[i].nom, octets,
				   relue.Width(), relue.Height(), memeTaille ? "identiques a la photo" : "DIFFERENTES");
		} else {
			// Pas de remplacement silencieux : un format illisible est un resultat.
			fprintf(out, "%s : %ld octets, illisible\n", formats[i].nom, octets);
			printf("%-5s : %ld octets, illisible (ecriture=%d)\n", formats[i].nom, octets, ecrit[i] ? 1 : 0);
		}
	}
	fclose(out);

	// Cout memoire une fois chargee : pixels bruts, quel que soit le format.
	printf("memoire apres chargement : %d octets (largeur x hauteur x canaux)\n",
		   photo.Width() * photo.Height() * photo.Channels());
	return 0;
}
