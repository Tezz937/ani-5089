#pragma once
#include <cstdint>

// Une chaine d'echange : un anneau d'images, un drapeau d'emprunt.
// Les trois refus ne sont pas des trous : c'est le dessin.
class ChaineEchange {
public:
	static const uint32_t kMax = 4; // maximum connu et petit : pas d'allocation

	// Refuse (faux) une chaine vide ou plus grande que le maximum.
	bool Creer(uint32_t nombre) {
		if (nombre == 0 || nombre > kMax) return false;
		mNombre = nombre;
		mSuivant = 0;
		mEmprunte = false;
		mCree = true;
		return true;
	}

	// Refuse le double emprunt : une image est deja sortie.
	bool Acquerir(uint32_t &index) {
		if (!mCree || mEmprunte) return false;
		mEmprunte = true;
		index = mSuivant;
		return true;
	}

	// Refuse le rendu sans emprunt. L'anneau avance au RENDU, pas a l'emprunt.
	bool Rendre() {
		if (!mCree || !mEmprunte) return false;
		mEmprunte = false;
		mDernierRendu = mSuivant;
		mSuivant = (mSuivant + 1) % mNombre;
		return true;
	}

	uint32_t DernierRendu() const { return mDernierRendu; }

private:
	uint32_t mNombre = 0;
	uint32_t mSuivant = 0;
	uint32_t mDernierRendu = 0;
	bool mEmprunte = false;
	bool mCree = false;
};
