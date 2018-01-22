/*$T Share/MessageFactory.h GC 1.140 10/10/07 10:06:30 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __MESSAGEFACTORY_H__
#define __MESSAGEFACTORY_H__

#include "KwNetMessage.h"

class	MessageFactory
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	virtual~MessageFactory()
	{
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	virtual IMessage *CreateMessage()
	{
		return NULL;
	};

	virtual MSG_ID	GetMsgID() const = 0;

	virtual uint32	GetMessageMaxSize() const = 0;
};
#endif
