#include "main.hpp"


int main(int argc, char** argv) {



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
    viewPos.x = 50;
    viewPos.y = -10;


    //Create the diamond square object and run function to generate map
    DiamSquare ds;
    ds.newMap(dsMap, mapSize);




    //----VIEW SETUP/SFML----

    sf::RenderWindow window; 
    sf::View view;

    game win;  

    //container for the height and width of the window
    sf::Vector2i resPixels;
    resPixels = win.windowSetup(window, view, 400, true,60);

    //how many grids can fit on the screen
    sf::Vector2i resTiles;
    resTiles.x = ceil(resPixels.x / tileSize.x)*2;
    resTiles.y = ceil(resPixels.y / tileSize.y);


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
    textSmall.setFillColor(sf::Color(c_snow.x, c_snow.y, c_snow.z));
    textSmall.setStyle(sf::Text::Regular);
    textSmall.setPosition(5, 5);

    //rectangles to draw the grids
    sf::RectangleShape rectCursor;
    rectCursor.setSize(sf::Vector2f(3,3));
    rectCursor.setFillColor(sf::Color::White);
    rectCursor.setOutlineColor(sf::Color::Transparent);
    rectCursor.setOutlineThickness(2);
    rectCursor.setOrigin(0, 0);




    //----DRAW STATIC----


    //clear viewwith a background color
    bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
    //draw the map with vertex array
    ds.drawMap(bufferMap, dsMap, tileSize, viewPos, resTiles);




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
            ds.drawMap(bufferMap, dsMap, tileSize, viewPos, resTiles);
        }




        //move the map around and update the graphics
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            viewPos.x += 1;
            //if (viewPos.x > mapSize - resTiles.x) viewPos.x = mapSize - resTiles.x;
            //else
            //{
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, dsMap, tileSize, viewPos, resTiles);
            //}
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            viewPos.x -= 1;
            //if (viewPos.x < 1) viewPos.x = 1;
            //else
            //{
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, dsMap, tileSize, viewPos, resTiles);
            //}
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            viewPos.y -= 1;
            //if (viewPos.y < 1) viewPos.y = 1;
            //else
            //{
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, dsMap, tileSize, viewPos, resTiles);
            //}
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            viewPos.y += 1;
            //if (viewPos.y > mapSize - resTiles.y) viewPos.y = mapSize - resTiles.y;
            //else
            //{
                bufferMap.clear(sf::Color(c_black.x, c_black.y, c_black.z));
                ds.drawMap(bufferMap, dsMap, tileSize, viewPos, resTiles);
            //}
        } 
        



        //----UPDATE MAKE CALCULATIONS-----     

        //Create a vector with the pixel coord's in the actual window not the display
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //This is the position of our mouse in world grid coord's but keep it in the map
        float c = sqrt(powf(tileSize.y/2, 2) + powf(tileSize.x/2, 2));

        mouseGrid.x = round(((mousePos.x * c) / tileSize.x) - (mousePos.y * c / tileSize.y));
        mouseGrid.y = round((mousePos.y * c) / tileSize.y + (mousePos.x * c) / tileSize.x);

        int gridX = viewPos.x + floor(mouseGrid.x / c);
        int gridY = viewPos.y + floor(mouseGrid.y / c);

        if (gridX > mapSize - 1) gridX = mapSize - 1;
        if (gridY > mapSize - 1) gridY = mapSize - 1;
        if (gridX < 1) gridX = 1;
        if (gridY < 1) gridY = 1;

        // create an array of 3 vertices that define a triangle primitive

        int posX = (mouseGrid.x-viewPos.x)*tileSize.x;
        int posY = (mouseGrid.y-viewPos.y)*tileSize.y;
        sf::VertexArray triangle1(sf::Triangles, 3);
        sf::VertexArray triangle2(sf::Triangles, 3);

        // define the position of the triangle's points
        triangle1[0].position = sf::Vector2f(posX, posY);
        triangle1[1].position = sf::Vector2f((posX + (tileSize.x / 2)), (posY - (tileSize.y / 2)));
        triangle1[2].position = sf::Vector2f((posX + (tileSize.x / 2)), (posY + (tileSize.y / 2)));
        triangle2[0].position = sf::Vector2f((posX + (tileSize.x / 2)), (posY - (tileSize.y / 2)));
        triangle2[1].position = sf::Vector2f((posX + (tileSize.x / 2)), (posY + (tileSize.y / 2)));
        triangle2[2].position = sf::Vector2f(posX + tileSize.x, posY);

        // define the color of the triangle's points
        triangle1[0].color = sf::Color(50,100,120,150);
        triangle1[1].color = sf::Color(50, 100, 120, 150);
        triangle1[2].color = sf::Color(50, 100, 120, 150);
        triangle2[0].color = sf::Color(50, 100, 120, 150);
        triangle2[1].color = sf::Color(50, 100, 120, 150);
        triangle2[2].color = sf::Color(50, 100, 120, 150);


        bufferGUI.clear(sf::Color::Transparent);
        bufferGUI.draw(triangle1);
        bufferGUI.draw(triangle2);




        //----DRAW UPDATE-----
      
        //clear the view transparent so it doesnt cover up the main buffer!
        

        //Draw the coord of the mouse on the screen for debugging
        textSmall.setPosition(5, 5);
        textSmall.setString("Mouse Position (" + std::to_string(gridX) + ", " + std::to_string(gridY) + ") : " + std::to_string(dsMap[gridX][gridY]));
        bufferGUI.draw(textSmall);
        textSmall.setPosition(5, 15);
        textSmall.setString("View Position (" + std::to_string(viewPos.x) + ", " + std::to_string(viewPos.y) + ")");
        bufferGUI.draw(textSmall);

        //Move the rectangle to the correct position before drawing
        rectCursor.setPosition(mousePos.x, mousePos.y);
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
    std::cout << "Window Resolution:  "  << std::endl;

    for (int i = 0; i < 8; ++i)
    {
        int var = floor((i+1)/2);
        std::cout << "i:  " << i << ", Out: " << var << std::endl;
    }
    std::cout << std::endl;

    return 0;
}