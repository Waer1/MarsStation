
//Formulation Event derived class

#ifndef __M_EVENT__
#define __M_EVENT__
#include "Event.h"

class F_event : public Event {
private:
    MISSIONS_TYPES type;
    int t_loc,    //Mission’s target location
        t_dur,    //Number of days mission needs at target location
        m_sig;    //Mission’s significance
public:
    F_event(MISSIONS_TYPES t, int ed, int id, int TLOC, int MDUR, int sig)
        : Event(ed, id), type(t), t_loc(TLOC), t_dur(MDUR), m_sig(sig) {}
    int tar_loc() { return t_loc; }
    int tar_dur() { return t_dur; }
    int mis_sig() { return m_sig; }
    MISSIONS_TYPES mis_type() { return type; }


    //To execute mountainous mission formulation
    void Execute(LinkedQueue <Mission* >* mMission , int ) {
        Mission* newmMission = new Mission(getID(), -1, getED(), t_loc, t_dur, m_sig, 0, 0, MOUNTANIOUS);
        mMission->enqueue(newmMission);
    }


    //To execute polar mission formulation
    void Execute(LinkedQueue<Mission* >* pMission) {
        Mission* newpMission = new Mission(getID(), -1, getED(), t_loc, t_dur, m_sig, 0, 0, POLAR);
        pMission->enqueue(newpMission);
    }


    //To execute emergency mission formulation
    void Execute(priorityQueue <Mission* , priorityInfo>* eMission ) {
        Mission* neweMission = new Mission(getID(), -1, getED(), t_loc, t_dur, m_sig, 0, 0, EMERGENCY);
        priorityInfo *pir = new priorityInfo( this->mis_sig() , this->getED() , this->tar_dur() , this->tar_loc());
        eMission->Push(neweMission , *pir);
    }

    ~F_event() {}
};
#endif