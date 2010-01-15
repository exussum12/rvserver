#include <cerrno>

#include <arpa/inet.h>
#include <netdb.h>

#include "Socket.h"
#include "logger/Logger.h"

const char* Socket::GetIP() const
{
	struct sockaddr_in sin;
	socklen_t len = sizeof( sin );

	// if the socket has been disconnected already by the OS, we
	// aren't gonna get the IP (which sucks a bit).
	if( getpeername( m_iSocket, (sockaddr *)&sin, &len) < 0 )
		return "<nil>";

	return inet_ntoa(sin.sin_addr);
}

bool Socket::OpenHost( const std::string &host, int port )
{
	const struct hostent *entry = gethostbyname( host.c_str() );

	if( entry == NULL )
	{
		Logger::SystemLog( "Lookup of \"%s\" failed: %u\n", host.c_str(), h_errno );
		return false;
	}

	/* just dereference the entry and call the IP socket maker. */
	const std::string ip = inet_ntoa( *(struct in_addr*)entry->h_addr );

	return Open( ip, port );
}

bool Socket::Open( const std::string &ip, int port )
{
	m_iSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if( m_iSocket < 0 )
	{
		Logger::SystemLog( "MakeSocket: Failed to open socket!\n" );
		return false;
	}

	sockaddr_in sin;
	memset( &sin, 0, sizeof(sin) );

	sin.sin_family = AF_INET;
	sin.sin_port = htons( port );

	// convert the IP string to our in_addr
	inet_aton( ip.c_str(), &sin.sin_addr );

	if( connect(m_iSocket, (sockaddr*)&sin, sizeof(sin)) == -1 )
	{
		printf( "MakeSocket: Failed to connect to %s!\n", ip.c_str() );
		return false;
	}

	return true;
}

void Socket::Close()
{
	shutdown( m_iSocket, SHUT_RDWR );
	close( m_iSocket );
	m_iSocket = -1;
}

int Socket::Read( char *buffer, unsigned len, bool bDontWait )
{
	int iRead = recv( m_iSocket, buffer, len, (bDontWait) ? MSG_DONTWAIT : 0 );

	if( iRead <= 0 )
	{
		// ignore and return as an error
		if( errno == EAGAIN || errno == EWOULDBLOCK )
			return -1;

		Logger::SystemLog( "Write( %u, %p, %u, %d ) failed: %i (%s)",
			m_iSocket, buffer, len, int(bDontWait), errno, strerror(errno) );

		// if iRead returns 0, we disconnected. return that instead.
		return (iRead == 0) ? 0 : -1;
	}

	return iRead;
}

int Socket::Write( const char *buffer, unsigned len, bool bDontWait )
{
	Logger::SystemLog( "Write( %u, %s, %u, %s )", m_iSocket, buffer, len, bDontWait?"true":"false" );
	int iSent = send( m_iSocket, buffer, len, (bDontWait) ? MSG_DONTWAIT : 0 );

	if( iSent <= 0 )
	{
		// ignore and return as an error
		if( errno == EAGAIN || errno == EWOULDBLOCK )
			return -1;

		Logger::SystemLog( "Write( %u, %p, %u, %d ) failed: %i (%s)",
			m_iSocket, buffer, len, int(bDontWait), errno, strerror(errno) );

		// if iSent returns 0, we disconnected. return that instead.
		return (iSent == 0) ? 0 : -1;	
	}

	return iSent;
}

int Socket::Write( const std::string &str, bool bDontWait )
{
	return Write( str.c_str(), str.length(), bDontWait );
}
