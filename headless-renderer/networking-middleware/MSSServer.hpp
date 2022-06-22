#pragma once

#include "IServer.hpp"
#include "MSteamSockets.hpp"


namespace MSteamSockets {
    class Server //: public IServer 
    {
        public:
            Server() {
                MSteamSockets::InitSteamDatagramConnectionSockets();
            }
            ~Server() {
                MSteamSockets::ShutdownSteamDatagramConnectionSockets();
            }
            
            void start();
            void start(uint16 nPort);
            void stop();

            // bool send() override;
            bool send(const char *str );
            // void receive() override;

        private:
            HSteamListenSocket _hListenSock;
            HSteamNetPollGroup _hPollGroup;
            ISteamNetworkingSockets *_pInterface;
            HSteamNetConnection _client;

            void OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo );
            void PollConnectionStateChanges()
            {
                s_pCallbackInstance = this;
                _pInterface->RunCallbacks();
            }

            static Server *s_pCallbackInstance;
            static void SteamNetConnectionStatusChangedCallback( SteamNetConnectionStatusChangedCallback_t *pInfo )
            {
                s_pCallbackInstance->OnSteamNetConnectionStatusChanged( pInfo );
            }
    };
    
}