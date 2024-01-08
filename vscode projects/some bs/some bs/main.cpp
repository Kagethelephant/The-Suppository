#pragma once

#include "main.hpp"



//create the grid here so it stays in the heap

int main() {

    static const int mapSize = 200;

   
    //----GRID VARIABLES----

    //size in pixels of the grids to draw to the window
    sf::Vector2i tileSize;
    tileSize.x = 32;
    tileSize.y = 16;

    //size of diamond square map *MUST BE EVEN*
    sf::Vector2f mousePos;
    sf::Vector2i mouseGrid;
    sf::Vector2i viewPos;

    //start position for the view
    viewPos.x = 45;
    viewPos.y = -5;


    //Create the diamond square object and run function to generate map
    Map ds(mapSize);
    ds.newMap();
    ds.sortMapValue();





    //----VIEW SETUP/SFML----

    //only window view for the game
    sf::RenderWindow window; 
    sf::View view;

    //container for the height and width of the window
    sf::Vector2i resPixels;
    resPixels = windowSetup(window, view, 500, true,30);

    //how many grids can fit on the screen
    sf::Vector2i resTiles;
    resTiles.x = ceil(resPixels.x / tileSize.x);
    resTiles.y = ceil(resPixels.y / tileSize.y)*2;


    //create the buffers and sprites used to draw the map and gui
    sf::RenderTexture bufferMap;
    sf::RenderTexture bufferGUI;

    bufferMap.create(resPixels.x, resPixels.y);
    bufferGUI.create(resPixels.x, resPixels.y);





    //----GLYPHS----

    //load font
    sf::Font fontSmall;
    if (!fontSmall.loadFromFile("../fonts/small_pixel.ttf")) return 1;
    
    //Text for the cursor position and debugging
    sf::Text textSmall;
    textSmall.setFont(fontSmall);
    textSmall.setString("Hello world");
    textSmall.setCharacterSize(8);
    textSmall.setFillColor(sf::Color(G_white_x, G_white_y, G_white_z));
    textSmall.setStyle(sf::Text::Regular);
    textSmall.setPosition(5, 5);

    //rectangles to draw the grids
    sf::RectangleShape rectCursor;
    rectCursor.setSize(sf::Vector2f(3,3));
    rectCursor.setFillColor(sf::Color(G_white_x, G_white_y, G_white_z));
    rectCursor.setOutlineColor(sf::Color::Transparent);
    rectCursor.setOutlineThickness(2);
    rectCursor.setOrigin(1, 1);

    sf::Texture selectionT;
    selectionT.loadFromFile("../sprites/selection.png");
    sf::Sprite selectionS;
    selectionS.setTexture(selectionT);
    selectionS.setOrigin(0,tileSize.y / 2);


    //----DRAW STATIC----





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
            ds.newMap();
            //ds.sortMapValue();
        }

        //move the map around and update the graphics
        viewPos.x += sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        viewPos.y += sf::Keyboard::isKeyPressed(sf::Keyboard::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Up);


      


        //----UPDATE MAKE CALCULATIONS-----     

        //Create a vector with the pixel coord's in the actual window not the display
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //This is the position of our mouse in world grid coord's but keep it in the map
        float c = sqrt(powf(tileSize.y/2, 2) + powf(tileSize.x/2, 2));

        mouseGrid.x = round(((mousePos.x * c) / tileSize.x) - (mousePos.y * c / tileSize.y));
        mouseGrid.y = round((mousePos.y * c) / tileSize.y + (mousePos.x * c) / tileSize.x);

        int gridX = viewPos.x + floor(mouseGrid.x / c);
        int gridY = viewPos.y + floor(mouseGrid.y / c);

        int winX = (gridX-viewPos.x) * 16 + (gridY - viewPos.y) * 16;
        int winY = (gridY - viewPos.y) * 8 - (gridX - viewPos.x) * 8 + 8;

        if (gridX > mapSize - 1) gridX = mapSize - 1;
        if (gridY > mapSize - 1) gridY = mapSize - 1;
        if (gridX < 1) gridX = 1;
        if (gridY < 1) gridY = 1;




        //clear view with a background color
        bufferMap.clear(sf::Color(G_black_x, G_black_y, G_black_z));
        //draw the map with vertex array
        ds.drawMap(bufferMap, tileSize, viewPos, resTiles, "../sprites/testTileset.png");
        ds.drawMiniMap(bufferMap, mapSize,sf::Vector2i(gridX,gridY));



        //----DRAW UPDATE-----
      
        //clear the view transparent so it doesnt cover up the main buffer!
        bufferGUI.clear(sf::Color::Transparent);

        //Draw the coord of the mouse on the screen for debugging
        textSmall.setPosition(5, 5);
        //textSmall.setString("Mouse Position (" + std::to_string(gridX) + ", " + std::to_string(gridY) + ") : " + std::to_string(ds.m_map[ds.m_mapSort[gridX][gridY][0]][ds.m_mapSort[gridX][gridY][1]]) + ", " + std::to_string(ds.m_mapSort[gridX][gridY][0]) + ", " + std::to_string(ds.m_mapSort[gridX][gridY][1]));
        textSmall.setString("Mouse Position (" + std::to_string(gridX) + ", " + std::to_string(gridY) + ") : " + std::to_string(ds.m_map[gridX][gridY]));
        bufferGUI.draw(textSmall);
        textSmall.setPosition(5, 15);
        textSmall.setString("View Position (" + std::to_string(viewPos.x) + ", " + std::to_string(viewPos.y) + ")");
        bufferGUI.draw(textSmall);


        //Move the rectangle to the correct position before drawing
        rectCursor.setPosition(mousePos.x, mousePos.y);
        bufferGUI.draw(rectCursor);

        selectionS.setPosition(winX, winY);
        bufferGUI.draw(selectionS);
        


       
        //----DISPLAY THE STUFF----
        
        //display the buffer on to the window and draw the sprite 
        //on to the window (not really sure how this works honestly)
        bufferMap.display(); 
        window.draw(sf::Sprite(bufferMap.getTexture()));

        //do the same as above but for the GUI
        bufferGUI.display(); 
        window.draw(sf::Sprite(bufferGUI.getTexture()));

        // Display the window to the screen
        window.display();
    }




    //----DEBUGGING OUTPUTS----

    std::cout << "*****GAME TERMINATED***** " << std::endl;


    return 0;
}