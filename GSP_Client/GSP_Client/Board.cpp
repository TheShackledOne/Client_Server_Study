#include "Board.h"

Board::Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            squares_[i * 8 + j].setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares_[i * 8 + j].setPosition(j * SQUARE_SIZE, i * SQUARE_SIZE);
            if ((i + j) & 1) squares_[i * 8 + j].setFillColor(sf::Color::White);
            else squares_[i * 8 + j].setFillColor(sf::Color::Black);
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    for (const auto& square : squares_)
        window.draw(square);
}