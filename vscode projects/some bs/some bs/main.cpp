#include "main.hpp"

int main(int argc, char** argv) {

    //----GENERAL VARIABLES----
    
    //define some colors cause the default ones are ugly
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


    //----GRID STUFF----

    //size in pixels of the grids to draw to the window
    int gridSize = 4;
    //size of diamond square map *MUST BE EVEN*
    int dsSize = 100;

    int mouseX = 150;
    int mouseY = 150;

    //Create the diamond square object and run function to generate map
    DiamSquare ds;
    ds.newMap(dsSize);


    //----VIEW SETUP/SFML----

    //  //get display resolution and aspect ratio
    float displayWidth = GetSystemMetrics(SM_CXSCREEN);
    float DisplayHeight = GetSystemMetrics(SM_CYSCREEN);
    float aspectRatio = displayWidth / DisplayHeight;

    //create a static window width in pixels and calculate the width based on the aspect ratio
    int bufferH = 450;
    int bufferW = abs(bufferH * aspectRatio);

    //center position of the window (needed for view setup)
    int bufferX = bufferW / 2;
    int bufferY = bufferH / 2;

    //create window with scaled resolution and make it fullscreen
    sf::RenderWindow window(sf::VideoMode(bufferW, bufferH), "Some BS", sf::Style::Fullscreen); 
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

     //create view at center of screen and scaled resolution
    sf::View view(sf::Vector2f(bufferX, bufferY), sf::Vector2f(bufferW, bufferH));
    window.setView(view); 

    //create a buffer and set it to the size of our window (this will be the main buffer)
    sf::RenderTexture buffer;
    buffer.create(bufferW, bufferH);

    //create another buffer for things that will update
    sf::RenderTexture bufferGUI; 
    bufferGUI.create(bufferW, bufferH); 

    //create sprites to draw to for each buffer
    sf::Sprite bufferSpriteGUI(bufferGUI.getTexture());
    sf::Sprite bufferSprite(buffer.getTexture());


    //----GLYPHS----

    //load font
    sf::Font font;
    if (!font.loadFromFile("../fonts/small_pixel.ttf")) return 1;
    
    //Text for the cursor position and debugging
    sf::Text text;
    text.setFont(font);
    text.setString("Hello world");
    text.setCharacterSize(8);
    text.setFillColor(sf::Color(c_black.x, c_black.y, c_black.z));
    text.setStyle(sf::Text::Regular);
    text.setPosition(5, 5);

    //Hexagon for cursor
    sf::CircleShape hex(10, 6);
    hex.setFillColor(sf::Color::Transparent);
    hex.setOutlineColor(sf::Color(c_black.x, c_black.y, c_black.z));
    hex.setOutlineThickness(2);
    hex.setOrigin(10, 10);

    //rectangles to draw the grids
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(gridSize, gridSize));
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::Transparent);
    rect.setOutlineThickness(2);
    rect.setOrigin(0, 0);


    //----DRAW STATIC----

    //clear viewwith a background color
    buffer.clear(sf::Color(c_black.x, c_black.y, c_black.z));

    //iterate throught the diamond square grid and draw squares at different colors
    //for different heights
    for (int i = 0; i < 101; i += 1)
    {
        for (int z = 0; z < 101; z += 1)
        {
            //Check the number from the grid and set a corisponding color
            if (ds.map[i][z] < -10) { rect.setFillColor(sf::Color(c_dkblue.x, c_dkblue.y, c_dkblue.z)); }
            else if (ds.map[i][z] < -5) { rect.setFillColor(sf::Color(c_blue.x, c_blue.y, c_blue.z)); }
            else if (ds.map[i][z] < 0) { rect.setFillColor(sf::Color(c_ltblue.x, c_ltblue.y, c_ltblue.z)); }
            else if (ds.map[i][z] < 5) { rect.setFillColor(sf::Color(c_tan.x, c_tan.y, c_tan.z)); }
            else if (ds.map[i][z] < 10) { rect.setFillColor(sf::Color(c_green.x, c_green.y, c_green.z)); }
            else if (ds.map[i][z] < 15) { rect.setFillColor(sf::Color(c_dkgreen.x, c_dkgreen.y, c_dkgreen.z)); }
            else if (ds.map[i][z] < 20) { rect.setFillColor(sf::Color(c_purple.x, c_purple.y, c_purple.z)); }
            else { rect.setFillColor(sf::Color(c_dkpurple.x, c_dkpurple.y, c_dkpurple.z)); }

            //Move the rectangle to the correct position before drawing
            rect.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
            buffer.draw(rect);
        }
    }

    rect.setFillColor(sf::Color(0,0,0,0));
    rect.setOutlineColor(sf::Color::Black);


    //----MAIN LOOP----

    while (window.isOpen()) {

        //WINDOW EVENTS
        //event handler
        sf::Event event; 
        while (window.pollEvent(event)) {

            switch (event.type)
            {

            //if the window was closed useing the x in the top right
            case sf::Event::Closed:
                window.close();
                break;

            //if the escape key is pressed
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) { window.close(); }
                break;
            }

        }


        //----UPDATE----

        //Create a vector with the pixel coord's in the actual window not the display
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //Update the positon of the hex (this is the cursor for now)
        hex.setPosition(mousePos.x + 3, mousePos.y + 3); 

        //This is the position of our mouse in world grid coord's
        mouseX = round(mousePos.x / gridSize);
        mouseY = round(mousePos.y / gridSize);

        //----DRAW UPDATE-----
      
        //clear the view transparent so it doesnt cover up the main buffer!
        bufferGUI.clear(sf::Color(0,0,0,0));

        //Draw the cursor on the screen
        bufferGUI.draw(hex);

        //Draw the coord of the mouse on the screen
        float inView = 0;
        if(mouseX >= 0 && mouseX <= 101 && mouseY >= 0 && mouseY <= 101) inView = ds.map[mouseX][mouseY];
        text.setString("(" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ") : " + std::to_string(inView));
        bufferGUI.draw(text);

        //Move the rectangle to the correct position before drawing
        rect.setPosition(mouseX*gridSize+1, mouseY*gridSize+1);
        bufferGUI.draw(rect);

        //display the buffer on to the window and draw the sprite 
        //on to the window (not really sure how this works honestly)
        buffer.display(); 
        window.draw(bufferSprite);

        //do the same as above but for the GUI
        bufferGUI.display(); 
        window.draw(bufferSpriteGUI);

        window.display();// Display the results
    }


    //----DEBUGGING OUTPUTS----

    std::cout << "Monitor Resolution: " << DisplayHeight << " X " << displayWidth << std::endl;
    std::cout << "Window Resolution:  " << bufferH << " X " << bufferW << std::endl;
    std::cout << "Aspect Ratio: " << aspectRatio << std::endl;
    std::cout << std::endl;

    return 0;
}