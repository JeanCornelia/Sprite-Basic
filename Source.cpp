#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "charactermovement.h"


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    positionCharacter spriteSkeleton({ 100.0f,100.0f });
    //start the initial time point //chrono was assigned to know the variable using auto
    auto timepoint = std::chrono::steady_clock::now();
  
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event skeletonwalk;
        while (window.pollEvent(skeletonwalk))
        {
            // Close window: exit
            if (skeletonwalk.type == sf::Event::Closed)
                window.close();
        }
        //Get delta time to update the timepoint to another timepoint
        float deltatime;
        {
            //makes or stops the newtimepoint to a particular timepoint using the const variable to hold it for the mean time
            const auto newTimepoint = std::chrono::steady_clock::now();
            //deltatime is equal to the time interval of the float to represent dractions of  newTimepoint minus timepoint where
            //it will be counted by .count()
            deltatime = std::chrono::duration<float>(newTimepoint - timepoint).count();
            //returns to normal back again where timepoint is equal to newTimepoint
            timepoint = newTimepoint;
        }
        //Control Input Of The Sprite
        sf::Vector2f currentdirection(0.0f, 0.0f); // initialization of the current direction or the starting position.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //up
        {
            currentdirection.y = currentdirection.y - 1.0f;
            spriteSkeleton.setDirections(currentdirection);
            spriteSkeleton.updatePositionAllOver(deltatime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))//down
        {
            currentdirection.y = currentdirection.y + 1.0f;
            spriteSkeleton.setDirections(currentdirection);
            spriteSkeleton.updatePositionAllOver(deltatime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //left
        {
            currentdirection.x = currentdirection.x - 1.0f;
            spriteSkeleton.setDirections(currentdirection);
            spriteSkeleton.updatePositionAllOver(deltatime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))//right
        {
            currentdirection.x = currentdirection.x + 1.0f;
            spriteSkeleton.setDirections(currentdirection);
            spriteSkeleton.updatePositionAllOver(deltatime);
        }
        else 
        {
            currentdirection.x = currentdirection.x;
            currentdirection.y = currentdirection.y;
            spriteSkeleton.setDirections(currentdirection);
            spriteSkeleton.stopWalk(deltatime);
        }
        window.clear();
        spriteSkeleton.ShiftingandDraw(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
