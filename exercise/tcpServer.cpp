#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main( void ) {

	// Create a socket
	int listening = socket( AF_INET, SOCK_STREAM, 0 );
	if ( listening == -1 ) {

		std::cerr << "Can't create a socket!";
		return -1;
	}

	// Bind the socket to a IP / port
	sockaddr_in	hint; /* sockaddr_in in is for internet protocol version 4 ipv4 */
	hint.sin_family = AF_INET;
	hint.sin_port = htons( 54000 ); /* htons is to convert the integer to what the network understand -host to network short- */
	inet_pton( AF_INET, 
				"0.0.0.0", 
				&hint.sin_addr ); /* internet command to convert a pointer to string to a number or array of integers for example 127.0.0.1 */

	if ( bind( listening, (sockaddr*)&hint, sizeof( hint ) ) == -1 ) {

		std::cerr << "Can't bind to IP/port";
		return -2;
	}

	// Mark the socket for listening in
	if ( listen( listening, SOMAXCONN ) == -1 ) {
		
		std::cerr << "Can't listen!";
		return -3;
	}

	while ( true ) {
		// Accept a call
		sockaddr_in client;
		socklen_t clientSize = sizeof( client );
		char host[NI_MAXHOST];
		char svc[NI_MAXSERV];

		int clientSocket = accept( listening, 
									(sockaddr*)&client, 
									&clientSize );

		if ( clientSocket == -1 ) {

			std::cerr << "Problem with client connecting!";
			continue;
		}
	
		memset( host, 0, NI_MAXHOST );
		memset( svc, 0, NI_MAXSERV );
		int result = getnameinfo( (sockaddr*)&client, 
									sizeof( client ), 
									host, 
									NI_MAXHOST, 
									svc, 
									NI_MAXSERV, 
									0);

		if ( result ) {

			std::cout << host << " connected on " << svc << std::endl;
		} else {

			inet_ntop( AF_INET, &client.sin_addr, host, NI_MAXHOST );
			std::cout << host << " connected on " << ntohs( client.sin_port ) << std::endl;
		}
		
	}
	// Close the listening socket
	close( listening );


	// While receiving - display message, echo message
	char buf[4096];
	while ( true ) {

		// clear the buffer
		memset( buf, 0, 4096 );
		int bytesRecv = recv( clientSocket, buf, 4096, 0 );
		if ( bytesRecv == -1 ) {

			std::cerr << "There was a connection issue" << std::endl;
			break;
		}

		if ( bytesRecv == 0 ) {

			std::cout << "The client disconnected" << std::endl;
			break;
		}

		// Display message
		std::cout << "Received: " << std::string( buf, 0, bytesRecv ) << std::endl;

		// Resend message
		send( clientSocket, buf, bytesRecv + 1, 0 );
	}

	// Close socket
	close( clientSocket );

	return 0;
}