#include "main.h"
#include <random>
//link all necessary code from the included packages (mostly SFML)


int main(int argc, char** argv) {


    sf::Vector3i c_black(13, 14, 26);
    sf::Vector3i c_dkblue(33, 47, 106);
    sf::Vector3i c_blue(47, 80, 118);
    sf::Vector3i c_ltblue(70, 113, 128);
    sf::Vector3i c_tan(116, 113, 89);
    sf::Vector3i c_green(49, 83, 76);
    sf::Vector3i c_dkgreen(34, 53, 59);
    sf::Vector3i c_dkpurple(43, 37, 49);
    sf::Vector3i c_purple(77, 61, 85);
    sf::Vector3i c_snow(182, 182, 182);

    game res; //this is defined in the header file
    float resW = res.GetResW();
    float resH = res.GetResH();

    float resRatio = resW / resH; //this is the aspect ratio

    int bufferH = 450; //Width of the new view scaled down for a pixel look
    int bufferW = abs(bufferH * resRatio); //the width will always be static and the height is determined with the aspect ratio

    int bufferX = bufferW / 2; //center of the buffer texture for positioning the game view
    int bufferY = bufferH / 2;

    int gridSize = 4;
    int gridCountW = round(bufferW / gridSize) + 1;
    int gridCountH = round(bufferH / gridSize) + 1;

    const int mapSize = 100;

    int playerX = 150;
    int playerY = 150;

    DiamSquare ds;
    ds.newMap(8);


    std::string tileTypes[4] = { "Tree","Plain","Mntn","Hill" };

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 3); // define the range

    struct tile {
        std::string tileType;
        float moisture;
        float height;
    };

    sf::Clock timer; //timer to rotate the hex
    sf::Vector3i col1(50, 104, 240); //we can use a vector to define a color for the background!



    sf::RenderWindow window(sf::VideoMode(bufferW, bufferH), "Scaling"); //create the winow at default resolution and make fullscreen

    sf::View view(sf::Vector2f(bufferX, bufferY), sf::Vector2f(bufferW, bufferH));// create a view and give it the scaled resolution
    window.setMouseCursorVisible(false);
    window.setView(view); //set the view of the window to the view we just created

    sf::RenderTexture buffer; //create the main window texture to draw in the proper resolution (needs to be drawn to the buffer sprite and drawn to the window)
    buffer.create(bufferW, bufferH); //create the buffer texter with the proper resolution
    buffer.setSmooth(false);

    sf::Sprite bufferSprite(buffer.getTexture()); //create the sprite that the buffer texture will be drawn to

    window.setFramerateLimit(10); //set the frame rate



    sf::Font font;
    sf::Font font1;
    sf::Font font2;
    sf::Font font3;
    if (!font.loadFromFile("../fonts/DisposableDroidBB.ttf")) return 1;
    if (!font1.loadFromFile("../fonts/manaspc.ttf")) return 1;
    if (!font2.loadFromFile("../fonts/novem___.ttf")) return 1;
    if (!font3.loadFromFile("../fonts/small_pixel.ttf")) return 1;



    sf::Text text;
    text.setFont(font3);
    text.setString("Hello world");
    text.setCharacterSize(8);
    text.setFillColor(sf::Color(c_black.x, c_black.y, c_black.z));
    text.setStyle(sf::Text::Regular);
    text.setPosition(20, 20);

    sf::CircleShape hex(10, 6);
    hex.setFillColor(sf::Color::Transparent);
    hex.setOutlineColor(sf::Color(col1.x, col1.y, col1.z));
    hex.setOutlineThickness(2);
    hex.setOrigin(10, 10);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(gridSize, gridSize));
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
                if (event.key.code == sf::Keyboard::Escape) { window.close(); }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { playerX += 1; } //this does the same thing as above
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { playerX -= 1; }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { playerY += 1; }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { playerY -= 1; }


        //UPDATE SHIT
        //create a vector with the mouse position in the window (mapPixelToCoords converts the screen location to the location in the view)
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        hex.setPosition(mousePos.x+gridSize, mousePos.y+gridSize); //update the hex to the mouse vector position created above
        hex.setRotation(timer.getElapsedTime().asMilliseconds() / 10);

        //DRAW SHIT
        // Clear the buffer texture to create a background
        buffer.clear(sf::Color(col1.x, col1.y, col1.z));

        playerX = round(mousePos.x / gridSize);
        playerY = round(mousePos.y / gridSize);

        for (int i = 0; i < 100; i += 1)
        {
            for (int z = 0; z < 100; z += 1)
            {
                if (ds.map[i][z] < -10) { rect.setFillColor(sf::Color(c_dkblue.x, c_dkblue.y, c_dkblue.z)); }
                else if (ds.map[i][z] < -5) { rect.setFillColor(sf::Color(c_blue.x, c_blue.y, c_blue.z)); }
                else if (ds.map[i][z] < 0) { rect.setFillColor(sf::Color(c_ltblue.x, c_ltblue.y, c_ltblue.z)); }
                else if (ds.map[i][z] < 5) { rect.setFillColor(sf::Color(c_tan.x, c_tan.y, c_tan.z)); }
                else if (ds.map[i][z] < 10) { rect.setFillColor(sf::Color(c_green.x, c_green.y, c_green.z)); }
                else if (ds.map[i][z] < 15) { rect.setFillColor(sf::Color(c_dkgreen.x, c_dkgreen.y, c_dkgreen.z)); }
                else if (ds.map[i][z] < 20) { rect.setFillColor(sf::Color(c_purple.x, c_purple.y, c_purple.z)); }
                else { rect.setFillColor(sf::Color(c_dkpurple.x, c_dkpurple.y, c_dkpurple.z)); }

                if (playerX == i && playerY == z) { rect.setFillColor(sf::Color(c_black.x, c_black.y, c_black.z)); }

                rect.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
                buffer.draw(rect);

            }
        }

        buffer.draw(hex);
        float val = 0;
        if(playerX >= 0 && playerX <= 65 && playerY >= 0 && playerY <= 65) val = ds.map[playerX][playerY];
        text.setString("(" + std::to_string(playerX) + ", " + std::to_string(playerY) + ") : " + std::to_string(val));
        buffer.draw(text);
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