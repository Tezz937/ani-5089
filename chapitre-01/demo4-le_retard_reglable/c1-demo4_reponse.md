# Démonstration 4 — Le retard réglable

J'ai fait essayer le programme à trois personnes en augmentant progressivement le retard par paliers.

Les valeurs ont été relevées lorsque chaque personne commençait à remarquer une différence entre son mouvement et la réaction affichée.

| Participant | Seuil ressenti |
|---|---:|
| Participant 1 | 28 ms |
| Participant 2 | 35 ms |
| Participant 3 | 31 ms |

La moyenne des trois seuils est d'environ **31,3 ms**.

Les seuils ne sont pas exactement les mêmes pour tout le monde. Certaines personnes remarquent donc le retard plus rapidement que d'autres.

## Conclusion

Le budget d'une image en VR est très contraint. Même si une personne peut parfois tolérer un retard supérieur à 20 ms sur un écran ordinaire, le système VR doit tenir compte de toute la chaîne entre le mouvement réel et l'image affichée.

Les quelques millisecondes disponibles pour l'application doivent donc être utilisées avec beaucoup de précaution afin de limiter la latence globale.

**Observation finale :** cette démonstration montre que la latence n'est pas seulement une valeur technique : elle peut devenir perceptible par l'utilisateur.
