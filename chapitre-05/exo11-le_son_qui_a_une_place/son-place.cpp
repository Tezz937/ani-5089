// Exercice 11 : pluie en boucle placee a la fenetre, auditeur a la place de la camera.
// Usage : son-place x y z   (position de la camera) ; MASALLE_SANS=1 desactive positional.
#include "NKAudio/NkAudio.h"
#include "NKTime/NkChrono.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace nkentseu;
using namespace nkentseu::audio;

int main(int argc, char **argv) {
	const float cx = argc > 3 ? float(atof(argv[1])) : 1.5f;
	const float cy = argc > 3 ? float(atof(argv[2])) : 1.7f;
	const float cz = argc > 3 ? float(atof(argv[3])) : 1.6f;
	AudioEngineConfig cfg;
	cfg.sampleRate = 48000;
	if (!AudioEngine::Instance().Initialize(cfg)) { printf("Initialize a echoue\n"); return 1; }
	AudioEngine &moteur = AudioEngine::Instance();

	// L'auditeur suit la camera : position, et regard de la salle (de (1.5,1.7,1.6) vers (-1.2,1.25,-0.9)).
	float dx = -2.7f, dy = -0.45f, dz = -2.5f;
	const float n = std::sqrt(dx * dx + dy * dy + dz * dz);
	moteur.SetListenerPosition(cx, cy, cz);
	moteur.SetListenerOrientation(dx / n, dy / n, dz / n, 0.f, 1.f, 0.f);

	AudioSample pluie = AudioGenerator::GenerateNoise(4.f, WaveformType::NOISE_PINK);
	if (!pluie.IsValid()) { printf("GenerateNoise a echoue\n"); moteur.Shutdown(); return 1; }

	// Pluie a la premiere fenetre ; distances choisies : minDistance 1 m, maxDistance 20 m.
	VoiceParams p;
	p.looping = true;
	p.source3d.position[0] = -1.98f;
	p.source3d.position[1] = 1.40f;
	p.source3d.position[2] = -0.90f;
	p.source3d.minDistance = 1.f;
	p.source3d.maxDistance = 20.f;
	p.source3d.positional = (getenv("MASALLE_SANS") == nullptr);
	moteur.Play(pluie, p);
	printf("camera (%.2f, %.2f, %.2f), positional = %d\n", cx, cy, cz, p.source3d.positional ? 1 : 0);

	NkChrono horloge;
	while (horloge.Elapsed().ToMilliseconds() < 5000.0) {}

	moteur.Shutdown();
	AudioLoader::Free(pluie);
	return 0;
}
