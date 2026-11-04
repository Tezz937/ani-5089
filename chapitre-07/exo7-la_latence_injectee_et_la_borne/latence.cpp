// Exercice 7 : latence injectee et borne de cent millisecondes, sans fenetre.
// Poses d'un quart de tour en 400 ms commence a t = 1 s, une pose par periode de 72 Hz, 3 secondes.
// Historique circulaire de 64 poses (instant + vitesse), comme le simulateur du moteur.
// Ecrit latences.txt.

#include "NKMath/NkAngle.h"
#include "NKMath/NkQuat.h"

#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::math;

static const double kPi = 3.14159265358979323846;
static const double kPeriode = 1.0 / 72.0;
static const unsigned kMaxSamples = 64;

// Un quart de tour en 400 ms, depart et arrivee doux, commence a t = 1 s. Lacet en degres.
static double LacetDeg(double t) {
	t -= 1.0;
	const double amplitude = 90.0, duree = 0.400;
	if (t <= 0.0) return 0.0;
	if (t >= duree) return amplitude;
	return amplitude * 0.5 * (1.0 - std::cos(kPi * t / duree));
}

static NkQuatf Pose(double t) {
	return NkQuatf::RotateY(NkAngle(float(LacetDeg(t))));
}

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

static NkQuatf Extrapoler(const NkQuatf &orientation, const NkVec3f &vitesse, float32 dtSeconds) {
	const float32 speedRad = vitesse.Len();
	if (speedRad > 1e-6f) {
		const NkVec3f axis = vitesse * (1.f / speedRad);
		// Delta en espace de BASE : il se compose a GAUCHE de l'orientation courante.
		const NkQuatf delta(NkAngle::FromRad(speedRad * dtSeconds), axis);
		return (delta * orientation).Normalized();
	}
	return orientation;
}

// Angle entre deux orientations, en degres (2 * atan2 : precis meme tres pres de zero).
static double AngleDeg(const NkQuatf &a, const NkQuatf &b) {
	NkQuatf d = (b * a.Conjugate()).Normalized();
	if (d.w < 0.f) {
		d = NkQuatf(-d.x, -d.y, -d.z, -d.w);
	}
	const double vec = std::sqrt(double(d.x) * d.x + double(d.y) * d.y + double(d.z) * d.z);
	return 2.0 * std::atan2(vec, double(d.w)) * 180.0 / kPi;
}

// Lacet d'une rotation autour de l'axe vertical : 2 * atan2(q.y, q.w). On choisit l'hemisphere
// w >= 0 pour que le resultat reste entre -180 et 180 degres.
static double LacetDeQuat(NkQuatf q) {
	if (q.w < 0.f) {
		q = NkQuatf(-q.x, -q.y, -q.z, -q.w);
	}
	return 2.0 * std::atan2(double(q.y), double(q.w)) * 180.0 / kPi;
}

struct Sample {
	double time = 0.0; // secondes
	NkQuatf orientation;
	NkVec3f velocity;
};

// Tableau circulaire des 64 dernieres poses.
class Historique {
  public:
	void Reinit() {
		mHead = kMaxSamples - 1u;
		mCount = 0u;
	}
	void Ajouter(double time, const NkQuatf &orientation) {
		Sample s;
		s.time = time;
		s.orientation = orientation;
		if (mCount > 0u) {
			const Sample &prev = mSamples[mHead];
			s.velocity = VitesseAngulaire(prev.orientation, orientation, float32(time - prev.time));
		} else {
			s.velocity = NkVec3f(0.f, 0.f, 0.f);
		}
		mHead = (mHead + 1u) % kMaxSamples;
		mSamples[mHead] = s;
		if (mCount < kMaxSamples) ++mCount;
	}
	unsigned Compte() const { return mCount; }

	// Meme logique que SampleAt du simulateur : l'echantillon d'il y a « latence » secondes.
	const Sample &SampleAt(double now, double latenceSeconds) const {
		if (latenceSeconds <= 0.0 || mCount <= 1u) {
			return mSamples[mHead];
		}
		const double target = now - latenceSeconds;
		unsigned index = mHead;
		for (unsigned i = 0u; i < mCount; ++i) {
			if (mSamples[index].time <= target) {
				return mSamples[index];
			}
			index = (index + kMaxSamples - 1u) % kMaxSamples;
		}
		// Plus vieux disponible : l'historique ne remonte pas si loin.
		return mSamples[(mHead + kMaxSamples - (mCount - 1u)) % kMaxSamples];
	}

  private:
	Sample mSamples[kMaxSamples];
	unsigned mHead = kMaxSamples - 1u;
	unsigned mCount = 0u;
};

