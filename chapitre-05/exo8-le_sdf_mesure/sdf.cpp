// Exercice 8 : le panneau « Sortie » en atlas ordinaire et en atlas SDF, a taille d'image egale.
// Usage : sdf [s]     s = douceur du bord SDF (defaut 0.08). Essayez 0.02, 0.05, 0.08, 0.12...
// Ecrit atlas.txt (3 tailles), ordinaire.png et sdf.png (a poser dans la salle).

#include "NKFont/NkFont.h"
#include "NKFont/Embedded/NkFontEmbedded.h"
#include "NKImage/Core/NkImage.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;

static const int kEchelle = 8; // image 8 fois plus grande que les lettres de l'atlas

// La regle du nuanceur kNkFontFragSDF, sur le processeur.
static float smoothstep(float a, float b, float x) {
	float t = (x - a) / (b - a);
	if (t < 0.f) t = 0.f;
	if (t > 1.f) t = 1.f;
	return t * t * (3.f - 2.f * t);
}

// Lecture bilineaire de l'atlas en UV : interpolation entre les quatre pixels voisins.
static float LireAtlas(const nkft_uint8 *px, int w, int h, float u, float v) {
	float x = u * float(w) - 0.5f;
	float y = v * float(h) - 0.5f;
	if (x < 0.f) x = 0.f;
	if (y < 0.f) y = 0.f;
	if (x > float(w - 1)) x = float(w - 1);
	if (y > float(h - 1)) y = float(h - 1);
	const int x0 = int(std::floor(x)), y0 = int(std::floor(y));
	const int x1 = (x0 + 1 < w) ? x0 + 1 : x0;
	const int y1 = (y0 + 1 < h) ? y0 + 1 : y0;
	const float fx = x - float(x0), fy = y - float(y0);
	const float a = px[y0 * w + x0], b = px[y0 * w + x1];
	const float c = px[y1 * w + x0], d = px[y1 * w + x1];
	return (a * (1.f - fx) + b * fx) * (1.f - fy) + (c * (1.f - fx) + d * fx) * fy;
}

static uint8 Melange(uint8 fond, uint8 texte, float couverture) {
	return uint8(float(fond) + (float(texte) - float(fond)) * couverture + 0.5f);
}

// Construit un atlas Inter a `taille` pixels ; sdf = mode champ de distance signee.
static NkFont *Construire(NkFontAtlas &atlas, float taille, bool sdf) {
	if (sdf) {
		atlas.sdfMode = true;
		atlas.sdfSpread = 6;
	}
	NkFont *police = NkFontEmbedded::AddToAtlas(atlas, NkEmbeddedFontId::Inter, taille);
	if (!police || !atlas.Build())
		return nullptr;
	return police;
}

