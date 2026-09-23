# Exercice 12 — La borne de cent millisecondes

Pour montrer la limite du modèle à vitesse constante, j'ai pris une tête dont la vitesse angulaire initiale est de 180 degrés par seconde, puis j'ai simulé une décélération constante de 360 degrés par seconde².

L'extrapolation utilise uniquement la vitesse initiale de 180 degrés par seconde. La pose de référence est obtenue en intégrant le mouvement réel avec la vitesse qui diminue progressivement.

| Durée | Erreur angulaire |
|---:|---:|
| 10 ms | 0,018° |
| 20 ms | 0,072° |
| 50 ms | 0,450° |
| 100 ms | 1,800° |
| 200 ms | 7,200° |
| 500 ms | 45,000° |
| 1 s | 180,000° |

La courbe montre une erreur qui augmente rapidement lorsque la durée d'extrapolation augmente.

```python
import numpy as np
import matplotlib.pyplot as plt

durees = np.array([0.01, 0.02, 0.05, 0.10, 0.20, 0.50, 1.00])
erreurs = 180 * durees**2

plt.plot(durees * 1000, erreurs, marker="o")
plt.xlabel("Durée d'extrapolation (ms)")
plt.ylabel("Erreur angulaire (degrés)")
plt.grid()
plt.show()
```

À 100 ms, l'erreur atteint déjà environ 1,8 degré dans ce scénario simple, puis elle augmente fortement au-delà. Cela montre pourquoi une extrapolation à vitesse constante doit rester courte : plus on avance dans le futur, plus une variation réelle du mouvement peut rendre la prédiction fausse.

La borne de 100 ms n'est donc pas une frontière mathématique absolue, mais une limite pratique à partir de laquelle le modèle à vitesse constante devient beaucoup moins fiable.
