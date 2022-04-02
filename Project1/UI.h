#ifndef __UI__
#define __UI__
#include <iostream>
#include "Info.h"
#include "f_event.h"
#include "C_event.h"
#include "p_event.h"
#include <fstream>
#include "LinkedQueue.h"
#include "Mission.h"
#include "Rover.h"
#include "PriorityInfo.h"
#include "priority_queue.h"
#include <Windows.h>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

#pragma comment(lib, "Winmm.lib");


using namespace std;

class UI
{
private:
	//Reads a line from the input
	void readLine(int*, ifstream&); 
public:

	//Constructor
	UI() {}

	//Apply the mode of program
	void receiveMode();

	//Fill the event list
	void readInput(LinkedQueue<Event*>*);

	//Print the Waiting Missions list every day
	void printWaitingMissions(LinkedQueue <Mission* >* mM, LinkedQueue<Mission* >* pM, priorityQueue <Mission*, priorityInfo >* eM);

	//Print the In_ExecMissionsAndRovers list every day
	void printIn_ExecMissionsAndRovers(priorityQueue<Mission*>* I_EM);

	//Print the AvailableRovers list every day
	void printAvailableRovers(LinkedQueue<Rover* >* mR, LinkedQueue<Rover* >* pR, LinkedQueue<Rover* >* eR);

	//Print the InCheckUpRovers list every day
	void printInCheckUpRovers(priorityQueue<Rover*>* CUR);

	//Print the CompletedMissions list every day
	void printCompletedMissions(LinkedQueue<Mission*>* cM);

	//print the output file
	void outputFile(ofstream& , LinkedQueue<Mission*> *cM);

	//print the statics in the output file
	void printStatics(ofstream& myFile, int* roversNum, int* missionsNum, int autoPmissionNum, double  AvgWait, double AvgExc);

	//print the current day every day
	void printCurrentDay(int d);

	//function to separate between printing functions
	void printBreak();  

	//print silent mode sentences
	void printSilentMode();

	//handling writing interface
	void printtype(string s);

	void SetColor(int ForgC);


};
#endif