// Fabrique l'image du mot. Meme taille quelle que soit la methode : seule la lecture change.
static NkImage Fabriquer(NkFontAtlas &atlas, NkFont *police, const char *mot, bool sdf, float s) {
	nkft_uint8 *px = nullptr;
	nkft_int32 aw = 0, ah = 0;
	atlas.GetTexDataAsAlpha8(&px, &aw, &ah);

	float largeurF = 0.f;
	for (const char *c = mot; *c; ++c) {
		const NkFontGlyph *g = police->FindGlyph((NkFontCodepoint)(unsigned char)*c);
		if (!g)
			return NkImage();
		largeurF += g->advanceX;
	}
	const int largeur = int(std::ceil(largeurF * kEchelle));
	const int hauteur = kEchelle * int(std::ceil(police->ascent - police->descent));
	const float base = police->ascent * kEchelle;

	NkImage image = NkImage::Create(uint32(largeur), uint32(hauteur), int32(4), uint32(0x1B7F3BFFu));
	if (!image.IsValid())
		return image;

	float curseur = 0.f;
	for (const char *c = mot; *c; ++c) {
		const NkFontGlyph *g = police->FindGlyph((NkFontCodepoint)(unsigned char)*c);
		const float rx0 = curseur + g->x0 * kEchelle, rx1 = curseur + g->x1 * kEchelle;
		const float ry0 = base + g->y0 * kEchelle, ry1 = base + g->y1 * kEchelle;

		for (int py = int(std::floor(ry0)); py < int(std::ceil(ry1)); ++py) {
			for (int pxl = int(std::floor(rx0)); pxl < int(std::ceil(rx1)); ++pxl) {
				if (pxl < 0 || py < 0 || pxl >= largeur || py >= hauteur)
					continue;
				const float fx = (float(pxl) + 0.5f - rx0) / (rx1 - rx0);
				const float fy = (float(py) + 0.5f - ry0) / (ry1 - ry0);
				if (fx < 0.f || fx > 1.f || fy < 0.f || fy > 1.f)
					continue;
				const float u = g->u0 + fx * (g->u1 - g->u0);
				const float v = g->v0 + fy * (g->v1 - g->v0);
				const float valeur = LireAtlas(px, aw, ah, u, v) / 255.f;

				// Ordinaire : la valeur EST la couverture.
				// SDF : la valeur est une distance ; le bord est a 0.5 (dessous = dehors).
				const float alpha = sdf ? smoothstep(0.5f - s, 0.5f + s, valeur) : valeur;

				const math::NkColor fond = image.GetPixel(pxl, py);
				image.SetPixel(pxl, py,
							   math::NkColor(Melange(fond.r, 255, alpha), Melange(fond.g, 255, alpha),
											 Melange(fond.b, 255, alpha), 255));
			}
		}
		curseur += g->advanceX * kEchelle;
	}
	return image;
}

int main(int argc, char **argv) {
	const float s = (argc > 1) ? float(atof(argv[1])) : 0.12f;

	// Trois atlas : ordinaire 32, SDF 32, ordinaire 128.
	NkFontAtlas ord32, sdf32, ord128;
	NkFont *f1 = Construire(ord32, 32.f, false);
	NkFont *f2 = Construire(sdf32, 32.f, true);
	NkFont *f3 = Construire(ord128, 128.f, false);
	if (!f1 || !f2 || !f3) {
		printf("construction d'un atlas echouee\n");
		return 1;
	}

	FILE *out = fopen("atlas.txt", "w");
	if (!out) {
		printf("impossible d'ecrire atlas.txt\n");
		return 1;
	}
	fprintf(out, "ordinaire 32 : %d x %d\n", ord32.texWidth, ord32.texHeight);
	fprintf(out, "sdf 32 : %d x %d\n", sdf32.texWidth, sdf32.texHeight);
	fprintf(out, "ordinaire 128 : %d x %d\n", ord128.texWidth, ord128.texHeight);
	fclose(out);
	printf("ordinaire 32 : %d x %d\nsdf 32 : %d x %d\nordinaire 128 : %d x %d\n", ord32.texWidth, ord32.texHeight,
		   sdf32.texWidth, sdf32.texHeight, ord128.texWidth, ord128.texHeight);

	// Deux images de meme taille, depuis les deux atlas de 32 pixels.
	NkImage imgOrd = Fabriquer(ord32, f1, "Sortie", false, s);
	NkImage imgSdf = Fabriquer(sdf32, f2, "Sortie", true, s);
	if (!imgOrd.IsValid() || !imgSdf.IsValid()) {
		printf("fabrication d'image echouee\n");
		return 1;
	}
	printf("ordinaire : %d x %d   sdf : %d x %d   (s = %.3f)\n", imgOrd.Width(), imgOrd.Height(), imgSdf.Width(),
		   imgSdf.Height(), s);
	if (imgOrd.Width() != imgSdf.Width() || imgOrd.Height() != imgSdf.Height())
		printf("ATTENTION : tailles differentes, la comparaison ne mesurerait plus la methode\n");

	imgOrd.SavePNG("ordinaire.png");
	imgSdf.SavePNG("sdf.png");

	// Memoire en jeu : une image huit fois plus grande a soixante-quatre fois plus de pixels.
	printf("pixels image : %d  (atlas 32 : %d)\n", imgOrd.Width() * imgOrd.Height(), ord32.texWidth * ord32.texHeight);
	return 0;
}
