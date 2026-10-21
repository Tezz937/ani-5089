// Exercice 12 : gain des quatre modeles d'attenuation a cinq distances, deux rolloff.
// Sans fenetre ni sortie sonore (NULL_OUTPUT). Ecrit mesures.txt : quarante lignes.

#include "NKAudio/NkAudio.h"

#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::audio;

static const int kTrames = 48000;
static const int kBloc = 256;

// Racine de la moyenne des carres sur les DEUX oreilles ensemble.
static double MesurerRms() {
	float tampon[kBloc * 2];
	double somme = 0.0;
	int fait = 0;
	while (fait < kTrames) {
		const int n = (kTrames - fait < kBloc) ? (kTrames - fait) : kBloc;
		AudioEngine::Instance().RenderToBuffer(tampon, n, 2);
		for (int i = 0; i < n * 2; ++i)
			somme += double(tampon[i]) * double(tampon[i]);
		fait += n;
	}
	return std::sqrt(somme / (double(kTrames) * 2.0));
}

static double Mesure(const AudioSample &son, AttenuationModel modele, float distance, float rolloff) {
	VoiceParams p;
	p.looping = true;
	p.source3d.positional = true;
	p.source3d.minDistance = 1.f;
	p.source3d.maxDistance = 100.f;
	p.source3d.rolloffFactor = rolloff;
	p.source3d.attenuationModel = modele;
	p.source3d.position[0] = 0.f;
	p.source3d.position[1] = 0.f;
	p.source3d.position[2] = -distance; // devant l'auditeur

	AudioHandle h = AudioEngine::Instance().Play(son, p);
	const double rms = MesurerRms();
	AudioEngine::Instance().Stop(h); // la source reste immobile pendant toute la mesure
	return rms;
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
	moteur.SetListenerPosition(0.f, 0.f, 0.f);
	moteur.SetListenerOrientation(0.f, 0.f, -1.f, 0.f, 1.f, 0.f);

	// Sinus doux : amplitude faible, le moteur adoucit les fortes valeurs.
	AudioSample son = AudioGenerator::GenerateTone(440.f, 2.f, WaveformType::SINE, 48000, 0.1f);
	if (!son.IsValid()) {
		printf("GenerateTone a echoue\n");
		moteur.Shutdown();
		return 1;
	}

	const AttenuationModel modeles[4] = {AttenuationModel::NONE, AttenuationModel::INVERSE,
										 AttenuationModel::LINEAR, AttenuationModel::EXPONENTIAL};
	const char *noms[4] = {"NONE", "INVERSE", "LINEAR", "EXPONENTIAL"};
	const float distances[5] = {0.5f, 1.f, 2.f, 4.f, 8.f};
	const float rolloffs[2] = {1.f, 2.f};

	FILE *out = fopen("mesures.txt", "w");
	if (!out) {
		printf("impossible d'ecrire mesures.txt\n");
		moteur.Shutdown();
		AudioLoader::Free(son);
		return 1;
	}

	// Une table brute (rms) pour calculer ensuite chaque gain par rapport a NONE.
	double rms[2][4][5];
	for (int r = 0; r < 2; ++r)
		for (int m = 0; m < 4; ++m)
			for (int d = 0; d < 5; ++d)
				rms[r][m][d] = Mesure(son, modeles[m], distances[d], rolloffs[r]);

	// Le gain de chaque modele est rapporte a NONE a la meme distance (et au meme rolloff).
	// NONE lui-meme vaut donc 1.000 : on l'ecrit aussi (les 40 lignes demandees).
	for (int r = 0; r < 2; ++r) {
		for (int m = 0; m < 4; ++m) {
			for (int d = 0; d < 5; ++d) {
				const double ref = rms[r][0][d];
				const double gain = (ref > 0.0) ? rms[r][m][d] / ref : 0.0;
				fprintf(out, "%s, rolloff %g, %g m : %.3f\n", noms[m], double(rolloffs[r]), double(distances[d]), gain);
				printf("%s, rolloff %g, %g m : %.3f   (rms %.6f)\n", noms[m], double(rolloffs[r]),
					   double(distances[d]), gain, rms[r][m][d]);
			}
		}
	}
	fclose(out);

	moteur.Shutdown();
	AudioLoader::Free(son);
	return 0;
}
