#pragma once
#include "Info.h"
class Rover
{
private:  
	int ID,
		numberOfMissions,  //number of mission that rover can complete before checkup
		CheckUpDuration,   //duration rover needs to perform after completin N mission
		checkUpDay,        //Day when rover starts checkup
		numberOfMissionsToMaintain, //number of mission rover can complete before maintenance
		maintanceDuration, //duration rover needs to perform after completin n mission
		maintenanceCount;  //To count no. of times rover perform maintenance
	double Speed;
	bool Available,         //check if rover is available or not
		 needsMaintainance; //check if rover needs maintenance
	MISSIONS_TYPES type;
public:
	Rover(int id, int n, int sp, int cp, bool av, MISSIONS_TYPES t) : ID(id), numberOfMissions(n), Speed(sp), CheckUpDuration(cp), Available(av), type(t) ,checkUpDay(-1), needsMaintainance(0), numberOfMissionsToMaintain(0) , maintenanceCount(0), maintanceDuration(-1) {}
	void setID(int id = 1) { ID = id; }
	int getID() { return ID; }
	void setNumberOfMission(int n = 1) { numberOfMissions = n; }
	int getNumberOfMission() { return numberOfMissions; }
	void setSpeed( double sp = 1) { Speed = sp; }
	double getSpeed() { return Speed; }
	void setCheckUpDuration(int cp = 1) { CheckUpDuration = cp; }
	int getCheckUpDuration() { return CheckUpDuration; }
	void setAvailable(bool av) { Available = av; }
	bool getAvailable() { return Available; }
	void setType(MISSIONS_TYPES t) { type = t; }
	MISSIONS_TYPES getType() { return type; }
	void startCheckUP() { checkUpDay = INFO.CurrentDay; }
	bool endCheckUP() { return checkUpDay + CheckUpDuration == INFO.CurrentDay; }
	void setNumberOfMissionsToMiantain(int n) { numberOfMissionsToMaintain = n; }
	int getNumberOfMissionsToMiantain() { return numberOfMissionsToMaintain; }
	void setNeedsMaintanance(bool n) { needsMaintainance = n; }
	int getNeedsMaintanance() { return needsMaintainance; }
	
	int getmaintenanceCount() { return maintenanceCount; };
	void incrementmaintenanceCount() { maintenanceCount++; };
	void setmaintenanceCount(int x) { maintenanceCount = x; };

	void setMaintanceDuration(int d) { maintanceDuration = d; }
	int getMaintanceDuration() { return maintanceDuration; }

	virtual ~Rover() {}
};
