/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef BLOCKING_REPLY_HANDLER_HPP
#define BLOCKING_REPLY_HANDLER_HPP

#include "channel.hpp"
#include "nub_exception.hpp"

namespace Mercury
{

class EventDispatcher;
class NetworkInterface;

/**
 *	This is a class to make simple blocking two-way calls easier.
 *
 *	@note You are STRONGLY discouraged from using this from within message
 *	handlers, as you are heading straight for all the common re-entrancy
 *	problems.
 *
 *	@ingroup mercury
 *	@see Bundle::startRequest
 */
class BlockingReplyHandler :
	public ReplyMessageHandler,
	public TimerHandler
{
public:
	BlockingReplyHandler( NetworkInterface & networkInterface,
			ReplyMessageHandler * pHandler = NULL );

	Reason waitForReply( Channel * pChannel = NULL,
		   int maxWaitMicroSeconds = 10 * 1000000 );

	static void safeToCall( bool value )	{ safeToCall_ = value; }

protected:
	virtual void handleTimeout( TimerHandle handle, void * arg );

	virtual void handleMessage( const Address & addr,
		UnpackedMessageHeader & header,
		BinaryIStream & data,
		void * arg );

	virtual void handleException( const NubException & ex, void * );

	// Methods to be overridden
	virtual void onMessage( const Address & addr,
		UnpackedMessageHeader & header,
		BinaryIStream & data,
		void * arg ) {}

	virtual void onException( const NubException & ex, void * ) {}

	EventDispatcher & dispatcher();

	NetworkInterface &	interface_;
	Reason				err_;
	TimerHandle			timerHandle_;

	ReplyMessageHandler * pHandler_;

	static bool safeToCall_;
};


/**
 *	This is a template class to make simple blocking two-way calls easier.
 *
 *	To use this class, first make a request using Bundle::startRequest.
 *	Then, instantiate an object of this type, using the expected
 *	reply type as the template argument. Then, call the @ref waitForReply
 *	method, and the handler will block until a reply is received or
 *	the request times out.
 *
 *	@note You are STRONGLY discouraged from using this from within message
 *	handlers, as you are heading straight for all the common re-entrancy
 *	problems.
 *
 *	@ingroup mercury
 *	@see Bundle::startRequest
 */
template <class RETURNTYPE>
class BlockingReplyHandlerWithResult : public BlockingReplyHandler
{
public:
	BlockingReplyHandlerWithResult( NetworkInterface & networkInterface );

	RETURNTYPE & get();

private:
	virtual void handleMessage( const Address & addr,
		UnpackedMessageHeader & header,
		BinaryIStream & data,
		void * arg );

	RETURNTYPE		result_;
};



// -----------------------------------------------------------------------------
// Section: BlockingReplyHandlerWithResult
// -----------------------------------------------------------------------------

/**
 * 	This is the constructor.
 *
 * 	@param interface	The NetworkInterface to be used for sending and receiving.
 */
template <class RETURNTYPE> inline
BlockingReplyHandlerWithResult<RETURNTYPE>::BlockingReplyHandlerWithResult(
		NetworkInterface & networkInterface ) :
	BlockingReplyHandler( networkInterface, NULL )
{
}


/**
 * 	This method returns the result of the request.
 */
template <class RETURNTYPE>
inline RETURNTYPE & BlockingReplyHandlerWithResult<RETURNTYPE>::get()
{
	return result_;
}


/**
 * 	This method handles reply messages from Mercury.
 * 	It unpacks the reply and stores it.
 */
template <class RETURNTYPE>
inline void BlockingReplyHandlerWithResult<RETURNTYPE>::handleMessage(
		const Address & addr,
		UnpackedMessageHeader & header,
		BinaryIStream & data,
		void * arg )
{
	data >> result_;

	this->BlockingReplyHandler::handleMessage( addr, header, data, arg );
}

} // namespace Mercury


#endif //  BLOCKING_REPLY_HANDLER_HPP
