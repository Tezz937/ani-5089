// Exercice 7 : le mot « Sortie » fabrique a partir d'un atlas de police embarquee.
//
// PARTIE A (ce fichier, autonome) : atlas -> image panneau.png + panneau.txt (tailles).
// PARTIE B (PoserPanneau, en bas)  : pose de l'image comme texture sur un pave de la salle.
//   Elle depend du renderer de MaSalle : voir le bloc PARTIE B.

#include "NKFont/NkFont.h"
#include "NKFont/Embedded/NkFontEmbedded.h"
#include "NKImage/Core/NkImage.h"

#include <cmath>
#include <cstdio>

using namespace nkentseu;

// Lecture bilineaire de l'atlas (octet 0..255) en coordonnees UV [0,1] : quatre voisins.
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

int main() {
	// --- 1. Atlas : police embarquee dans le moteur, aucun fichier a fournir ---
	NkFontAtlas atlas;
	NkFont *police = NkFontEmbedded::AddToAtlas(atlas, NkEmbeddedFontId::Inter, 32.f);
	if (!police) {
		printf("AddToAtlas a echoue\n");
		return 1;
	}
	if (!atlas.Build()) {
		printf("atlas.Build a echoue\n");
		return 1;
	}

	nkft_uint8 *pixelsAtlas = nullptr;
	nkft_int32 aw = 0, ah = 0;
	atlas.GetTexDataAsAlpha8(&pixelsAtlas, &aw, &ah);
	if (!pixelsAtlas || aw <= 0 || ah <= 0) {
		printf("GetTexDataAsAlpha8 a echoue\n");
		return 1;
	}
	printf("atlas : %d x %d\n", atlas.texWidth, atlas.texHeight);

	// --- 2. Image du panneau ---
	const char *mot = "Sortie";
	float largeurF = 0.f;
	for (const char *c = mot; *c; ++c) {
		const NkFontGlyph *g = police->FindGlyph((NkFontCodepoint)(unsigned char)*c);
		if (!g) {
			printf("glyphe absent : %c\n", *c);
			return 1;
		}
		largeurF += g->advanceX;
	}
	const int largeur = int(std::ceil(largeurF));
	const int hauteur = int(std::ceil(police->ascent - police->descent));
	const float base = police->ascent; // ligne de base, a `ascent` pixels du haut

	// Fond vert « sortie de secours », texte blanc.
	NkImage image = NkImage::Create(uint32(largeur), uint32(hauteur), int32(4), uint32(0x1B7F3BFFu));
	if (!image.IsValid()) {
		printf("NkImage::Create a echoue\n");
		return 1;
	}

	float curseur = 0.f;
	for (const char *c = mot; *c; ++c) {
		const NkFontGlyph *g = police->FindGlyph((NkFontCodepoint)(unsigned char)*c);
		const float rx0 = curseur + g->x0, rx1 = curseur + g->x1;
		const float ry0 = base + g->y0, ry1 = base + g->y1;

		for (int py = int(std::floor(ry0)); py < int(std::ceil(ry1)); ++py) {
			for (int px = int(std::floor(rx0)); px < int(std::ceil(rx1)); ++px) {
				if (px < 0 || py < 0 || px >= largeur || py >= hauteur)
					continue;
				// Fraction du pixel (centre) dans le rectangle de la lettre -> UV dans l'atlas.
				const float fx = (float(px) + 0.5f - rx0) / (rx1 - rx0);
				const float fy = (float(py) + 0.5f - ry0) / (ry1 - ry0);
				if (fx < 0.f || fx > 1.f || fy < 0.f || fy > 1.f)
					continue;
				const float u = g->u0 + fx * (g->u1 - g->u0);
				const float v = g->v0 + fy * (g->v1 - g->v0);
				const float couverture = LireAtlas(pixelsAtlas, aw, ah, u, v) / 255.f;

				const math::NkColor fond = image.GetPixel(px, py);
				image.SetPixel(px, py,
							   math::NkColor(Melange(fond.r, 255, couverture), Melange(fond.g, 255, couverture),
											 Melange(fond.b, 255, couverture), 255));
			}
		}
		curseur += g->advanceX;
	}

	// Si le mot apparait a l'envers ou en miroir DANS LA SALLE (pas dans ce PNG), c'est le sens
	// de la face du pave : decommentez l'une de ces lignes avant de sauvegarder.
	// image.FlipVertical();
	// image.FlipHorizontal();

	if (!image.SavePNG("panneau.png")) {
		printf("SavePNG a echoue\n");
		return 1;
	}
	printf("image : %d x %d\n", image.Width(), image.Height());

	FILE *out = fopen("panneau_tailles.txt", "w");
	if (out) {
		fprintf(out, "atlas : %d x %d\nimage : %d x %d\n", atlas.texWidth, atlas.texHeight, image.Width(),
				image.Height());
		fclose(out);
	}

	// Proportions pour le pave : 20 cm de haut -> largeur = 0.20 * largeur / hauteur.
	printf("pave : 0.20 m de haut x %.3f m de large\n", 0.20 * double(largeur) / double(hauteur));
	return 0;
}

// =============================================================================
// PARTIE B : a COLLER dans MaSalle (ce code depend de votre renderer et de Pave).
// =============================================================================
//
//   // Une seule fois, AVANT la boucle :
//   NkTextureHandle tex = renderer->GetTextures()->Load("panneau.png");   // via LocateResource (exo 6)
//   NkMaterial *mat = NkMaterial::Create(renderer->GetMaterials(), NkMaterialType::NK_PBR_METALLIC);
//   mat->SetAlbedoMap(tex);
//
//   // Dans la boucle, un pave de 0.20 m de haut, de largeur 0.20*L/H, centre en x = -0.70,
//   // centre en y = 2.0 + 0.15 (au-dessus de la porte haute de 2 m), et a z = -2 + 0.005 (decolle du mur).
//   dc.material = mat->GetInstHandle();
