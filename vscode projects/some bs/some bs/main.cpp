#include "main.hpp"


int main(int argc, char** argv) {

    //----GENERAL VARIABLES----
    



    //----GRID STUFF----

    //size in pixels of the grids to draw to the window
    int gridSize = 4;
    //size of diamond square map *MUST BE EVEN*

    int mouseX = 150;
    int mouseY = 150;

    sf::Vector2i viewPos;

    viewPos.x = 100;
    viewPos.y = 125;

    //Create the diamond square object and run function to generate map
    DiamSquare ds;
    ds.newMap(map, mapSize);





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
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    rect.setOrigin(0, 0);





    //----DRAW STATIC----

    //how many grids can fit on the screen
    int viewGridW = floor(bufferW / gridSize);
    int viewGridH = floor(bufferH / gridSize);
    int viewGridX = viewPos.x - floor((viewGridW)/2);
    int viewGridY = viewPos.y - floor((viewGridH)/2);

    sf::Vector2u tileSize(gridSize, gridSize);

    //clear viewwith a background color
    buffer.clear(sf::Color(c_black.x, c_black.y, c_black.z));

    ds.load(buffer, map, tileSize,viewPos, viewGridW, viewGridH);






    //----MAIN LOOP----

    while (window.isOpen()) {

        //----WINDOW EVENTS----

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

        // regenerate the map when the enter button is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            ds.newMap(map, mapSize);
            ds.load(buffer, map, tileSize, viewPos, viewGridW, viewGridH);
        }

        //move the map around and update the graphics
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            viewPos.x += 1;
            if (viewPos.x > mapSize - floor((viewGridW) / 2)) viewPos.x = mapSize - floor((viewGridW) / 2);
            else
            {
                ds.load(buffer, map, tileSize, viewPos, viewGridW, viewGridH);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            viewPos.x -= 1;
            if (viewPos.x < floor((viewGridW) / 2)+1) viewPos.x = floor((viewGridW) / 2) + 1;
            else
            {
                ds.load(buffer, map, tileSize, viewPos, viewGridW, viewGridH);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            viewPos.y -= 1;
            if (viewPos.y < floor((viewGridH) / 2)+1) viewPos.y = floor((viewGridH) / 2) + 1;
            else
            {
                ds.load(buffer, map, tileSize, viewPos, viewGridW, viewGridH);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            viewPos.y += 1;
            if (viewPos.y > mapSize - floor((viewGridH) / 2)) viewPos.y = mapSize - floor((viewGridH) / 2);
            else
            {
                ds.load(buffer, map, tileSize, viewPos, viewGridW, viewGridH);
            }
        } 
        



        //update the map position to give accurate grid positions
        int viewGridX = viewPos.x - floor((viewGridW) / 2);
        int viewGridY = viewPos.y - floor((viewGridH) / 2);

        //Create a vector with the pixel coord's in the actual window not the display
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //Update the positon of the hex (this is the cursor for now)
        hex.setPosition(mousePos.x + 3, mousePos.y + 3); 

        //This is the position of our mouse in world grid coord's but keep it in the map
        mouseX = round(mousePos.x / gridSize);
        mouseY = round(mousePos.y / gridSize);
        if (mouseX > mapSize) mouseX = mapSize;
        if (mouseY > mapSize) mouseY = mapSize;
        if (mouseX < 0) mouseX = 0;
        if (mouseY < 0) mouseY = 0;




        //----DRAW UPDATE-----
      
        //clear the view transparent so it doesnt cover up the main buffer!
        bufferGUI.clear(sf::Color(0,0,0,0));

        //Draw the cursor on the screen
        bufferGUI.draw(hex);

        //Draw the coord of the mouse on the screen as long as its in the map  
        int newX = mouseX + viewGridX;
        int newY = mouseY + viewGridY;
        text.setString("(" + std::to_string(newX) + ", " + std::to_string(newY) + ") : " + std::to_string(map[newX][newY]));
        bufferGUI.draw(text);

        //Move the rectangle to the correct position before drawing
        rect.setPosition(mouseX*gridSize, mouseY*gridSize);
        bufferGUI.draw(rect);





        //----DISPLAY THE STUFF----
        
        //display the buffer on to the window and draw the sprite 
        //on to the window (not really sure how this works honestly)
        buffer.display(); 
        window.draw(bufferSprite);

        //do the same as above but for the GUI
        bufferGUI.display(); 
        window.draw(bufferSpriteGUI);

        // Display the window to the screen
        window.display();
    }


    //----DEBUGGING OUTPUTS----

    std::cout << "Monitor Resolution: " << DisplayHeight << " X " << displayWidth << std::endl;
    std::cout << "Window Resolution:  " << bufferH << " X " << bufferW << std::endl;
    std::cout << "Aspect Ratio: " << aspectRatio << std::endl;
    std::cout << std::endl;

    return 0;
}