#ifndef __MARS_STATION
#define __MARS_STATION
#include "UI.h"
#include "LinkedList.h"
#include "Info.h"

using namespace std;

class Mars_Station
{
	UI input_output;
	LinkedQueue<Event*> event_list;

	LinkedQueue <Mission* >* mMission;                  //mountainous mission waiting list
	LinkedQueue<Mission* > * pMission;                     //polar mission waiting list
	priorityQueue <Mission* , priorityInfo > * eMission; //emergency mission waiting list

	LinkedQueue<Rover* >* mRover;            //mountainous rover Available list
	LinkedQueue<Rover* >* pRover;            //polar rover Available list
	LinkedQueue<Rover* >* eRover;            //emergency rover Available list


	priorityQueue<Rover*>* CheckUpRovers;
	priorityQueue<Mission*>* In_ExecutionMissions;
	LinkedQueue<Mission*>* completedMissions;

	LinkedQueue<Rover* >* inMaintenanceRover;
	void assMissionToRover(Mission*, Rover*); //assign mission to rover

	int missionsNum[COUNT_MISSIONS_TYPES],
		roversNum[COUNT_MISSIONS_TYPES],
		autoPmissionNum;         //number of automatically promoted mission
	double AvgWait,              //Average waiting time in days of missions
		   AvgExc;               // average execution time in days of missions

	void endCheckUp();         //Check on the rovers to be moved out of checkUp list
	void Avgcalc(double & AvgWait,double & AvgExc );    /* calculate Average waiting time
													     and average execution time */

public:
	Mars_Station();
	
	//function to read input file
	void read_input();

	//function to start the program and execute functions per day
	void start();

	//assign new rovers and add them to available rover lists
	void assRovers();

	//assign rover to mission at day
	void assRovrsToMissionsAtDay();

	//print mission and rover information per day
	void printDayDetails();

	//print sample output file
	void outFile();

	//check if program is ended
	bool endProgram();

	//check if mission is completed or failure or not and check rover status
	void stateRoversStatusFromExecution();

	//check if there is event and execute it
	void eventExc();

	//check if any mountainous mission needed to be automatically promoted
	void checkAutoP();

	//check if mission is failure or not
	bool missionFailure(Mission* mis);

	//Decrement the maintainence duration for rovers in maintainence
	void decMaintenanceDur();

	//Destructor
	~Mars_Station();

};
#endif


