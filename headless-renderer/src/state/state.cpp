#include <chrono>

#include "GameScene.h"

int main(int argc, char const *argv[])
{
    /*init socket connection*/
    /*receive input*/
    /*send updated state*/
    /* Start client to send state updates and give ref to game loop context */
 
    /*FIXME: make clock class to handle dt*/
    std::chrono::steady_clock::time_point baseTime = std::chrono::steady_clock::now();    
    std::chrono::steady_clock::time_point previousTime = baseTime;
    
    GameScene scene;

    /* Start game loop */
    while (true)
    {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();    
        double dt =  std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
        previousTime = currentTime;

        // scene.update(dt);
    }
    
    
    return 0;
}
