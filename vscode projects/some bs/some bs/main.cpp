#include "main.h"
#include <random>
//link all necessary code from the included packages (mostly SFML)


int main(int argc, char** argv) {

    game res; //this is defined in the header file
    float resW = res.GetResW();
    float resH = res.GetResH();

    float resRatio = resW / resH; //this is the aspect ratio

    int bufferH = 400; //Width of the new view scaled down for a pixel look
    int bufferW = abs(bufferH * resRatio); //the width will always be static and the height is determined with the aspect ratio

    int bufferX = bufferW / 2; //center of the buffer texture for positioning the game view
    int bufferY = bufferH / 2;

    int gridSize = 32;
    int gridCountW = round(bufferW / gridSize) + 1;
    int gridCountH = round(bufferH / gridSize) + 1;

    const int mapSize = 100;

    int playerX = 150;
    int playerY = 150;

    std::string tileTypes[4] = { "Tree","Plain","Mntn","Hill" };

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 3); // define the range

    struct tile {
        std::string tileType;
        float moisture;
        float height;
    };

    tile mapGrid[mapSize][mapSize];

    for (int i = 0; i < mapSize; i += 1)
    {
        for (int z = 0; z < mapSize; z += 1)
        {
            mapGrid[i][z].tileType = "3test";
            mapGrid[i][z].moisture = 1;
            mapGrid[i][z].height = 1;
        }
    }



    sf::Clock timer; //timer to rotate the hex
    sf::Vector3i col1(50, 104, 240); //we can use a vector to define a color for the background!



    sf::RenderWindow window(sf::VideoMode(), "Scaling", sf::Style::Fullscreen); //create the winow at default resolution and make fullscreen

    sf::View view(sf::Vector2f(bufferX, bufferY), sf::Vector2f(bufferW, bufferH));// create a view and give it the scaled resolution
    window.setView(view); //set the view of the window to the view we just created

    sf::RenderTexture buffer; //create the main window texture to draw in the proper resolution (needs to be drawn to the buffer sprite and drawn to the window)
    buffer.create(bufferW, bufferH); //create the buffer texter with the proper resolution

    sf::Sprite bufferSprite(buffer.getTexture()); //create the sprite that the buffer texture will be drawn to

    window.setFramerateLimit(60); //set the frame rate


    sf::Font font;
    if (!font.loadFromFile("../fonts/DisposableDroidBB.ttf")) return 1;

    sf::Text text;
    text.setFont(font);
    text.setString("Hello world");
    text.setCharacterSize((96 / 72) * 12);
    text.setFillColor(sf::Color(col1.x, col1.y, col1.z));
    text.setStyle(sf::Text::Regular);

    sf::CircleShape hex(20, 6);
    hex.setFillColor(sf::Color::Transparent);
    hex.setOutlineColor(sf::Color(col1.x, col1.y, col1.z));
    hex.setOutlineThickness(2);
    hex.setOrigin(20, 20);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(gridSize - 1, gridSize - 1));
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::Transparent);
    rect.setOutlineThickness(2);
    rect.setOrigin(0, 0);

    //Main while loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close();} //this does the same thing as above

        //UPDATE SHIT
        //create a vector with the mouse position in the window (mapPixelToCoords converts the screen location to the location in the view)
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        hex.setPosition(mousePos.x, mousePos.y); //update the hex to the mouse vector position created above

        // Rotate the hex
        hex.setRotation(timer.getElapsedTime().asMilliseconds() / 10);

        //DRAW SHIT
        // Clear the buffer texture to create a background
        buffer.clear(sf::Color(col1.x, col1.y, col1.z));

        for (int i = 0; i < gridCountW; i += 1)
        {
            for (int z = 0; z < gridCountH; z += 1)
            {
                rect.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
                buffer.draw(rect);
                text.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
                text.setString(mapGrid[i][z].tileType);
                buffer.draw(text);
            }
        }

        buffer.draw(hex);

        buffer.display(); //send the texture from the back buffer to the screen
        window.draw(bufferSprite);// Draw the render texture's contents
        window.display();// Display the results
    }
    //some outputs to the console after the while loop closes for debugging
    std::cout << "Monitor Resolution: " << resH << " X " << resW << std::endl;
    std::cout << "Window Resolution:  " << bufferH << " X " << bufferW << std::endl;
    std::cout << "Aspect Ratio: " << resRatio << std::endl;
    std::cout << std::endl;

    return 0;
}