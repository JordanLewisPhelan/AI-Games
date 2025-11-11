#pragma once
#include <SFML/Graphics.hpp>



enum class PieceType { None, Donkey, Snake, Frog };
enum class Player { None, Player1, Player2 };

struct Piece {
    PieceType type;
    Player owner;

    Piece() : type(PieceType::None), owner(Player::None) {}
    Piece(PieceType t, Player p) : type(t), owner(p) {}

    bool isEmpty() const { return type == PieceType::None; }
};

// For UI - represents pieces available to place
struct UnplacedPiece {
    PieceType type;
    int count;
    sf::RectangleShape shape;
    sf::Text label;

    UnplacedPiece(sf::Font& t_font)
        : type(PieceType::None), count(0), label(t_font) {}
    UnplacedPiece(PieceType t, int c, sf::Font& t_font)
        : type(t), count(c), label(t_font) {}
};