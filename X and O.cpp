#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <ostream>
#include <time.h>
#include <sstream>

using namespace sf;
using namespace std;

int checkIfClicked(const int squaresH, const int squaresV, const int squareSize, Vector2i pos)
{
    for (int k(0); k < squaresH * squaresV; k++)
    {
        if (pos.x >= k % squaresH * squareSize + k % squaresH * 10 && pos.x <= k % squaresH * squareSize + k % squaresH * 10 + squareSize && pos.y >= k / squaresH * squareSize + k / squaresH * 10 && pos.y <= k / squaresH * squareSize + k / squaresH * 10 + squareSize)
        {
            return k;
        }
    }
    return -1;
}

bool gain(const int squareH, int table[])
{

    //horizontal
    if (table[0] == table[1] && table[0] == table[2] && table[0] != -1)return true;
    else if (table[squareH] == table[squareH + 1] && table[squareH] == table[squareH + 2] && table[squareH] != -1)return true;
    else if (table[squareH * 2] == table[squareH * 2 + 1] && table[squareH * 2] == table[squareH * 2 + 2] && table[squareH * 2] != -1)return true;

    //vertical
    else if (table[0] == table[squareH] && table[0] == table[squareH * 2] && table[0] != -1)return true;
    else if (table[1] == table[squareH + 1] && table[1] == table[squareH * 2 + 1] && table[1] != -1)return true;
    else if(table[2] == table[squareH + 2] && table[2] == table[squareH * 2 + 2] && table[2] != -1)return true;
    //x
    if (table[0] == table[squareH + 1] && table[0] == table[squareH * 2 + 2] && table[0] != -1)return true;
    if (table[2] == table[squareH + 1] && table[2] == table[squareH * 2] && table[2] != -1)return true;

    return false;
}

const int width(920), height(920);
const int squareSize(300);
const int squareH(width / squareSize);
const int squareV(height / squareSize);

int main()
{
    const int space(10);
    const int allSquares = squareH * squareV;
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(width, height), "X & O");

    Sprite* rectangles = new Sprite[allSquares];
    Sprite* tablex = new Sprite[allSquares];
    Sprite* tablez = new Sprite[allSquares];
    Texture zeroTexture, xTexture, squareTexture;
    zeroTexture.loadFromFile("images/zero.png");
    xTexture.loadFromFile("images/x.png");
    squareTexture.loadFromFile("images/square.png");

    for (int k(0); k < allSquares; k++)
    {
        rectangles[k].setTexture(squareTexture);
        tablex[k].setTexture(xTexture);
        tablez[k].setTexture(zeroTexture);
    }

    vector<int> positions;

    Clock clock;

    bool xPlayer = true;
    bool finish = false;
    bool restart = false;

    int* table = new int[allSquares];

    for (int k(0); k < allSquares; k++)table[k] = -1;

    float timer(0), delay(1);

    while (window.isOpen())
    {

        Vector2i pos = Mouse::getPosition(window);
        float time = clock.getElapsedTime().asSeconds();
        timer += time;
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        const int position = checkIfClicked(squareH, squareV, squareSize, pos);
      
        if (Keyboard::isKeyPressed(Keyboard::R))restart = true, finish = false;

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (xPlayer && table[position] < 0 && !finish)
                {
                    table[position] = 1;
                    tablex[position].setPosition(position % squareH * squareSize + position % squareH * 10, position / squareH * squareSize + (position / squareH) * 10);
                    xPlayer = false;              
                    if (gain(squareH, table))
                    {
                        cout << "X player win : " << endl;
                        finish = true;
                    }

                }
            }
        }
        if (restart)
        {
            for (int k(0); k < allSquares; k++)table[k] = -1;
            restart = false;
        }  

        if (!xPlayer && timer > delay && !finish)
        {

            for (int k(0); k < allSquares; k++)
            {
                if (table[k] == -1)positions.push_back(k);
            }
            
            int computerPosition = 0;
            if (positions.size() > 0)computerPosition = positions[rand() % positions.size()];
            while (positions.size() != 0)positions.pop_back();

            tablez[computerPosition].setPosition(computerPosition % squareH * squareSize + computerPosition % squareH * 10, computerPosition / squareH * squareSize + (computerPosition / squareH) * 10);
            if (table[computerPosition] == -1)table[computerPosition] = 2;
            xPlayer = true;
            if (gain(squareH, table))
            {
                cout << "O player win : " << endl;
                finish = true;
            }

            timer = 0;
        }

        window.clear();
        for (int k(0); k < allSquares; k++)
        {
            if (k % squareH == 0)rectangles[k].setPosition(k % squareH * squareSize, k / squareH * squareSize);
            if (k % squareH != 0)rectangles[k].setPosition(k % squareH * squareSize + k % squareH * 10, k / squareH * squareSize);
            if (k / squareH > 0)rectangles[k].setPosition(k % squareH * squareSize + k % squareH * 10, k / squareH * squareSize + (k / squareH) * 10);

            window.draw(rectangles[k]);
            if (table[k] == 2)window.draw(tablez[k]);
            if (table[k] == 1)window.draw(tablex[k]);
        }

      
        window.display();
    }

    return 0;
}