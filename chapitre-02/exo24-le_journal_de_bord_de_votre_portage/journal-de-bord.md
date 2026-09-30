# Journal de bord de mon portage

Symptôme : La commande `clang++` n'était pas reconnue correctement ou Jenga ne trouvait pas le compilateur nécessaire pour construire le projet.
J'ai cru : Que le compilateur Clang n'était pas installé sur mon ordinateur.
C'était : Clang était bien installé, mais le chemin vers le compilateur n'était pas correctement pris en compte dans l'environnement utilisé. Il fallait utiliser le bon environnement et vérifier le `PATH`.
Temps perdu : 6

Symptôme : Le programme ne compilait pas lorsque j'utilisais `iostream` dans mon fichier source.
J'ai cru : Que le problème venait du code C++ ou d'une erreur dans l'utilisation de `iostream`.
C'était : Le problème était lié à l'environnement de compilation et au choix du toolchain utilisé par Jenga. Une fois le bon environnement Clang/MinGW utilisé, la compilation fonctionnait correctement.
Temps perdu : 10

Symptôme : La commande Git ne fonctionnait pas lorsque j'essayais d'ajouter le dossier de l'exercice.
J'ai cru : Que Git avait un problème avec le dossier ou que les fichiers de l'exercice n'étaient pas correctement créés.
C'était : Le problème venait simplement du chemin utilisé avec `git add`. J'étais déjà placé dans un sous-dossier et je répétais le chemin `chapitre-02/...`, ce qui faisait chercher à Git un dossier qui n'existait pas à cet emplacement.
Temps perdu : 10

Symptôme : Le projet Jenga ne se construisait pas avec le premier compilateur Clang trouvé sur la machine.
J'ai cru : Que Jenga ou le fichier de projet était mal configuré.
C'était : Le premier Clang trouvé utilisait l'environnement Windows/MSVC et la liaison échouait à cause de bibliothèques manquantes. Le problème venait donc du toolchain utilisé. En utilisant le Clang de MSYS2 UCRT64 avec le toolchain `clang-mingw`, la construction a réussi.
Temps perdu : 3

Symptôme : Je ne trouvais pas `gdb` ou les commandes permettant de l'installer dans le terminal utilisé pour l'exercice de plantage.
J'ai cru : Que GDB n'était pas disponible sur mon ordinateur.
C'était : J'utilisais Git Bash classique, dans lequel la commande `pacman` n'est pas disponible. GDB devait être utilisé depuis l'environnement dans lequel il était effectivement installé et fonctionnel.
Temps perdu : 4
