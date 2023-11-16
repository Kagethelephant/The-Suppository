#include "main.hpp"

//alocate space for the grid
static const int arraySize = 200;
//create the grid here so it stays in the heap
float map[arraySize][arraySize] = { -1000.0f };

int main(int argc, char** argv) {

    //----GENERAL VARIABLES----
    



    //----GRID STUFF----

    //size in pixels of the grids to draw to the window
    int gridSize = 4;
    //size of diamond square map *MUST BE EVEN*
    int dsSize = 200;

    int mouseX = 150;
    int mouseY = 150;

    int viewPosX = 100;
    int viewPosY = 125;

    //Create the diamond square object and run function to generate map
    DiamSquare ds;
    ds.newMap(map, dsSize);




    //----VIEW SETUP/SFML----

    sf::RenderWindow window; 
    sf::View view;
    sf::RenderTexture buffer;
    sf::Sprite bufferSprite;

    Game win;
    sf::Vector2i res;
    res = win.windowSetup(window, view, buffer, bufferSprite, 450);

    //create a seperate texture to draw the GUI to
    sf::RenderTexture bufferGUI; 
    bufferGUI.create(res.x, res.y); 
    sf::Sprite bufferSpriteGUI(bufferGUI.getTexture());




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


    //how many grids can fit on the screen
    int viewGridW = floor(res.x / gridSize);
    int viewGridH = floor(res.y / gridSize);
    int viewGridX = viewPosX - floor((viewGridW)/2);
    int viewGridY = viewPosY - floor((viewGridH)/2);

    //iterate throught the diamond square grid and draw squares at different colors
    //for different heights
    for (int i = 0; i < dsSize+1; i += 1)
    {
        for (int z = 0; z < dsSize+1; z += 1)
        {
            int tx = floor(viewGridX + i);
            int ty = floor(viewGridY + z);

            //Check the number from the grid and set a corisponding color
            if (map[tx][ty] < -10) { rect.setFillColor(sf::Color(c_dkblue.x, c_dkblue.y, c_dkblue.z)); }
            else if (map[tx][ty] < -5) { rect.setFillColor(sf::Color(c_blue.x, c_blue.y, c_blue.z)); }
            else if (map[tx][ty] < 0) { rect.setFillColor(sf::Color(c_ltblue.x, c_ltblue.y, c_ltblue.z)); }
            else if (map[tx][ty] < 5) { rect.setFillColor(sf::Color(c_tan.x, c_tan.y, c_tan.z)); }
            else if (map[tx][ty] < 10) { rect.setFillColor(sf::Color(c_green.x, c_green.y, c_green.z)); }
            else if (map[tx][ty] < 15) { rect.setFillColor(sf::Color(c_dkgreen.x, c_dkgreen.y, c_dkgreen.z)); }
            else if (map[tx][ty] < 20) { rect.setFillColor(sf::Color(c_purple.x, c_purple.y, c_purple.z)); }
            else { rect.setFillColor(sf::Color(c_dkpurple.x, c_dkpurple.y, c_dkpurple.z)); }

            //Move the rectangle to the correct position before drawing
            rect.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
            buffer.draw(rect);
        }
    }

    //change the rectangle so it can be used as the mouse position
    rect.setFillColor(sf::Color(0,0,0,0));
    rect.setOutlineColor(sf::Color::Black);




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


        //----UPDATE GET INPUT----

        // regenerate the map when the enter button is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            ds.newMap(map, dsSize);
            ds.reGen(map, dsSize, buffer, rect, gridSize, viewPosX - floor((viewGridW) / 2), viewPosY - floor((viewGridH) / 2));
        }

        //move the map around and update the graphics
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            viewPosX += 1;
            if (viewPosX > dsSize - floor((viewGridW) / 2)) viewPosX = dsSize - floor((viewGridW) / 2);
            else
            {
                ds.reGen(map, dsSize, buffer, rect, gridSize, viewPosX - floor((viewGridW) / 2), viewPosY - floor((viewGridH) / 2));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            viewPosX -= 1;
            if (viewPosX < floor((viewGridW) / 2)) viewPosX = floor((viewGridW) / 2);
            else
            {
                ds.reGen(map, dsSize, buffer, rect, gridSize, viewPosX - floor((viewGridW) / 2), viewPosY - floor((viewGridH) / 2));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            viewPosY -= 1;
            if (viewPosY < floor((viewGridH) / 2)) viewPosY = floor((viewGridH) / 2);
            else
            {
                ds.reGen(map, dsSize, buffer, rect, gridSize, viewPosX - floor((viewGridW) / 2), viewPosY - floor((viewGridH) / 2));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            viewPosY += 1;
            if (viewPosY > dsSize - floor((viewGridH) / 2)) viewPosY = dsSize - floor((viewGridH) / 2);
            else
            {
                ds.reGen(map, dsSize, buffer, rect, gridSize, viewPosX - floor((viewGridW) / 2), viewPosY - floor((viewGridH) / 2));
            }
        } 
        


        //----UPDATE MAKE CALCULATIONS-----
        
        //update the map position to give accurate grid positions
        int viewGridX = viewPosX - floor((viewGridW) / 2);
        int viewGridY = viewPosY - floor((viewGridH) / 2);

        //Create a vector with the pixel coord's in the actual window not the display
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //Update the positon of the hex (this is the cursor for now)
        hex.setPosition(mousePos.x + 3, mousePos.y + 3); 

        //This is the position of our mouse in world grid coord's but keep it in the map
        mouseX = round(mousePos.x / gridSize);
        mouseY = round(mousePos.y / gridSize);
        if (mouseX > dsSize) mouseX = dsSize;
        if (mouseY > dsSize) mouseY = dsSize;
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
        rect.setPosition(mouseX*gridSize+1, mouseY*gridSize+1);
        bufferGUI.draw(rect);


        sf::VertexArray f(sf::Triangles, 3);
        f[0].position = sf::Vector2f(20.0f, 20.0f);
        f[0].color = sf::Color(c_blue.x, c_blue.y, c_blue.z);

        f[1].position = sf::Vector2f(100.0f, 20.0f);
        f[1].color = sf::Color::Red;

        f[2].position = sf::Vector2f(50.0f, 50.0f);
        f[2].color = sf::Color::Green;

        buffer.draw(f);

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

    std::cout << "Window Resolution:  " << res.x << " X " << res.y << std::endl;
    std::cout << std::endl;

    return 0;
}