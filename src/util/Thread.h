/* Thread: a base class that creates a thread and provides basic controls.
 * Based entirely off the Pthread library. */

#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <pthread.h>

class Thread
{
public:
	Thread(): m_iID(0), m_bPaused(false) { }

	/* starts a thread that runs CreateFn(data) until end */
	void Start( void*(*CreateFn)(void *), void *data );

	/* attempts to stop the thread, waiting until it finishes. */
	void Stop();

	/* temporarily halts execution of this thread. */
	void Pause();

	/* continues processing of this thread after a Pause(). */
	void Continue();

	/* self-explanatory */
	bool IsPaused()		{ return m_bPaused; }

	/* returns the unique identifier of the thread */
	uint64_t GetID() const 	{ return m_iID; }

private:
	pthread_t m_Thread;
	uint64_t m_iID;

	bool m_bPaused;
};

/* A very simple C++ wrapper for pthread_mutex_t. */
class Mutex
{
public:
	Mutex();
	~Mutex();

	/* returns 0 on success, nonzero on failure. I don't expect you to need
	 * to check these, but they're there if you need them. Lock() blocks. */
	int Lock();
	int Unlock();

	/* returns true if this call locks, but does not block otherwise. */
	bool TryLock();

private:
	pthread_mutex_t m_Lock;
};

/* A very simple C++ wrapper for pthread_spinlock_t. */
class Spinlock
{
public:
	Spinlock();
	~Spinlock();

	/* see above comments in Mutex */
	int Lock();
	int Unlock();

	/* returns true if this call locks, but does not block otherwise. */
	bool TryLock();

private:
	pthread_spinlock_t m_Lock;
};	

#endif // THREAD_H
