#include <assert.h>

#include "MSSClient.hpp"
#include "../utils/LogUtils.hpp"


namespace MSteamSockets {
    Client *Client::s_pCallbackInstance = nullptr;

    void Client::start(const SteamNetworkingIPAddr &serverAddr,
                        uint16 nPort) {
        _pInterface = SteamNetworkingSockets();

        // Start connecting
        char szAddr[ SteamNetworkingIPAddr::k_cchMaxString ];
		serverAddr.ToString( szAddr, sizeof(szAddr), true );
		logger::print( "Connecting to server at %s", szAddr );
		SteamNetworkingConfigValue_t opt;
		opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback );
		_server = _pInterface->ConnectByIPAddress( serverAddr, 1, &opt );
		if ( _server == k_HSteamNetConnection_Invalid )
			logger::printErr( "Failed to create connection" );        
        logger::print( "Client connectedto Server\n" );
    }

    int Client::receive(ByteVector& buffer) {
        ISteamNetworkingMessage *pIncomingMsg = nullptr;
        int numMsgs = _pInterface->ReceiveMessagesOnConnection( _server, &pIncomingMsg, 1 );
        if ( numMsgs == 0 )
            return numMsgs;
        if ( numMsgs < 0 )
            logger::printErr( "Error checking for messages" );

        // Just echo anything we get from the server
        buffer.reserve(pIncomingMsg->m_cbSize);
        memcpy(buffer.data(), pIncomingMsg->m_pData, pIncomingMsg->m_cbSize);
        // fwrite( pIncomingMsg->m_pData, 1, pIncomingMsg->m_cbSize, stdout );
        // fputc( '\n', stdout );

        // We don't need this anymore.
        pIncomingMsg->Release();
        return numMsgs;
    }

    void Client::stop() {
        // Close all the connections
		logger::print( "Closing connection...\n" );
        // Send them one more goodbye message.  Note that we also have the
        // connection close reason as a place to send final data.  However,
        // that's usually best left for more diagnostic/logger text not actual
        // protocol strings.
        // send("Client is shutting down.  Goodbye." );

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out and close gracefully.
        _pInterface->CloseConnection( _server, 0, "Shutting down!", true );
    }

    void Client::OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
	{
		assert( pInfo->m_hConn == _server || _server == k_HSteamNetConnection_Invalid );

		// What's the state of the connection?
		switch ( pInfo->m_info.m_eState )
		{
			case k_ESteamNetworkingConnectionState_None:
				// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
				break;

			case k_ESteamNetworkingConnectionState_ClosedByPeer:
			case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
			{
				// g_bQuit = true;

				// Print an appropriate message
				if ( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting )
				{
					// Note: we could distinguish between a timeout, a rejected connection,
					// or some other transport problem.
					logger::print( "We sought the remote host, yet our efforts were met with defeat.  (%s)", pInfo->m_info.m_szEndDebug );
				}
				else if ( pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally )
				{
					logger::print( "Alas, troubles beset us; we have lost contact with the host.  (%s)", pInfo->m_info.m_szEndDebug );
				}
				else
				{
					// NOTE: We could check the reason code for a normal disconnection
					logger::print( "The host hath bidden us farewell.  (%s)", pInfo->m_info.m_szEndDebug );
				}

				// Clean up the connection.  This is important!
				// The connection is "closed" in the network sense, but
				// it has not been destroyed.  We must close it on our end, too
				// to finish up.  The reason information do not matter in this case,
				// and we cannot linger because it's already closed on the other end,
				// so we just pass 0's.
				_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
				_server = k_HSteamNetConnection_Invalid;
				break;
			}

			case k_ESteamNetworkingConnectionState_Connecting:
				// We will get this callback when we start connecting.
				// We can ignore this.
				break;

			case k_ESteamNetworkingConnectionState_Connected:
				logger::print( "Connected to server OK" );
				break;

			default:
				// Silences -Wswitch
				break;
		}
	}

}
    