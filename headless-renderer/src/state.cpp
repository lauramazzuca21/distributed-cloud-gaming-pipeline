#include <chrono>

#include "../networking-middleware/MSSServer.hpp"
#include "../state/GameScene.hpp"
#include "state/GODragon.hpp"

#include "../constants/Values.hpp"

static const int	MAX_DRAGONS_PER_ROW = 10; //state stuff

int main(int argc, char const *argv[])
{
    /*FIXME: make clock class to handle dt*/
    std::chrono::steady_clock::time_point baseTime = std::chrono::steady_clock::now();    
    std::chrono::steady_clock::time_point previousTime = baseTime;
    
    GameScene scene;
    MSteamSockets::Server server;
    u_int64_t currentFrame = 0;
    /*TODO: init webrtc connection w/client*/

    /*the state starts up a server, waiting for the renderer to connect.
      once it does, the server starts sending it the updates. */

    /*init connection w/renderer*/
    logger::print("Starting server.\n");
    server.start();

    logger::print("Adding Dragon to scene.\n");
    scene.addSceneObject(new Dragon());

    /* Start game loop */
    logger::print("Starting game loop.\n");
    while (!server.hasClientConnected())
    {
    }
    while (!server.hasClientDisconnected())
    {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();    
        double dt =  std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
        previousTime = currentTime;
        /*receive input from client*/
        Input input;
        input.k_key_pressed = Constants::KeyboardKey::W;
        input.m_key_pressed = Constants::MouseKey::LEFT;
        /*update frame*/
        FrameParams frameParams = scene.update(input, dt);
        frameParams._frame_num = currentFrame;
        /*encode frame prams*/
        ByteVector buffer;
        buffer.reserve(Constants::FRAME_PARAMS_CBOR_LEN);
        frameParams.encodeCBOR(buffer);
        /*send updated state to renderer*/
        server.send(buffer);

        ++currentFrame;
    }

    logger::print("Stopping server.\n");
    server.stop();
        
    return 0;
}
