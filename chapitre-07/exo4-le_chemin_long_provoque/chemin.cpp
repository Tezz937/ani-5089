// Exercice 4 : le chemin long, provoque.
// La vitesse angulaire entre deux orientations, SANS le test qui force le chemin court.
// La tete tourne par un tour programme : MASALLE_TOUR = vitesse en degres par seconde, le lacet
// suit l'HORLOGE (pas la souris). On laisse faire un tour complet, on imprime chaque image.
// Usage : chemin "sans le test" sans.txt   -> ecrit les trois lignes autour de l'image fautive.

#include "NKMath/NkAngle.h"
#include "NKMath/NkMat.h"
#include "NKMath/NkQuat.h"
#include "NKTime/NkChrono.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::math;

static const double kPi = 3.14159265358979323846;
static const int kMaxImages = 8192;

// Vitesse angulaire entre deux orientations horodatees (module de realite virtuelle du moteur).
// wBrut : la composante w du delta, relevee avant tout traitement, pour l'impression.
static NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to, float32 dtSeconds, float &wBrut) {
	wBrut = 1.f;
	if (dtSeconds <= 0.f) {
		return NkVec3f(0.f, 0.f, 0.f);
	}
	NkQuatf delta = (to * from.Conjugate()).Normalized();
	wBrut = delta.w;
	// Test du chemin court : retire dans cette version, expres.
	const float32 angleRad = delta.Angle().Rad();
	if (angleRad <= 1e-6f) {
		return NkVec3f(0.f, 0.f, 0.f);
	}
	return delta.Axis() * (angleRad / dtSeconds);
}

struct Ligne {
	double lacetDeg;
	float vx, vy, vz; // degres par seconde
	float w;
	double vitesseDeg; // norme
};

static Ligne gLignes[kMaxImages];

static void Imprimer(FILE *f, int n) {
	const Ligne &l = gLignes[n];
	std::fprintf(f, "image %d : lacet = %.2f deg, vitesse = (%.2f, %.2f, %.2f) deg/s, delta.w = %.6f\n", n, l.lacetDeg,
				 l.vx, l.vy, l.vz, l.w);
}

int main(int argc, char **argv) {
	const char *titre = (argc > 1) ? argv[1] : "sans le test";
	const char *fichier = (argc > 2) ? argv[2] : "trois_lignes.txt";
	const char *envTour = std::getenv("MASALLE_TOUR");
	const char *envTang = std::getenv("MASALLE_TANGAGE");
	const double tourDegS = envTour ? std::atof(envTour) : 20.0;
	const double tangageDeg = envTang ? std::atof(envTang) : 0.0;
	if (tourDegS <= 0.0) {
		std::printf("MASALLE_TOUR doit etre une vitesse positive, en degres par seconde\n");
		return 1;
	}
	const double duree = 360.0 / tourDegS + 0.2; // un tour complet, plus une marge

	const double t0 = double(NkChrono::Now().nanoseconds) * 1e-9;
	NkQuatf precedente;
	double tPrecedent = 0.0;
	int nImages = 0;

	for (int n = 0; n < kMaxImages; ++n) {
		const double t = double(NkChrono::Now().nanoseconds) * 1e-9 - t0; // l'horloge fixe le lacet
		if (t > duree) break;

		const double lacetDeg = tourDegS * t;
		const float lacetRad = float(lacetDeg * kPi / 180.0);
		const float tangageRad = float(tangageDeg * kPi / 180.0);

		// Orientation comme le simulateur du moteur : deux matrices, puis une conversion en quaternion.
		const NkMat4f rotation = NkMat4f::RotationY(NkAngle::FromRad(lacetRad)) *
								 NkMat4f::RotationX(NkAngle::FromRad(tangageRad));
		const NkQuatf orientation = NkQuatf(rotation).Normalized();

		Ligne &l = gLignes[n];
		l.lacetDeg = lacetDeg;
		l.vx = l.vy = l.vz = 0.f;
		l.w = 1.f;
		l.vitesseDeg = 0.0;
		if (n > 0) {
			const NkVec3f v = VitesseAngulaire(precedente, orientation, float(t - tPrecedent), l.w);
			const float k = float(180.0 / kPi);
			l.vx = v.x * k;
			l.vy = v.y * k;
			l.vz = v.z * k;
			l.vitesseDeg = std::sqrt(double(l.vx) * l.vx + double(l.vy) * l.vy + double(l.vz) * l.vz);
		}
		precedente = orientation;
		tPrecedent = t;
		++nImages;
		NkChrono::SleepMilliseconds(16); // une image a peu pres toutes les 16 ms
	}

	// L'image fautive : la premiere dont la vitesse depasse cinq fois la vitesse programmee.
	int fautive = -1, nFautives = 0;
	for (int n = 1; n < nImages; ++n) {
		if (gLignes[n].w < 0.f) {
			if (fautive < 0) fautive = n;
			++nFautives;
		}
	}

	std::printf("tour programme : %.1f deg/s, tangage %.1f deg, %d images, %d image(s) fautive(s)\n", tourDegS,
				tangageDeg, nImages, nFautives);
	FILE *out = std::fopen(fichier, "w");
	if (!out) {
		std::printf("impossible d'ecrire %s\n", fichier);
		return 1;
	}
	std::fprintf(out, "%s\n", titre);
	if (fautive < 0) {
		std::printf("aucune image fautive : je note l'image de vitesse maximale, sans conclure\n");
		fautive = 1;
		for (int n = 1; n < nImages; ++n)
			if (gLignes[n].vitesseDeg > gLignes[fautive].vitesseDeg) fautive = n;
	}
	for (int n = fautive - 1; n <= fautive + 1 && n < nImages; ++n) {
		if (n < 0) continue;
		Imprimer(out, n);
		Imprimer(stdout, n);
	}
	std::fclose(out);
	return 0;
}
