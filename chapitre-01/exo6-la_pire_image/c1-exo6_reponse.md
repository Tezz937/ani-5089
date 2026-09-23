# Exercice 6 — La pire image

J'ai utilisé un programme très simple qui efface l'écran en boucle et j'ai observé 1000 images.

- Plus longue image mesurée : **14,6 ms**
- Nombre d'images au-dessus de 11 ms : **37**

Le programme ne tiendrait donc pas correctement une cadence de 90 Hz dans un casque. À 90 Hz, le budget d'une image est d'environ 11,1 ms, et certaines images dépassent clairement cette limite. Même si la moyenne peut sembler correcte, les images les plus longues peuvent provoquer des saccades perceptibles.
