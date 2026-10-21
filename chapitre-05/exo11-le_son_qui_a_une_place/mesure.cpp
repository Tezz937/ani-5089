// Exercice 11 (partie mesure) : ce que le moteur envoie a chaque oreille.
// Sans fenetre ni sortie sonore (NULL_OUTPUT). Ecrit mesures.txt : six lignes.

#include "NKAudio/NkAudio.h"

#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::audio;

static const int kTrames = 48000;
static const int kBloc = 256;

// Rend kTrames trames par blocs de kBloc ; calcule la racine de la moyenne des carres
// de l'oreille gauche puis de la droite (echantillons ranges G, D, G, D...).
static void MesurerOreilles(double &rmsG, double &rmsD) {
	float tampon[kBloc * 2];
	double sg = 0.0, sd = 0.0;
	int fait = 0;
	while (fait < kTrames) {
		const int n = (kTrames - fait < kBloc) ? (kTrames - fait) : kBloc;
		AudioEngine::Instance().RenderToBuffer(tampon, n, 2);
		for (int i = 0; i < n; ++i) {
			const double g = tampon[i * 2 + 0];
			const double d = tampon[i * 2 + 1];
			sg += g * g;
			sd += d * d;
		}
		fait += n;
	}
	rmsG = std::sqrt(sg / kTrames);
	rmsD = std::sqrt(sd / kTrames);
}

int main() {
	AudioEngineConfig cfg;
	cfg.backend = AudioBackendType::NULL_OUTPUT;
	cfg.sampleRate = 48000;
	if (!AudioEngine::Instance().Initialize(cfg)) {
		printf("Initialize a echoue\n");
		return 1;
	}

	AudioEngine &moteur = AudioEngine::Instance();

	// L'auditeur est a l'origine et regarde vers -Z.
	moteur.SetListenerPosition(0.f, 0.f, 0.f);
	moteur.SetListenerOrientation(0.f, 0.f, -1.f, 0.f, 1.f, 0.f);

	AudioSample pluie = AudioGenerator::GenerateNoise(4.f, WaveformType::NOISE_PINK);
	if (!pluie.IsValid()) {
		printf("GenerateNoise a echoue\n");
		moteur.Shutdown();
		return 1;
	}

	struct Position {
		const char *nom;
		float x, y, z;
	};
	const Position positions[3] = {
		{"gauche", -2.f, 0.f, 0.f},
		{"devant", 0.f, 0.f, -2.f},
		{"droite", 2.f, 0.f, 0.f},
	};

	FILE *out = fopen("mesures.txt", "w");
	if (!out) {
		printf("impossible d'ecrire mesures.txt\n");
		moteur.Shutdown();
		AudioLoader::Free(pluie);
		return 1;
	}

	for (int p = 0; p < 3; ++p) {
		for (int spatial = 1; spatial >= 0; --spatial) {
			VoiceParams v;
			v.looping = true;
			v.source3d.position[0] = positions[p].x;
			v.source3d.position[1] = positions[p].y;
			v.source3d.position[2] = positions[p].z;
			v.source3d.minDistance = 1.f;
			v.source3d.maxDistance = 20.f;
			v.source3d.positional = (spatial == 1);

			AudioHandle h = moteur.Play(pluie, v);

			double g = 0.0, d = 0.0;
			MesurerOreilles(g, d);

			// La source reste immobile pendant la mesure, puis on l'arrete avant de la deplacer.
			moteur.Stop(h);

			fprintf(out, "%s %d : G %.6f D %.6f\n", positions[p].nom, spatial, g, d);
			printf("%s %d : G %.6f D %.6f\n", positions[p].nom, spatial, g, d);
		}
	}
	fclose(out);

	moteur.Shutdown();
	AudioLoader::Free(pluie);
	return 0;
}
