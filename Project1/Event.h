#ifndef __EVENT__
#define __EVENT__

#include <iostream>
#include"PriorityInfo.h"
#include "Mission.h"
#include "priority_queue.h"
#include "LinkedList.h"
#include "LinkedQueue.h"
#include "Info.h"

class Event { //abstract class
private:
	int ED,   //Event Day
		ID;   //Related missiom ID
public:
	Event(int e, int i) : ED(e), ID(i) {}
	virtual int getID() { return ID; } //for all derived classes
	virtual int getED() { return ED; } //for all derived classes
	void print() { std::cout << "ID : " << ID << "  " << " ED : " << ED << std::endl; }

	Mission* cancel(LinkedQueue <Mission* >* mMission)  //To Cancel mountainous mission
	{

		Mission* searchedOne;
		Mission* deleted = nullptr;
		LinkedQueue <Mission* >* temporaryone = new  LinkedQueue <Mission* >();
		while (!mMission->isEmpty())
		{
			mMission->dequeue(searchedOne);

			if (searchedOne->GetID() == this->getID())
				deleted = searchedOne;
			else
				temporaryone->enqueue(searchedOne);
		}

		while (!temporaryone->isEmpty())
		{
			temporaryone->dequeue(searchedOne);
			mMission->enqueue(searchedOne);
		}

		return deleted;
	}
	
	virtual ~Event() {} //virtual destructor
};
#endif


