#include "User.h"
#include "logger/Logger.h"
#include "packet/ChatPacket.h"	// XXX
#include <cerrno>

/* 20 seconds to idle status */
const unsigned SECONDS_TO_IDLE = (60 / 3);

User::User( unsigned iSocket ) : m_iSocket(iSocket), m_sName("<no name>"),
	m_sRoom("Main")
{
	m_bLoggedIn = m_bMuted = m_bIdle = m_bAway = m_bIsMod = false;
}

User::~User()
{

}

unsigned User::GetIdleTime() const
{
	time_t now = time_t(NULL);
	double diff = difftime( now, m_LastMessage );

	return unsigned(diff);
}

bool User::IsIdle() const
{
	return GetIdleTime() >= SECONDS_TO_IDLE;
}

void User::PacketSent()
{
	m_bAway = m_bIdle = false;
	m_sMessage.clear();

	// update last packet time
	m_LastMessage = time_t(NULL);
}
