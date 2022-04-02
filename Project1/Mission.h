#pragma once
#include <iostream>
#include "Rover.h"


class Mission {
private:
	int ID,
		FD,    //Formulation day
		CD,    //Completion day
		ED,    //Execution days
		WD,    //Waiting days
		TLOC,  //Target Location
		MDUR,  //Number of days mission needs at target location
		SIG,   //Mission’s significance
		RoverIn_ExecutionID;  //rover’s ID which assigned to this mission
	
	int assignedDay;    //Day when mission assigned to rover
	bool Assigned, Completed;
	MISSIONS_TYPES TYP;
	Rover* inExecutionRover;
public:
	Mission(int id = 1, int cd = 1, int fd = 1, int tloc = 1, int mdur = 1, int sig = 1, bool assigned = false, bool completed = false, MISSIONS_TYPES typ = EMERGENCY);

	void SetID(int id);

	int GetID();

	void SetFD(int fd);

	int GetFD();

	int GetWD();
	void setWD(int);

	void SetTLOC(int tloc);

	int GetTLOC();

	void SetMDUR(int mdur);

	int GetMDUR();

	void SetSIG(int sig);

	int GetSIG();

	void SetAssigned(bool assigned);

	bool GetAssigned();

	void SetCompleted(bool completed);

	bool GetCompleted();

	void SetTYP(MISSIONS_TYPES typ);

	MISSIONS_TYPES GetTYP();


	void print();

	void setRoverIn_ExecutionID(int RID) { RoverIn_ExecutionID = RID; }

	int getRoverIn_ExecutionID() { return RoverIn_ExecutionID; }

	void SetCD(int cd) { CD = cd; }

	int GetCD() { return CD; }

	void SetED(int ed) { ED = ed; }

	int GetED() { return ED; }

	Rover* getRover();

	void addRover(Rover*);   //function adds rover to assign to mission

	void startMission() {  assignedDay = INFO.CurrentDay; }

	bool endMission() {	
		return ( assignedDay+ ceil( 2*TLOC / ( getRover()->getSpeed() * 25.0 ) ) + MDUR == INFO.CurrentDay ); }

	int getAssignedDay() { return assignedDay; }

};



