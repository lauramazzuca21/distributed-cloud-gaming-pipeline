#include <assert.h>

#include "MSSServer.hpp"
#include "../utils/LogUtils.hpp"


namespace MSteamSockets {
    Server *Server::s_pCallbackInstance = nullptr;

    void Server::start(uint16 nPort) {
        _pInterface = SteamNetworkingSockets();

        // Start listening
        SteamNetworkingIPAddr serverLocalAddr;
        serverLocalAddr.Clear();
        serverLocalAddr.m_port = nPort;
        SteamNetworkingConfigValue_t opt;
        opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback );
        _hListenSock = _pInterface->CreateListenSocketIP( serverLocalAddr, 1, &opt );
        if ( _hListenSock == k_HSteamListenSocket_Invalid )
            logger::printErr( "Failed to listen on port %d", nPort );
        _hPollGroup = _pInterface->CreatePollGroup();
        if ( _hPollGroup == k_HSteamNetPollGroup_Invalid )
            logger::printErr( "Failed to listen on port %d", nPort );
        logger::print( "Server listening on port %d\n", nPort );
    }

    bool Server::send(ByteVector& buffer)
	{
		EResult res = _pInterface->SendMessageToConnection( _client, buffer.data(), buffer.size(), k_nSteamNetworkingSend_Reliable, nullptr );
        if (res != k_EResultOK)
            return false;
        return true;
	}

    void Server::stop() {
        // Close all the connections
		logger::print( "Closing connection...\n" );
        // Send them one more goodbye message.  Note that we also have the
        // connection close reason as a place to send final data.  However,
        // that's usually best left for more diagnostic/logger text not actual
        // protocol strings.
        // send("Server is shutting down.  Goodbye." );

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out and close gracefully.
        _pInterface->CloseConnection( _client, 0, "Server Shutdown", true );
    }

    void Server::OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
	{
		char temp[1024];

		// What's the state of the connection?
		switch ( pInfo->m_info.m_eState )
		{
			case k_ESteamNetworkingConnectionState_None:
				// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
				break;

			case k_ESteamNetworkingConnectionState_ClosedByPeer:
			case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
			{
				// Ignore if they were not previously connected.  (If they disconnected
				// before we accepted the connection.)
				if ( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected )
				{

					// Locate the client.  Note that it should have been found, because this
					// is the only codepath where we remove clients (except on shutdown),
					// and connection change callbacks are dispatched in queue order.
					assert( _client != k_HSteamNetConnection_Invalid );

					// Select appropriate logger messages
					const char *pszDebugLogAction;
					if ( pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally )
					{
						pszDebugLogAction = "problem detected locally";
					}
					else
					{
						// Note that here we could check the reason code to see if
						// it was a "usual" connection or an "unusual" one.
						pszDebugLogAction = "closed by peer";
					}

					// Spew something to our own logger.  Note that because we put their nick
					// as the connection description, it will show up, along with their
					// transport-specific data (e.g. their IP address)
					logger::print( "Connection %s %s, reason %d: %s\n",
						pInfo->m_info.m_szConnectionDescription,
						pszDebugLogAction,
						pInfo->m_info.m_eEndReason,
						pInfo->m_info.m_szEndDebug
					);

					_client = k_HSteamNetConnection_Invalid;
				}
				else
				{
					assert( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting );
				}

				// Clean up the connection.  This is important!
				// The connection is "closed" in the network sense, but
				// it has not been destroyed.  We must close it on our end, too
				// to finish up.  The reason information do not matter in this case,
				// and we cannot linger because it's already closed on the other end,
				// so we just pass 0's.
				_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
				break;
			}

			case k_ESteamNetworkingConnectionState_Connecting:
			{
				// This must be a new connection
				assert( pInfo->m_hConn != _client );

				logger::print( "Connection request from %s", pInfo->m_info.m_szConnectionDescription );

				// A client is attempting to connect
				// Try to accept the connection.
				if ( _pInterface->AcceptConnection( pInfo->m_hConn ) != k_EResultOK )
				{
					// This could fail.  If the remote host tried to connect, but then
					// disconnected, the connection may already be half closed.  Just
					// destroy whatever we have on our side.
					_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
					logger::print( "Can't accept connection.  (It was already closed?)" );
					break;
				}

				// Assign the poll group
				if ( !_pInterface->SetConnectionPollGroup( pInfo->m_hConn, _hPollGroup ) )
				{
					_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
					logger::print( "Failed to set poll group?" );
					break;
				}
			}

			case k_ESteamNetworkingConnectionState_Connected:
				// We will get a callback immediately after accepting the connection.
				// Since we are the server, we can ignore this, it's not news to us.
				break;

			default:
				// Silences -Wswitch
				break;
		}
	}

}
    