#pragma once

#include "../constants/Types.hpp"

#include "IServer.hpp"
#include "MSteamSockets.hpp"

namespace MSteamSockets {
    class Client //: public IServer 
    {
        public:
            Client() {
                MSteamSockets::InitSteamDatagramConnectionSockets();
            }
            ~Client() {
                MSteamSockets::ShutdownSteamDatagramConnectionSockets();
            }
            
            // void start() { start(4999); }
            void start(const SteamNetworkingIPAddr &serverAddr,
                        uint16 nPort);
            void stop();

            // bool send() override;
            // bool send(ByteVector& buffer);
            int receive(ByteVector& buffer);

            bool hasConnectedToServer() {return _server != k_HSteamNetConnection_Invalid; }
            bool hasDisconnectedFromServer() {return _server == k_HSteamNetConnection_Invalid; }

        private:
            HSteamListenSocket _hListenSock;
            // HSteamNetPollGroup _hPollGroup;
            ISteamNetworkingSockets *_pInterface;
            HSteamNetConnection _server;

            void OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo );
            void PollConnectionStateChanges()
            {
                s_pCallbackInstance = this;
                _pInterface->RunCallbacks();
            }

            static Client *s_pCallbackInstance;
            static void SteamNetConnectionStatusChangedCallback( SteamNetConnectionStatusChangedCallback_t *pInfo )
            {
                s_pCallbackInstance->OnSteamNetConnectionStatusChanged( pInfo );
            }
    };
    
}