
//promotion event derived class

#ifndef __P_EVENT__
#define __P_EVENT__
#include "Event.h"
class P_event : public Event {
public:
    P_event(int e, int i) : Event(e, i) {}


    //promote mountainous mission to emergency
    void Execute(LinkedQueue <Mission* >* mMission, priorityQueue <Mission*, priorityInfo >* eMission)
    {

        Mission* transferd = cancel(mMission);
        if (transferd) {
            priorityInfo* pir = new priorityInfo(transferd->GetSIG(), transferd->GetFD(), transferd->GetMDUR(), transferd->GetTLOC());
            transferd->SetTYP(EMERGENCY);
            eMission->Push(transferd, *pir);
        }


    }
    //to be implemented
    //delete the mission from mountainous waiting list 
    // create its priority
    //insert it to emergency waiting Mission



    ~P_event() {}
};
#endif

