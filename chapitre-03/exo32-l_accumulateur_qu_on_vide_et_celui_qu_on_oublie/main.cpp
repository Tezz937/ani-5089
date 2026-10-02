#include <iostream>
#include <string>
using namespace std;

int main() {
    int n;
    cin >> n;

    int total_x = 0, total_y = 0;
    int moteur_x = 0, moteur_y = 0;
    string commande;

    for (int i = 0; i < n; ++i) {
        cin >> commande;

        if (commande == "bouge") {
            int dx, dy;
            cin >> dx >> dy;
            total_x += dx;
            total_y += dy;
            moteur_x = dx;
            moteur_y = dy;
        } else if (commande == "image") {
            cout << total_x << ' ' << total_y << ' ' << moteur_x << ' ' << moteur_y << '\n';
            total_x = 0;
            total_y = 0;
        }
    }

    return 0;
}