// Horizon de la prediction : (instant demande) - (instant de l'echantillon), borne ou non.
static float32 Horizon(double when, const Sample &s, bool borne) {
	float32 dt = float32(when - s.time);
	if (borne) {
		// Borne symetrique de cent millisecondes : au-dela, le modele ment plus qu'il n'aide.
		if (dt > 0.1f) dt = 0.1f;
		if (dt < -0.1f) dt = -0.1f;
	}
	return dt;
}

struct Stats {
	double sommeAvec = 0.0, maxAvec = 0.0, sommeSans = 0.0, maxSans = 0.0;
	int n = 0;
};

// Erreur de prediction pour une latence donnee, de t = 1 s a t = 1,9 s.
static Stats MesurerLatence(double latenceSeconds, bool borne) {
	Historique h;
	h.Reinit();
	Stats st;
	for (int k = 0; double(k) * kPeriode <= 3.0; ++k) {
		const double now = double(k) * kPeriode;
		h.Ajouter(now, Pose(now));
		if (now < 1.0 || now > 1.9) continue;

		const Sample &s = h.SampleAt(now, latenceSeconds);
		const double quand = now + kPeriode; // l'image sera vue une periode plus tard
		const NkQuatf predite = Extrapoler(s.orientation, s.velocity, Horizon(quand, s, borne));
		const NkQuatf vraie = Pose(quand); // la VRAIE pose de l'instant d'affichage

		const double eAvec = AngleDeg(predite, vraie);
		const double eSans = AngleDeg(s.orientation, vraie); // la pose de l'echantillon, telle quelle
		st.sommeAvec += eAvec;
		st.sommeSans += eSans;
		if (eAvec > st.maxAvec) st.maxAvec = eAvec;
		if (eSans > st.maxSans) st.maxSans = eSans;
		++st.n;
	}
	return st;
}

// Latence d'une seconde : l'image dont l'echantillon est le plus proche du milieu du geste (t = 1,2 s).
static void LigneBorne(FILE *out, const char *nom, bool borne) {
	Historique h;
	h.Reinit();
	double meilleurEcart = 1e9, tSample = 0.0, horizonMs = 0.0, predDeg = 0.0, vraiDeg = 0.0;
	for (int k = 0; double(k) * kPeriode <= 3.0; ++k) {
		const double now = double(k) * kPeriode;
		h.Ajouter(now, Pose(now));
		if (now < 1.0) continue;
		const Sample &s = h.SampleAt(now, 1.0);
		const double ecart = std::fabs(s.time - 1.2);
		if (ecart < meilleurEcart) {
			meilleurEcart = ecart;
			const double quand = now + kPeriode;
			const float32 dt = Horizon(quand, s, borne);
			tSample = s.time;
			horizonMs = double(dt) * 1000.0;
			predDeg = LacetDeQuat(Extrapoler(s.orientation, s.velocity, dt));
			vraiDeg = LacetDeQuat(Pose(quand));
		}
	}
	std::fprintf(out, "%s : echantillon a %.4f s, horizon %.1f ms, predit %.2f deg, vrai %.2f deg\n", nom, tSample,
				 horizonMs, predDeg, vraiDeg);
	std::printf("%s : echantillon a %.4f s, horizon %.1f ms, predit %.2f deg, vrai %.2f deg\n", nom, tSample,
				horizonMs, predDeg, vraiDeg);
}

int main() {
	FILE *out = std::fopen("latences.txt", "w");
	if (!out) {
		std::printf("impossible d'ecrire latences.txt\n");
		return 1;
	}

	const int latencesMs[5] = {0, 10, 20, 40, 80};
	for (int ms : latencesMs) {
		const Stats st = MesurerLatence(double(ms) / 1000.0, true);
		std::fprintf(out, "latence %d ms : avec %.4f deg (max %.4f), sans %.4f deg (max %.4f)\n", ms,
					 st.sommeAvec / st.n, st.maxAvec, st.sommeSans / st.n, st.maxSans);
		std::printf("latence %d ms : avec %.4f deg (max %.4f), sans %.4f deg (max %.4f)  [%d images]\n", ms,
					st.sommeAvec / st.n, st.maxAvec, st.sommeSans / st.n, st.maxSans, st.n);
	}

	std::printf("historique : %u poses, %.3f s ; periode %.4f ms\n", kMaxSamples, (kMaxSamples - 1u) * kPeriode,
				kPeriode * 1000.0);
	LigneBorne(out, "sans borne", false);
	LigneBorne(out, "avec borne", true);
	std::fclose(out);
	return 0;
}
