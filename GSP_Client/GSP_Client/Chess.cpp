#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Pawn.h"

int main()
{
    std::unique_ptr<Board> board{ std::make_unique<Board>() };

    std::string playerName;
    std::cout << "Enter your name : ";
    std::cin >> playerName;

    sf::Font font;
    font.loadFromFile("./Assets/Font/D2Coding.ttc");

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess_Client");

    Pawn pawn("./Assets/Image/pawn.png", sf::Vector2f(100, 100), playerName, font);
 //------------------------------------------------------------------------------------//
 //------------------------------------------------------------------------------------//
    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f currentPosition;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) pawn.move(event.key.code);
        }
        window.clear();
        board->draw(window);
        pawn.draw(window);

        window.display();
    }
}