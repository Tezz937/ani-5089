# Démonstration 1 — Le budget au tableau

J'ai représenté les 20 ms disponibles pour une image VR sous la forme d'une barre horizontale.

J'ai ensuite placé les cinq étapes dans l'ordre :

1. Capteurs
2. Transmission des mesures
3. Application : simulation et rendu
4. Compositeur
5. Affichage

L'objectif était de montrer que les 20 ms ne sont pas entièrement disponibles pour le code de l'application.

À partir des valeurs observées dans le chapitre, les étapes liées aux capteurs, à la transmission, au compositeur et à l'affichage prennent déjà une partie du budget. Il reste donc seulement quelques millisecondes pour la simulation et le rendu de l'application.

La réaction principale de la classe a été de constater que le temps disponible pour le code est beaucoup plus petit que les 20 ms annoncées au départ.

**Conclusion :** en VR, le budget par image est très limité et un dépassement peut avoir un impact directement perceptible.
