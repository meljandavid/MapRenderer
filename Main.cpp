#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include <list>
#include <iostream>
#include <cmath>

struct Building {
    sf::Vector2i pos;
    sf::Color color;
};

int main() {
    std::cout << "Add meg a keresett utak szamat!\nN=";
    int N; std::cin >> N;

    // GENERATE MAP
    std::list<Building> buildings;
    int columns = int(std::log2(N-1)) + 1;
    int upperpow = (1 << (columns+1));

    for (int c = 0; c < columns; c++) {
        for (int y = (1<<c); y <= upperpow; y++) {
            if ( ( (y - (1 << c) ) % (1 << (c + 2) ) ) < (1 << (c + 1) ) ) {
                buildings.push_back({ { 2 * c,y }, sf::Color::Black });
                if (c != 0) buildings.push_back({ { -2 * c,y }, sf::Color::Black });
            }
        }
    }

    int deletable = upperpow/2 - N;
    for (int c = columns - 1; c >= 0; c--) {
        if (deletable >= (1 << c)) {
            deletable -= (1 << c);
            int y = (1 << c) + (1 << (c + 1));
            buildings.push_back({ {-2 * c, y}, sf::Color::Red });
        }
    }

    // INIT TEXTURE
    int TILESIZE = 40;
    if (upperpow > 16) TILESIZE = 20;
    if (upperpow*TILESIZE > 16000) TILESIZE = 15000 / upperpow;
    sf::RenderTexture texture;
    texture.create(TILESIZE * (8*columns), (upperpow+2) * TILESIZE);
    texture.clear(sf::Color::White);

    // INIT TEXT, DISPLAY N
    sf::Font font;
    font.loadFromFile("Assets\\FFFFORWA.ttf");
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString("N=" + std::to_string(N));
    text.setCharacterSize( (texture.getSize().x - 2*TILESIZE*columns) / 2 / text.getString().getSize() );
    text.setPosition(sf::Vector2f( (float)text.getCharacterSize()/4, (float)text.getCharacterSize()/2 ) );
    texture.draw(text);
    
    // DISPLAY BUILDINGS
    sf::RectangleShape rect(sf::Vector2f((float)TILESIZE, (float)TILESIZE));
    for (const Building& b : buildings) {
        rect.setPosition(sf::Vector2f((b.pos.x-.5f)*TILESIZE + texture.getSize().x/2, (b.pos.y+1.f)*TILESIZE));
        rect.setFillColor(b.color);
        texture.draw(rect);
    }

    // DRAW POINTS AND LABELS
    sf::CircleShape circle(float(TILESIZE/4));
    circle.setFillColor(sf::Color::Blue);
    circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
    text.setFillColor(sf::Color(0,255,255));
    text.setCharacterSize(text.getCharacterSize() / 2);
    // START
    circle.setPosition(sf::Vector2f(texture.getSize().x / 2 - 2.f * TILESIZE * columns, texture.getSize().y / 2.f));
    texture.draw(circle);
    text.setString("Start");
    text.setPosition(circle.getPosition() +
        sf::Vector2f(-text.getLocalBounds().width, -text.getLocalBounds().height -10.f));
    texture.draw(text);
    // END
    circle.setPosition(sf::Vector2f(texture.getSize().x / 2 + 2.f * TILESIZE * columns, texture.getSize().y / 2.f));
    texture.draw(circle);
    text.setString("Cel");
    text.setPosition(circle.getPosition() +
        sf::Vector2f( 0.f, -text.getLocalBounds().height-10.f));
    texture.draw(text);

    // SAVE IT
    texture.display();
    std::string filename = "n" + std::to_string(N) + ".jpeg";
    if (texture.getTexture().copyToImage().saveToFile(filename)) {
        std::cout << "A konstrukcio le lett mentve az " << filename << " fajlba\n";
    }

    system("pause");

    // return 0;
}