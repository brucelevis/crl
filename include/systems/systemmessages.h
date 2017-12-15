/*
 * systemmessages.h
 *
 *  Created on: Dec 10, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_SYSTEMMESSAGES_H_
#define INCLUDE_SYSTEMMESSAGES_H_

#include <memory>

namespace SystemMessage
{
	enum MessageType
	{
		NONE  = 0,
		INPUT = (1 << 0)
	};

	struct Message
	{
		MessageType type;

		Message(MessageType type) :
			type(type)
		{}
	};

	struct InputMessage : public Message
	{
		int  key;
		bool shift;
		bool control;
		bool alt;

		InputMessage(int key, bool shift, bool control, bool alt) :
			Message(MessageType::INPUT)
		  , key     (key)
		  , shift   (shift)
		  , control (control)
		  , alt     (alt)
		{}
	};

	typedef std::shared_ptr<Message> TMessagePtr;
}



#endif /* INCLUDE_SYSTEMMESSAGES_H_ */
