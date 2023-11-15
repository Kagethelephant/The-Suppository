#include "main.hpp"
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

    float resW = GetSystemMetrics(SM_CXSCREEN);
    float resH = GetSystemMetrics(SM_CYSCREEN);

    float resRatio = resW / resH; //this is the aspect ratio

    int bufferH = 450; //Width of the new view scaled down for a pixel look
    int bufferW = abs(bufferH * resRatio); //the width will always be static and the height is determined with the aspect ratio

    int bufferX = bufferW / 2; //center of the buffer texture for positioning the game view
    int bufferY = bufferH / 2;

    int gridSize = 4;
    const int mapSize = 100;

    int mouseX = 150;
    int mouseY = 150;

    DiamSquare ds;
    ds.newMap(50);

    //VIEW SETUP/SFML
    sf::RenderWindow window(sf::VideoMode(bufferW, bufferH), "Scaling", sf::Style::Fullscreen); //create the winow at default resolution and make fullscreen

    sf::View view(sf::Vector2f(bufferX, bufferY), sf::Vector2f(bufferW, bufferH));// create a view and give it the scaled resolution
    window.setMouseCursorVisible(false);
    window.setView(view); //set the view of the window to the view we just created

    sf::RenderTexture buffer; //create the main window texture to draw in the proper resolution (needs to be drawn to the buffer sprite and drawn to the window)
    buffer.create(bufferW, bufferH); //create the buffer texter with the proper resolution

    sf::RenderTexture bufferGUI; //create the main window texture to draw in the proper resolution (needs to be drawn to the buffer sprite and drawn to the window)
    bufferGUI.create(bufferW, bufferH); //create the buffer texter with the proper resolution
    sf::Sprite bufferSpriteGUI(bufferGUI.getTexture());

    sf::Sprite bufferSprite(buffer.getTexture()); //create the sprite that the buffer texture will be drawn to

    window.setFramerateLimit(60); //set the frame rate


    //GLYPHS
    sf::Font font;
    if (!font.loadFromFile("../fonts/small_pixel.ttf")) return 1;

    sf::Text text;
    text.setFont(font);
    text.setString("Hello world");
    text.setCharacterSize(8);
    text.setFillColor(sf::Color(c_black.x, c_black.y, c_black.z));
    text.setStyle(sf::Text::Regular);
    text.setPosition(400, 20);

    sf::CircleShape hex(10, 6);
    hex.setFillColor(sf::Color::Transparent);
    hex.setOutlineColor(sf::Color(c_black.x, c_black.y, c_black.z));
    hex.setOutlineThickness(2);
    hex.setOrigin(10, 10);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(gridSize, gridSize));
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::Transparent);
    rect.setOutlineThickness(2);
    rect.setOrigin(0, 0);

    //DRAW STATIC
    //Try to put costly drawing commands here that do not need to be updated
    buffer.clear(sf::Color(c_black.x, c_black.y, c_black.z));

    for (int i = 0; i < 101; i += 1)
    {
        for (int z = 0; z < 101; z += 1)
        {
            if (ds.map[i][z] < -10) { rect.setFillColor(sf::Color(c_dkblue.x, c_dkblue.y, c_dkblue.z)); }
            else if (ds.map[i][z] < -5) { rect.setFillColor(sf::Color(c_blue.x, c_blue.y, c_blue.z)); }
            else if (ds.map[i][z] < 0) { rect.setFillColor(sf::Color(c_ltblue.x, c_ltblue.y, c_ltblue.z)); }
            else if (ds.map[i][z] < 5) { rect.setFillColor(sf::Color(c_tan.x, c_tan.y, c_tan.z)); }
            else if (ds.map[i][z] < 10) { rect.setFillColor(sf::Color(c_green.x, c_green.y, c_green.z)); }
            else if (ds.map[i][z] < 15) { rect.setFillColor(sf::Color(c_dkgreen.x, c_dkgreen.y, c_dkgreen.z)); }
            else if (ds.map[i][z] < 20) { rect.setFillColor(sf::Color(c_purple.x, c_purple.y, c_purple.z)); }
            else { rect.setFillColor(sf::Color(c_dkpurple.x, c_dkpurple.y, c_dkpurple.z)); }

            if (mouseX == i && mouseY == z) { rect.setFillColor(sf::Color(c_black.x, c_black.y, c_black.z)); }

            rect.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
            buffer.draw(rect);
        }
    }

    //MAIN LOOP
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) { window.close(); }
            }
        }

        //UPDATE
        //Create a vector with the pixel coord's in the actual window not the display
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //Update the positon of the hex (this is the cursor for now)
        hex.setPosition(mousePos.x + gridSize, mousePos.y + gridSize); 

        //This is the position of our mouse in world grid coord's
        mouseX = round(mousePos.x / gridSize);
        mouseY = round(mousePos.y / gridSize);

        //DRAW UPDATE
        //clear the view transparent so it doesnt cover up our map!
        bufferGUI.clear(sf::Color(0,0,0,0));
        bufferGUI.draw(hex);

        //Draw the coord
        float inView = 0;
        if(mouseX >= 0 && mouseX <= 101 && mouseY >= 0 && mouseY <= 101) inView = ds.map[mouseX][mouseY];
        text.setString("(" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ") : " + std::to_string(inView));
        bufferGUI.draw(text);

        buffer.display(); //send the texture from the back buffer to the screen
        window.draw(bufferSprite);// Draw the render texture's contents

        bufferGUI.display(); //send the texture from the back buffer to the screen
        window.draw(bufferSpriteGUI);// Draw the render texture's contents

        window.display();// Display the results
    }


    //DEBUGGING OUTPUTS
    std::cout << "Monitor Resolution: " << resH << " X " << resW << std::endl;
    std::cout << "Window Resolution:  " << bufferH << " X " << bufferW << std::endl;
    std::cout << "Aspect Ratio: " << resRatio << std::endl;
    std::cout << std::endl;

    return 0;
}