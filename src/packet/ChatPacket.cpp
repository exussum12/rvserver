#include "packet/ChatPacket.h"
#include "packet/PacketUtil.h"
#include <cstdlib>
#include <vector>

using namespace std;

ChatPacket::ChatPacket() : iCode( INVALID_CODE )
{
	r = g = b = 0;
}

ChatPacket::ChatPacket( const ChatPacket &other )
{
	iCode = other.iCode;
	sUsername.assign( other.sUsername );
	sMessage.assign( other.sMessage );
	r = other.r;
	g = other.g;
	b = other.b;
}

ChatPacket::ChatPacket( uint16_t iCode ) : sUsername(BLANK), sMessage(BLANK)
{
	this->iCode = iCode;
	r = g = b = 0;
}

ChatPacket::ChatPacket( uint16_t iCode, const std::string &sUsername_,
	const std::string &sMessage_ ) : sUsername( sUsername_ ), sMessage( sMessage_ )
{
	this->iCode = iCode;
	r = g = b = 0;
}

ChatPacket::ChatPacket( uint16_t iCode, const std::string &sUsername_,
	const std::string &sMessage_, uint8_t r, uint8_t g, uint8_t b ) :
	sUsername( sUsername_ ), sMessage( sMessage_ )
{
	this->iCode = iCode;
	this->r = r;
	this->g = g;
	this->b = b;
	
}

ChatPacket::ChatPacket( const string &sData ) : iCode( INVALID_CODE )
{
	FromString( sData );
}

bool ChatPacket::FromString( const string &sData )
{
	/* XXX: should we split this off into a util file? */
	vector<string> vTokens;

	split( sData, vTokens, '`' );

	// not a well-formed packet
	if( vTokens.size() != 6 )
		return false;

	iCode = atoi( vTokens[0].c_str() );
	sUsername = vTokens[1];
	sMessage = vTokens[2];
	r = atoi( vTokens[3].c_str() );
	g = atoi( vTokens[4].c_str() );
	b = atoi( vTokens[5].c_str() );

	return true;
}

/* Creates a string out of packet data. The data is placed in a
 * buffer and formatted, then copied to a string and returned. */
string ChatPacket::ToString() const
{
	// 3 digits for code, two variable params, 9 digits for RGB, 5 delims
	size_t iLen = 3 + sUsername.size() + sMessage.size() + 9 + 5;
	char *sBuffer = new char[iLen];

	sprintf( sBuffer, "%u`%s`%s`%u`%u`%u", iCode, sUsername.c_str(), sMessage.c_str(), r, g, b );
	string ret( sBuffer );
	delete sBuffer;

	return ret;
}
