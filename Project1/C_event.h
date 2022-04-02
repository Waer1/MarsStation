
//Cancellation Event derived Class

#pragma once
#include "Event.h"
class C_event : public Event {

public:
	C_event(int e, int i) : Event(e, i) {}

	//To execute mountainous mission cancellation
	void Execute(LinkedQueue <Mission* >* mMission) 
	{
		Mission* temp = cancel(mMission);
	};



};
