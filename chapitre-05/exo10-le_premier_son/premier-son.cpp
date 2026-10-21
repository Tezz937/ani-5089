// Exercice 10 : faire jouer un son au demarrage, sans position dans l'espace.
// Ecrit demarrage.txt avec les quatre valeurs annoncees par le moteur.

#include "NKAudio/NkAudio.h"
#include "NKTime/NkChrono.h"

#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::audio;

int main() {
	// --- debut du compte de lignes : initialisation ---
	AudioEngineConfig cfg;
	cfg.sampleRate = 48000;
	if (!AudioEngine::Instance().Initialize(cfg)) {
		printf("Initialize a echoue\n");
		return 1;
	}

	AudioEngine &moteur = AudioEngine::Instance();

	// Les quatre valeurs, relevees juste apres l'initialisation, sans retouche.
	char l1[128], l2[128], l3[128], l4[128];
	snprintf(l1, sizeof(l1), "dorsal : %s", moteur.GetBackendName());
	snprintf(l2, sizeof(l2), "frequence : %d", moteur.GetSampleRate());
	snprintf(l3, sizeof(l3), "tampon : %d", moteur.GetBufferSize());
	snprintf(l4, sizeof(l4), "latence : %.3f", moteur.GetLatencyMs());
	printf("%s\n%s\n%s\n%s\n", l1, l2, l3, l4);

	FILE *out = fopen("demarrage.txt", "w");
	if (out) {
		fprintf(out, "%s\n%s\n%s\n%s\n", l1, l2, l3, l4);
		fclose(out);
	}

	// Un la 440 Hz d'une seconde, joue une fois, SANS position (non spatialise).
	AudioSample son = AudioGenerator::GenerateTone(440.f, 1.f);
	if (!son.IsValid()) {
		printf("GenerateTone a echoue\n");
		moteur.Shutdown();
		return 1;
	}
	moteur.Play(son);

	// Boucle de "rendu" : le son joue sur un autre fil pendant qu'elle tourne.
	NkChrono horloge;
	while (horloge.Elapsed().ToMilliseconds() < 1500.0) {
		// ici : le travail de la salle (ne jamais calculer de son dans cette boucle)
	}

	// Fermeture : d'abord le moteur, ENSUITE l'echantillon (une voix lit encore ses donnees).
	moteur.Shutdown();
	AudioLoader::Free(son);
	// --- fin du compte de lignes ---
	return 0;
}
