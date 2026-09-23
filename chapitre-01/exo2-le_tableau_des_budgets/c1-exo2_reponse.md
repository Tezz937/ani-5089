# Exercice 2 — Le tableau des budgets

| Étape | Valeur retenue | Source |
|---|---:|---|
| Capteurs | 1 ms entre deux mesures à 1000 Hz | Oculus Rift : les observations des capteurs sont rapportées à 1000 Hz, soit un intervalle de 1 ms. Source : *Head Tracking for the Oculus Rift*, University of North Carolina. |
| Transmission des mesures | Non isolée proprement | La littérature mesure généralement la chaîne complète plutôt que la seule transmission capteur → ordinateur. Je ne donne donc pas une valeur inventée pour cette étape. Source de contexte : Warburton et al., *Measuring motion-to-photon latency for sensorimotor experiments with virtual reality systems*. |
| Application : simulation et rendu | 6,41 ms dans un exemple de statistiques Meta Quest 2 | Meta documente un exemple de statistiques donnant `CPU&GPU = 6,41 ms` pour une application Quest 2. Cette valeur regroupe le travail CPU/GPU et n'est donc pas un temps de rendu GPU pur. Source : Meta Horizon OS Developers, documentation des statistiques de performance. |
| Compositeur | 2,80 ms dans un exemple Meta Quest 2 | Meta donne un exemple de statistiques avec `TW = 2,80 ms`, correspondant au travail du compositeur / TimeWarp. Source : Meta Horizon OS Developers, documentation des couches du compositeur. |
| Affichage | 0,5 à 2 ms d'illumination de l'écran | Une étude mesurant plusieurs casques décrit des écrans à faible persistance illuminés pendant environ 0,5 à 2 ms en fin de cycle de rafraîchissement. Source : Warburton et al., *Measuring motion-to-photon latency for sensorimotor experiments with virtual reality systems*. |

## Remarque

Les valeurs ne sont pas toutes directement comparables, car elles ne viennent pas du même casque ni du même protocole. L'objectif est surtout de vérifier qu'un budget de latence est composé de plusieurs étapes mesurables. La transmission seule est la partie pour laquelle je n'ai pas trouvé de mesure isolée suffisamment propre pour la présenter comme un fait.
