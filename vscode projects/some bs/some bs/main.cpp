#include "main.hpp"


int main(int argc, char** argv) {



    //----GRID VARIABLES----

    //size in pixels of the grids to draw to the window
    int tileSize = 16;

    //size of diamond square map *MUST BE EVEN*
    sf::Vector2f mousePos;
    sf::Vector2i mouseGrid;
    sf::Vector2i viewPos;

    //start position for the view
    viewPos.x = 100;
    viewPos.y = 125;


    //Create the diamond square object and run function to generate map
    DiamSquare ds;
    ds.newMap(dsMap, mapSize);




    //----VIEW SETUP/SFML----

    sf::RenderWindow window; 
    sf::View view;

    game win;  

    //container for the height and width of the window
    sf::Vector2i resPixels;
    resPixels = win.windowSetup(window, view, 400, false,60);

    //how many grids can fit on the screen
    sf::Vector2i resTiles;
    resTiles.x = floor(resPixels.x / tileSize);
    resTiles.y = floor(resPixels.y / tileSize);


    //create the buffers and sprites used to draw the map and gui
    sf::RenderTexture bufferMap;
    sf::RenderTexture bufferGUI;

    bufferMap.create(resPixels.x, resPixels.y);
    bufferGUI.create(resPixels.x, resPixels.y);

    sf::Sprite bufferSpriteMap(bufferMap.getTexture());
    sf::Sprite bufferSpriteGUI(bufferGUI.getTexture());




    //----GLYPHS----

    //load font
    sf::Font fontSmall;
    if (!fontSmall.loadFromFile("../fonts/small_pixel.ttf")) return 1;
    
    //Text for the cursor position and debugging
    sf::Text textSmall;
    textSmall.setFont(fontSmall);
    textSmall.setString("Hello world");
    textSmall.setCharacterSize(8);
    textSmall.setFillColor(sf::Color(c_black.x, c_black.y, c_black.z));
    textSmall.setStyle(sf::Text::Regular);
    textSmall.setPosition(5, 5);

    //rectangles to draw the grids
    sf::RectangleShape rectCursor;
    rectCursor.setSize(sf::Vector2f(tileSize, tileSize));
    rectCursor.setFillColor(sf::Color::Transparent);
    rectCursor.setOutlineColor(sf::Color::Black);
    rectCursor.setOutlineThickness(2);
    rectCursor.setOrigin(0, 0);




    //----DRAW STATIC----


    //clear viewwith a background color
    bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
    //draw the map with vertex array
    ds.drawMap(bufferMap, tileMap2, tileSize, viewPos, resTiles, false, "../sprites/tilesetTransitions.png");




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
            ds.newMap(dsMap, mapSize);
            bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
            ds.drawMap(bufferMap, tileMap2, tileSize, viewPos, resTiles, false, "../sprites/tilesetTransitions.png");
        }




        //move the map around and update the graphics
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            viewPos.x += 1;
            if (viewPos.x > mapSize - resTiles.x) viewPos.x = mapSize - resTiles.x;
            else
            {
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, tileMap2, tileSize, viewPos, resTiles, false, "../sprites/tilesetTransitions.png");
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            viewPos.x -= 1;
            if (viewPos.x < 1) viewPos.x = 1;
            else
            {
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, tileMap2, tileSize, viewPos, resTiles, false, "../sprites/tilesetTransitions.png");
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            viewPos.y -= 1;
            if (viewPos.y < 1) viewPos.y = 1;
            else
            {
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, tileMap2, tileSize, viewPos, resTiles, false, "../sprites/tilesetTransitions.png");
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            viewPos.y += 1;
            if (viewPos.y > mapSize - resTiles.y) viewPos.y = mapSize - resTiles.y;
            else
            {
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, tileMap2, tileSize, viewPos, resTiles, false, "../sprites/tilesetTransitions.png");
            }
        } 
        



        //----UPDATE MAKE CALCULATIONS-----     

        //Create a vector with the pixel coord's in the actual window not the display
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //This is the position of our mouse in world grid coord's but keep it in the map
        mouseGrid.x = round(mousePos.x / tileSize) + viewPos.x;
        mouseGrid.y = round(mousePos.y / tileSize) + viewPos.y;

        if (mouseGrid.x > mapSize-1) mouseGrid.x = mapSize-1;
        if (mouseGrid.y > mapSize-1) mouseGrid.y = mapSize-1;
        if (mouseGrid.x < 1) mouseGrid.x = 1;
        if (mouseGrid.y < 1) mouseGrid.y = 1;




        //----DRAW UPDATE-----
      
        //clear the view transparent so it doesnt cover up the main buffer!
        bufferGUI.clear(sf::Color::Transparent);

        //Draw the coord of the mouse on the screen for debugging
        textSmall.setString("(" + std::to_string(mouseGrid.x) + ", " + std::to_string(mouseGrid.y) + ") : " + std::to_string(dsMap[mouseGrid.x][mouseGrid.y]));
        bufferGUI.draw(textSmall);

        //Move the rectangle to the correct position before drawing
        rectCursor.setPosition((mouseGrid.x-viewPos.x)*tileSize, (mouseGrid.y-viewPos.y)*tileSize);
        bufferGUI.draw(rectCursor);




        //----DISPLAY THE STUFF----
        
        //display the buffer on to the window and draw the sprite 
        //on to the window (not really sure how this works honestly)
        bufferMap.display(); 
        window.draw(bufferSpriteMap);

        //do the same as above but for the GUI
        bufferGUI.display(); 
        window.draw(bufferSpriteGUI);

        // Display the window to the screen
        window.display();
    }



    //----DEBUGGING OUTPUTS----

    std::cout << "Window Resolution:  " << resPixels.x << " X " << resPixels.y << std::endl;
    std::cout << std::endl;

    return 0;
}