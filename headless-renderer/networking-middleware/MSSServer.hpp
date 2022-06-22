#pragma once

#include "../constants/Types.hpp"

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
            
            void start() { start(4999); }
            void start(uint16 nPort);
            void stop();

            // bool send() override;
            bool send(ByteVector& buffer);
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