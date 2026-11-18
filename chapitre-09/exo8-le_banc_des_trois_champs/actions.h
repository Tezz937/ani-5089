// actions.h : un etat d'action a trois champs.
//
//   courant : la valeur de l'action a cette image (ici un booleen : tenue ou non).
//   change  : vrai si courant differe de celui de l'image precedente (front de montee OU de descente).
//   actif   : vrai si une source est branchee a l'action. Sans source, courant reste faux.
//
// Une balle se tire sur un front (courant ET change), un objet se tient sur un niveau (courant).
#pragma once

struct EtatAction {
    bool courant = false;
    bool change = false;
    bool actif = false;
};

// Une action liee a une touche (indice dans le tableau des touches), ou a rien (touche < 0).
struct Action {
    const char *nom;
    int touche;
    EtatAction etat;
};

// Calcule l'etat de l'image a partir des touches. "change" se calcule sur l'ETAT (courant),
// pas sur la touche : une action non liee ou une manette eteinte produit donc aussi un change.
inline void MettreAJour(Action &action, const bool *touches, int nbTouches) {
    const bool precedent = action.etat.courant;
    const bool lie = (action.touche >= 0 && action.touche < nbTouches);

    action.etat.actif = lie;
    // Une action inactive rend toujours courant faux, quelle que soit la touche.
    action.etat.courant = lie && touches[action.touche];
    action.etat.change = (action.etat.courant != precedent);
}

// Front de saisie : courant ET change. Niveau : courant, quel que soit change.
inline bool EstFront(const EtatAction &etat) { return etat.courant && etat.change; }
inline bool EstNiveau(const EtatAction &etat) { return etat.courant; }
