#include <chrono>

#include "../networking-middleware/MSSServer.hpp"
#include "../state/GameScene.hpp"
#include "state/GODragon.hpp"

static const int	MAX_DRAGONS_PER_ROW = 10; //state stuff

int main(int argc, char const *argv[])
{
    /*FIXME: make clock class to handle dt*/
    std::chrono::steady_clock::time_point baseTime = std::chrono::steady_clock::now();    
    std::chrono::steady_clock::time_point previousTime = baseTime;
    
    GameScene scene;
    MSteamSockets::Server server;
    /*TODO: init webrtc connection w/client*/

    /*the state starts up a server, waiting for the renderer to connect.
      once it does, the server starts sending it the updates. */

    /*init connection w/renderer*/
  
    server.start();

    scene.addSceneObject(new Dragon());

    /* Start game loop */
    while (true)
    {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();    
        double dt =  std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
        previousTime = currentTime;
        /*receive input from client*/

        scene.update(dt);

        /*send updated state to renderer*/
        server.send();
        
    }

    server.stop();
        
    return 0;
}
