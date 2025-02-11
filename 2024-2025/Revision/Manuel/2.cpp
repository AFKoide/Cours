#include <iostream>
#include <vector>

// Déclaration de la classe Checkers
class Checkers {
public:
    Checkers(); // Constructeur
    ~Checkers(); // Destructeur
    void putSquare(int x, int y, int color); // Fonction pour placer un pion
    void displayBoard() const; // Affiche le plateau

private:
    std::vector<std::vector<int>> board; // Plateau de jeu 8x8
};

// Implémentation du constructeur : Initialise un plateau 8x8 avec des cases vides (0)
Checkers::Checkers() : board(8, std::vector<int>(8, 0)) {}

// Implémentation du destructeur
Checkers::~Checkers() {}

// Implémentation de la fonction putSquare : place un pion sur le plateau
void Checkers::putSquare(int x, int y, int color) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        board[x][y] = color;
    }
}

// Fonction pour afficher le plateau
void Checkers::displayBoard() const {
    for (const auto& row : board) {
        for (int square : row) {
            std::cout << square << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    Checkers game; // Création d'une instance de Checkers
    
    // Ajout de pions : 1 pour blanc, 2 pour noir
    game.putSquare(2, 3, 1); // Pion blanc
    game.putSquare(5, 4, 1); // Pion blanc
    game.putSquare(3, 2, 2); // Pion noir
    game.putSquare(6, 5, 2); // Pion noir
    
    // Affichage du plateau
    game.displayBoard();
    
    return 0;
}
