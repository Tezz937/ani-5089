// Exercice 10 : un modele de prediction a acceleration, compare au modele a vitesse constante.
// Travail sur le lacet seul, en degres, sans quaternion. Ecrit modeles.txt.
// Echantillonnage a 72 Hz : une pose par periode ; prediction une periode plus loin.

#include <cmath>
#include <cstdio>

static const double kPi = 3.14159265358979323846;
static const double kHz = 72.0;
static const double kPeriode = 1.0 / kHz; // secondes

struct Geste {
	const char *titre;
	double amplitude; // degres
	double duree;     // secondes
	bool arretNet;    // vrai : vitesse constante puis arret net
};

// Geste doux : depart et arrivee doux (meme forme que l'enonce).
static double LacetDoux(double t, double amplitude, double duree) {
	if (t <= 0.0) return 0.0;
	if (t >= duree) return amplitude;
	return amplitude * 0.5 * (1.0 - std::cos(kPi * t / duree));
}

// Arret brusque : vitesse constante, puis arret net a t = duree. Vaut aussi pour t < 0 :
// la tete tournait deja a vitesse constante avant le debut de la mesure.
static double LacetArret(double t, double amplitude, double duree) {
	if (t >= duree) return amplitude;
	return (amplitude / duree) * t;
}

static double Lacet(const Geste &g, double t) {
	return g.arretNet ? LacetArret(t, g.amplitude, g.duree) : LacetDoux(t, g.amplitude, g.duree);
}

struct Erreurs {
	double somme = 0.0, max = 0.0;
	int n = 0;
	void Ajouter(double e) {
		somme += e;
		if (e > max) max = e;
		++n;
	}
	double Moyenne() const { return n ? somme / n : 0.0; }
};

int main() {
	const Geste gestes[4] = {
		{"lent (30 deg en 1000 ms)", 30.0, 1.000, false},
		{"rapide (90 deg en 400 ms)", 90.0, 0.400, false},
		{"demi-tour (180 deg en 250 ms)", 180.0, 0.250, false},
		{"arret (120 deg/s, arret net a 300 ms)", 36.0, 0.300, true},
	};

	FILE *out = std::fopen("modeles.txt", "w");
	if (!out) {
		std::printf("impossible d'ecrire modeles.txt\n");
		return 1;
	}

	for (const Geste &g : gestes) {
		Erreurs vit, acc;
		const double fin = g.duree + 0.200; // pendant le geste et les 200 ms qui suivent
		for (int k = 0;; ++k) {
			const double t = k * kPeriode;
			if (t > fin) break;
			// Les trois dernieres poses : t, t - T, t - 2T.
			const double y0 = Lacet(g, t);
			const double y1 = Lacet(g, t - kPeriode);
			const double y2 = Lacet(g, t - 2.0 * kPeriode);

			const double vitesse = (y0 - y1) / kPeriode;                       // deg/s, deux poses
			const double accel = (y0 - 2.0 * y1 + y2) / (kPeriode * kPeriode); // deg/s^2, trois poses

			// Les deux modeles predisent la MEME pose : celle de t + une periode.
			const double predV = y0 + vitesse * kPeriode;
			const double predA = y0 + vitesse * kPeriode + accel * kPeriode * kPeriode / 2.0;
			const double vrai = Lacet(g, t + kPeriode);

			vit.Ajouter(std::fabs(predV - vrai));
			acc.Ajouter(std::fabs(predA - vrai));
		}
		std::fprintf(out, "%s : vitesse %.4f deg (max %.4f), acceleration %.4f deg (max %.4f)\n", g.titre,
					 vit.Moyenne(), vit.max, acc.Moyenne(), acc.max);
		std::printf("%s : vitesse %.4f deg (max %.4f), acceleration %.4f deg (max %.4f)  [%d poses]\n", g.titre,
					vit.Moyenne(), vit.max, acc.Moyenne(), acc.max, vit.n);
	}
	std::fclose(out);
	return 0;
}
