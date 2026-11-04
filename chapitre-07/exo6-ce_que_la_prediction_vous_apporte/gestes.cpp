// Exercice 6 : ce que la prediction apporte, sur des gestes de tete connus, sans fenetre.
// Un casque a 72 Hz : une pose par periode, chaque pose est vue UNE periode plus tard.
// Avec prediction : la pose extrapolee d'une periode. Sans : la pose telle quelle.
// Erreur : angle avec la vraie pose de l'instant d'affichage. Ecrit gestes.txt.

#include "NKMath/NkAngle.h"
#include "NKMath/NkQuat.h"

#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::math;

static const double kPi = 3.14159265358979323846;
static const float kPeriode = 1.0f / 72.0f; // secondes

// Geste doux : depart et arrivee doux, lacet en degres (forme de l'enonce, amplitude et duree variables).
static double Lacet(double t, double amplitude, double duree) {
	if (t <= 0.0) return 0.0;
	if (t >= duree) return amplitude;
	return amplitude * 0.5 * (1.0 - std::cos(kPi * t / duree));
}

static NkQuatf Pose(double t, double amplitude, double duree) {
	return NkQuatf::RotateY(NkAngle(float(Lacet(t, amplitude, duree))));
}

// Vitesse angulaire entre deux poses horodatees (module de realite virtuelle du moteur).
static NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to, float32 dtSeconds) {
	if (dtSeconds <= 0.f) {
		return NkVec3f(0.f, 0.f, 0.f);
	}
	NkQuatf delta = (to * from.Conjugate()).Normalized();
	// Les deux hemispheres du quaternion codent la meme rotation : on force le chemin court.
	if (delta.w < 0.f) {
		delta = NkQuatf(-delta.x, -delta.y, -delta.z, -delta.w);
	}
	const float32 angleRad = delta.Angle().Rad();
	if (angleRad <= 1e-6f) {
		return NkVec3f(0.f, 0.f, 0.f);
	}
	return delta.Axis() * (angleRad / dtSeconds);
}

// Extrapolation : delta en espace de BASE, compose a GAUCHE de l'orientation courante.
static NkQuatf Extrapoler(const NkQuatf &orientation, const NkVec3f &vitesse, float32 dtSeconds) {
	const float32 speedRad = vitesse.Len();
	if (speedRad > 1e-6f) {
		const NkVec3f axis = vitesse * (1.f / speedRad);
		const NkQuatf delta(NkAngle::FromRad(speedRad * dtSeconds), axis);
		return (delta * orientation).Normalized();
	}
	return orientation;
}

// Angle entre deux orientations, en degres. On le calcule par 2 * atan2(|partie vectorielle|, w) :
// c'est le meme angle que Angle().Deg(), mais sans la perte de precision de acos pres de zero
// (en float, acos de 0,99999 vaut un angle bruite, du meme ordre que nos petites erreurs).
static double AngleDeg(const NkQuatf &a, const NkQuatf &b) {
	NkQuatf d = (b * a.Conjugate()).Normalized();
	if (d.w < 0.f) {
		d = NkQuatf(-d.x, -d.y, -d.z, -d.w);
	}
	const double vec = std::sqrt(double(d.x) * d.x + double(d.y) * d.y + double(d.z) * d.z);
	return 2.0 * std::atan2(vec, double(d.w)) * 180.0 / kPi;
}

struct Geste {
	const char *titre;
	double amplitude; // degres
	double duree;     // secondes
};

int main() {
	const Geste gestes[3] = {
		{"lent (30 deg en 1000 ms)", 30.0, 1.000},
		{"rapide (90 deg en 400 ms)", 90.0, 0.400},
		{"demi-tour (180 deg en 250 ms)", 180.0, 0.250},
	};

	FILE *out = std::fopen("gestes.txt", "w");
	if (!out) {
		std::printf("impossible d'ecrire gestes.txt\n");
		return 1;
	}

	for (const Geste &g : gestes) {
		double sommeAvec = 0.0, sommeSans = 0.0;
		int n = 0;
		// Mesure PENDANT le geste seulement : avant et apres, la tete est immobile.
		for (int k = 0;; ++k) {
			const double t = double(k) * double(kPeriode);
			if (t > g.duree) break;
			const NkQuatf precedente = Pose(t - double(kPeriode), g.amplitude, g.duree);
			const NkQuatf courante = Pose(t, g.amplitude, g.duree);
			const NkVec3f vitesse = VitesseAngulaire(precedente, courante, kPeriode);

			const NkQuatf avec = Extrapoler(courante, vitesse, kPeriode);
			const NkQuatf vraie = Pose(t + double(kPeriode), g.amplitude, g.duree);

			sommeAvec += AngleDeg(avec, vraie);
			sommeSans += AngleDeg(courante, vraie);
			++n;
		}
		const double avec = sommeAvec / n, sans = sommeSans / n;
		std::fprintf(out, "%s : avec %.4f deg, sans %.4f deg, rapport %.2f\n", g.titre, avec, sans, sans / avec);
		std::printf("%s : avec %.4f deg, sans %.4f deg, rapport %.2f  [%d poses]\n", g.titre, avec, sans,
					sans / avec, n);
	}
	std::fclose(out);
	return 0;
}
