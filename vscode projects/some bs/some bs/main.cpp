#include "main.hpp"


int main(int argc, char** argv) {

    //----GENERAL VARIABLES----
    



    //----GRID STUFF----

    //size in pixels of the grids to draw to the window
    int tileSize = 16;

    //size of diamond square map *MUST BE EVEN*
    sf::Vector2i mousePos;
    sf::Vector2i mouseGrid;
    sf::Vector2i viewPos;

    //start position for the view
    viewPos.x = 100;
    viewPos.y = 125;



    //Create the diamond square object and run function to generate map
    DiamSquare ds;
    ds.newMap(map, mapSize);




    //----VIEW SETUP/SFML----

    sf::RenderWindow window; 
    sf::View view;

    game win;  

    //container for the height and width of the window
    sf::Vector2i resPixels;
    resPixels = win.windowSetup(window, view, 600, true,60);

    //create a seperate texture to draw the GUI to
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

    //how many grids can fit on the screen
    sf::Vector2i resTiles;
    resTiles.x = floor(resPixels.x / tileSize);
    resTiles.y = floor(resPixels.y / tileSize);


    //clear viewwith a background color
    bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));

    ds.drawMap(bufferMap, map, tileSize,viewPos, resTiles,false,"../sprites/tileset.png");






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
            ds.drawMap(bufferMap, map, tileSize, viewPos, resTiles, false, "../sprites/tileset.png");
        }






        //move the map around and update the graphics
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            viewPos.x += 1;
            if (viewPos.x > mapSize - floor((resTiles.x) / 2)) viewPos.x = mapSize - floor((resTiles.x) / 2);
            else
            {
                ds.drawMap(bufferMap, map, tileSize, viewPos, resTiles, false, "../sprites/tileset.png");
            }
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            viewPos.x -= 1;
            if (viewPos.x < floor((resTiles.x) / 2)+1) viewPos.x = floor((resTiles.x) / 2)+1;
            else
            {
                ds.drawMap(bufferMap, map, tileSize, viewPos, resTiles, false, "../sprites/tileset.png");
            }
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            viewPos.y -= 1;
            if (viewPos.y < floor((resTiles.y) / 2)+1) viewPos.y = floor((resTiles.y) / 2)+1;
            else
            {
                ds.drawMap(bufferMap, map, tileSize, viewPos, resTiles, false, "../sprites/tileset.png");
            }
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            viewPos.y += 1;
            if (viewPos.y > mapSize - floor((resTiles.y) / 2)-1) viewPos.y = mapSize - floor((resTiles.y) / 2)-1;
            else
            {
                ds.drawMap(bufferMap, map, tileSize, viewPos, resTiles, false, "../sprites/tileset.png");
            }
        } 
        




        //----UPDATE MAKE CALCULATIONS-----
        
        //update the map position to give accurate grid positions
        int viewGridX = viewPos.x - floor((resTiles.x) / 2);
        int viewGridY = viewPos.y - floor((resTiles.y) / 2);

        //Create a vector with the pixel coord's in the actual window not the display
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


        //This is the position of our mouse in world grid coord's but keep it in the map
        mouseGrid.x = round(mousePos.x / tileSize);
        mouseGrid.y = round(mousePos.y / tileSize);

        if (mouseGrid.x > mapSize) mouseGrid.x = mapSize;
        if (mouseGrid.y > mapSize) mouseGrid.y = mapSize;
        if (mouseGrid.x < 0) mouseGrid.x = 0;
        if (mouseGrid.y < 0) mouseGrid.y = 0;




        //----DRAW UPDATE-----
      
        //clear the view transparent so it doesnt cover up the main buffer!
        bufferGUI.clear(sf::Color::Transparent);

        //Draw the coord of the mouse on the screen as long as its in the map  
        int newX = mouseGrid.x + viewGridX;
        int newY = mouseGrid.y + viewGridY;
        textSmall.setString("(" + std::to_string(newX) + ", " + std::to_string(newY) + ") : " + std::to_string(map[newX][newY]));
        bufferGUI.draw(textSmall);

        //Move the rectangle to the correct position before drawing
        rectCursor.setPosition(mouseGrid.x*tileSize, mouseGrid.y*tileSize);
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