/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef REVIVER_HPP
#define REVIVER_HPP

#include "cstdmf/singleton.hpp"
#include "network/machine_guard.hpp"
#include "server/server_app.hpp"

class ReviverConfig;

namespace Mercury
{
class EventDispatcher;
class NetworkInterface;
}

class ComponentReviver;
typedef std::vector< ComponentReviver * > ComponentRevivers;

extern ComponentRevivers * g_pComponentRevivers;

/**
 *	This class is used to represent the reviver process. It monitors for the
 *	unexpected death of processes and starts new ones.
 */
class Reviver : public ServerApp, public TimerHandler,
	public Singleton< Reviver >
{
public:
	SERVER_APP_HEADER( Reviver, reviver )

	typedef ReviverConfig Config;

	Reviver( Mercury::EventDispatcher & mainDispatcher,
			Mercury::NetworkInterface & interface );
	virtual ~Reviver();

	bool queryMachinedSettings();

	void shutDown();

	void revive( const char * createComponent );

	bool hasEnabledComponents() const;

	// Overrides from TimerHandler
	virtual void handleTimeout( TimerHandle handle, void * arg );

	void markAsDirty()			{ isDirty_ = true; }

	/**
	 *	This class is used to handle a reply from BWMachined telling us the tags
	 *	associated with this machine.
	 */
	class TagsHandler : public MachineGuardMessage::ReplyHandler
	{
	public:
		TagsHandler( Reviver &reviver ) : reviver_( reviver ) {}
		virtual bool onTagsMessage( TagsMessage &tm, uint32 addr );

	private:
		Reviver &reviver_;
	};

private:
	virtual bool init( int argc, char * argv[] );
	virtual bool run();

	enum TimeoutType
	{
		TIMEOUT_REATTACH
	};

	TimerHandle						timerHandle_;

	ComponentRevivers	components_;

	bool				shuttingDown_;
	bool				isDirty_; // For output
};

#endif // REVIVER_HPP
