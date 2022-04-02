#include "Mars_Station.h"

Mars_Station::Mars_Station() {


	mMission = new LinkedQueue <Mission* >();
	pMission = new LinkedQueue<Mission* >();
	eMission = new priorityQueue <Mission*, priorityInfo>();
	In_ExecutionMissions = new priorityQueue<Mission*>();
	completedMissions = new LinkedQueue<Mission*>();
	mRover = new LinkedQueue<Rover* >();
	pRover = new LinkedQueue<Rover* >();
	eRover = new LinkedQueue<Rover* >();
	CheckUpRovers = new priorityQueue<Rover*>();
	inMaintenanceRover = new LinkedQueue<Rover* >();
	autoPmissionNum = 0;
	
}

void Mars_Station::assMissionToRover(Mission* m, Rover* r)
{
	m->setWD(INFO.CurrentDay - m->GetFD() );
	m->setRoverIn_ExecutionID(r->getID());
	m->addRover(r);
	m->startMission();
	r->setAvailable(false);
	r->setNumberOfMission( r->getNumberOfMission() - 1);

	r->setNumberOfMissionsToMiantain(r->getNumberOfMissionsToMiantain() + 1);
	if (r->getNumberOfMissionsToMiantain() >= 2 * INFO.missions_before_checkup) {
		r->setNumberOfMissionsToMiantain(0);
		r->incrementmaintenanceCount();
		r->setNeedsMaintanance(1);
		r->setMaintanceDuration(ceil(r->getCheckUpDuration() / 2.0));
		inMaintenanceRover->enqueue(r);
	}
	
	missionsNum[m->GetTYP()]++;
	//roversNum[r->getType()]++;
	//waiting day , exec day 
	In_ExecutionMissions->Push(m, INFO.CurrentDay + 2 * ceil(m->GetTLOC() / (r->getSpeed() * 25.0)) + m->GetMDUR());
}



void Mars_Station::read_input() {
	input_output.readInput(&event_list);
	assRovers();
}

void Mars_Station::eventExc() {

	Event* excteone;
	while (event_list.peek(excteone)) {

		if (excteone->getED() == INFO.CurrentDay) {

			event_list.dequeue(excteone);
			// based on event action will be done (promotion , cancellation , formulation )
			F_event* fChossen = dynamic_cast <F_event*>(excteone);
			if (fChossen) {

				switch (fChossen->mis_type())
				{

				case MOUNTANIOUS:
					fChossen->Execute(mMission, 1);
					INFO.totalMount++;
						break;

				case EMERGENCY:
					fChossen->Execute(eMission); break;

				case POLAR:
					fChossen->Execute(pMission); break;

				default:
					break;
				}

			}
			P_event* pChossen = dynamic_cast <P_event*>(excteone);
			if (pChossen) {
				pChossen->Execute(mMission, eMission);
			}
			C_event* cChossen = dynamic_cast <C_event*>(excteone);
			if (cChossen) {
				cChossen->Execute(mMission);
			}

		}
		else
			break;


	}

};

void Mars_Station::checkAutoP()
{
	if (!mMission->isEmpty()) {
		Mission* checkAutoPromo;
		mMission->peek(checkAutoPromo);
		bool autoPromot = (INFO.CurrentDay - checkAutoPromo->GetFD()) >= INFO.AutoP;


		while (autoPromot) {
			mMission->dequeue(checkAutoPromo);
			priorityInfo* tempPir = new  priorityInfo(checkAutoPromo->GetSIG(), checkAutoPromo->GetFD(), checkAutoPromo->GetMDUR(), checkAutoPromo->GetTLOC());
			checkAutoPromo->SetTYP(EMERGENCY);
			eMission->Push(checkAutoPromo, *tempPir);
			autoPmissionNum++;

			if (mMission->peek(checkAutoPromo)) {
				autoPromot = (INFO.CurrentDay - checkAutoPromo->GetFD()) >= INFO.AutoP;
			}
			else
				autoPromot = 0;

		}

	}

};




void Mars_Station::start()
{
	bool flag{ 0 };
	INFO.CurrentDay = 1;
	input_output.receiveMode();
	while (!flag) {
		// function to check if there is event in that day
		eventExc();

		//transfer completed mission and rovers
		stateRoversStatusFromExecution();
		
		checkAutoP();

		// check if any rover is avilable now
		endCheckUp();

		// check if any rover available for this mission
		assRovrsToMissionsAtDay();

		// Decrements maintenance duration of the rover that is in the maintenance duration
		decMaintenanceDur();

		switch (INFO.mode)
		{
		case (1 ) :
			//print day details
			printDayDetails(); break;

		case(2): 
			//print day details
			Sleep(1000);
			printDayDetails(); break;

		default:
			break;
		}

		// function to end program;
		flag = endProgram();


		INFO.CurrentDay++;
	}
	if (INFO.mode == 3)
		input_output.printSilentMode();


	outFile();
}






void Mars_Station::assRovrsToMissionsAtDay()
{
	Rover* curRover = nullptr;
	Mission* curMission = nullptr;
	bool available = 1;
	while (available && !eMission->empty())
	{
		
		if (eRover->dequeue(curRover) || mRover->dequeue(curRover) || pRover->dequeue(curRover))
		{
			eMission->getFront(curMission);
			assMissionToRover(curMission, curRover);
			if (curRover->getNeedsMaintanance()) {
				curRover->setNeedsMaintanance(0);
				curRover->setMaintanceDuration(0);
				curRover->setSpeed((1 / 2.0) * curRover->getSpeed());
			}

		}
		else
			available = 0;
	}
	while (available && !mMission->isEmpty())
	{
		if (mRover->dequeue(curRover))
		{
			mMission->dequeue(curMission);
			assMissionToRover(curMission, curRover);
			if (curRover->getNeedsMaintanance()) {
				curRover->setNeedsMaintanance(0);
				curRover->setMaintanceDuration(0);
				curRover->setSpeed((1 / 2.0) * curRover->getSpeed());
			}

		}
		else
			available = 0;
	}
	available = 1;
	while (available && !pMission->isEmpty())
	{
		if (pRover->dequeue(curRover))
		{
			pMission->dequeue(curMission);
			assMissionToRover(curMission, curRover);
			if (curRover->getNeedsMaintanance()) {
				curRover->setNeedsMaintanance(0);
				curRover->setMaintanceDuration(0);
				curRover->setSpeed((1 / 2.0) * curRover->getSpeed());
			}

		}
		else
			available = 0;
	}
}


void Mars_Station::assRovers() {
	int numOfMountainRovers = INFO.rovers_num[MOUNTANIOUS];
	int numOfPolarRovers = INFO.rovers_num[POLAR];
	int numOfEmergenRovers = INFO.rovers_num[EMERGENCY];
	int speedofmountainous = INFO.rovers_speed[MOUNTANIOUS];
	int speedofPoler = INFO.rovers_speed[POLAR];
	int speedofEmegency = INFO.rovers_speed[EMERGENCY];
	int checkupformountionous = INFO.checkup_durations[MOUNTANIOUS];
	int checkupforPoler = INFO.checkup_durations[POLAR];
	int checkupforEmergen = INFO.checkup_durations[EMERGENCY];

	int ID = 0;
	for (int i = 0; i < numOfMountainRovers; i++) {
		Rover* newMRover = new Rover(++ID, INFO.missions_before_checkup, speedofmountainous, checkupformountionous, 1 , MOUNTANIOUS);
		mRover->enqueue(newMRover);
	}

	for (int i = 0; i < numOfPolarRovers; i++) {
		Rover* newPRover = new Rover(++ID, INFO.missions_before_checkup, speedofPoler, checkupforPoler, 1 , POLAR);
		pRover->enqueue(newPRover);
	}

	for (int i = 0; i < numOfEmergenRovers; i++) {
		Rover* newERover = new Rover(++ID, INFO.missions_before_checkup, speedofEmegency, checkupforEmergen, 1 , EMERGENCY);
		eRover->enqueue(newERover);
	}
};


void Mars_Station::printDayDetails() {
	input_output.printCurrentDay(INFO.CurrentDay);
	input_output.printWaitingMissions(mMission, pMission, eMission);
	input_output.printBreak();
	input_output.printIn_ExecMissionsAndRovers(In_ExecutionMissions);
	input_output.printBreak();
	input_output.printAvailableRovers(mRover, pRover, eRover);
	input_output.printBreak();
	input_output.printInCheckUpRovers(CheckUpRovers);
	input_output.printBreak();
	input_output.printCompletedMissions(completedMissions);
	
}

void Mars_Station::outFile()
{
	Avgcalc(AvgWait , AvgExc);
	ofstream myFile;
	myFile.open("Output.txt");
	input_output.outputFile(myFile , completedMissions);
	input_output.printStatics(myFile , roversNum , missionsNum , autoPmissionNum , AvgWait, AvgExc);
	myFile.close();

}

bool Mars_Station::endProgram()
{
	return ((mMission->isEmpty()) 
		&& (pMission->isEmpty()) 
		&& (eMission->empty()) 
		&& (In_ExecutionMissions->empty()) 
		&& (CheckUpRovers->empty()) 
		&& (event_list.isEmpty()));
}

void Mars_Station::stateRoversStatusFromExecution()
{
	Rover* rov;
	Mission* mis;

	while (In_ExecutionMissions->peek(mis) && mis->endMission())
	{
		In_ExecutionMissions->getFront(mis);
		mis->SetCD(INFO.CurrentDay);
		mis->SetED(INFO.CurrentDay - mis->getAssignedDay());
		completedMissions->enqueue(mis);
		rov = mis->getRover();
		if (rov->getNumberOfMission() == 0)
		{
			rov->setNumberOfMission(INFO.missions_before_checkup);
			rov->startCheckUP();
			CheckUpRovers->Push(rov, rov->getCheckUpDuration() + INFO.CurrentDay );
		}
		else
		{
			switch (rov->getType())
			{
			case EMERGENCY:
				rov->setAvailable(true);
				eRover->enqueue(rov);
				break;
			case MOUNTANIOUS:
				rov->setAvailable(true);
				mRover->enqueue(rov);
				break;
			case POLAR:
				rov->setAvailable(true);
				pRover->enqueue(rov);
				break;
			default:
				cout << "Something wrong in rover status";
			}
		}

	}


	while ( In_ExecutionMissions->peek(mis) && missionFailure(mis) )
	{
		In_ExecutionMissions->getFront(mis);
		rov = mis->getRover();
		rov->setNumberOfMission(INFO.missions_before_checkup);
		rov->setmaintenanceCount(0);
		rov->startCheckUP();
		CheckUpRovers->Push(rov, rov->getCheckUpDuration());
		F_event* evento = nullptr;
		evento = new F_event(mis->GetTYP(), mis->GetFD(), mis->GetID(), mis->GetTLOC(), mis->GetMDUR(), mis->GetSIG());
		switch (mis->GetTYP())
		{

		case MOUNTANIOUS:
			evento->Execute(mMission, 1); break;

		case EMERGENCY:
			evento->Execute(eMission); break;

		case POLAR:
			evento->Execute(pMission); break;

		default:
			break;
		}

	}



}


void Mars_Station::endCheckUp()
{
	Rover* temp;
	while (CheckUpRovers->peek(temp) && temp->endCheckUP())
	{
		CheckUpRovers->getFront(temp);
		if (temp->getType() == EMERGENCY)
		{
			temp->setAvailable(true);
			eRover->enqueue(temp);
		}
		else if (temp->getType() == MOUNTANIOUS)
		{
			temp->setAvailable(true);
			mRover->enqueue(temp);
		}
		else if (temp->getType() == POLAR)
		{
			temp->setAvailable(true);
			pRover->enqueue(temp);
		}
	}
}


void Mars_Station::Avgcalc(double & AvgWait, double & AvgExc)
{
	AvgWait = 0;
	AvgExc = 0;
	int num{0};

	Mission* temp;
	LinkedQueue <Mission* >* temporaryone = new  LinkedQueue <Mission* >();
	while (!completedMissions->isEmpty())
	{
		completedMissions->dequeue(temp);
		AvgWait += temp->GetWD();
		AvgExc += temp->GetED();
		temporaryone->enqueue(temp);
		num++;
	}
	while (!temporaryone->isEmpty())
	{
		temporaryone->dequeue(temp);
		completedMissions->enqueue(temp);
	}
	if (num != 0) {
		AvgWait /= num;
		AvgExc /= num;
	}

}


bool Mars_Station::missionFailure(Mission* mis)
{
	Rover* rov;
	rov = mis->getRover();
	if (rov->getmaintenanceCount() % 3 == 0 && rov->getmaintenanceCount() )
		return true;
	else
		return false;
}

void Mars_Station::decMaintenanceDur() {
	LinkedQueue<Rover* >* inMaintenanceRoverTemp = new LinkedQueue<Rover* >();
	Rover* Temp = nullptr;
	while (inMaintenanceRover->dequeue(Temp)) {
		if (Temp->getAvailable()) {
			Temp->setMaintanceDuration(Temp->getMaintanceDuration() - 1);
			if (Temp->getMaintanceDuration() == 0) {
				Temp->setNeedsMaintanance(false);
			}
		}
		inMaintenanceRoverTemp->enqueue(Temp);
	}
	while (inMaintenanceRoverTemp->dequeue(Temp)) {
		if (Temp->getNeedsMaintanance())
			inMaintenanceRover->enqueue(Temp);
	}

}

Mars_Station::~Mars_Station()
{
	while (!completedMissions->isEmpty()) {
		Mission* tempMissionToBeDeleted = nullptr;
		completedMissions->dequeue(tempMissionToBeDeleted);
		delete tempMissionToBeDeleted;
	}
	Rover* tempRoverToBeDeleted = nullptr;
	while (!mRover->isEmpty())
	{
		mRover->dequeue(tempRoverToBeDeleted);
		delete tempRoverToBeDeleted;
	}
	while (!pRover->isEmpty())
	{
		pRover->dequeue(tempRoverToBeDeleted);
		delete tempRoverToBeDeleted;
	}
	while (!eRover->isEmpty())
	{
		eRover->dequeue(tempRoverToBeDeleted);
		delete tempRoverToBeDeleted;
	}
	delete mMission;
	delete pMission;
	delete eMission;
	delete In_ExecutionMissions;
	delete completedMissions;
	delete mRover;
	delete pRover;
	delete eRover;
	delete CheckUpRovers;
	delete inMaintenanceRover;
	mMission = nullptr;
	pMission = nullptr;
	eMission = nullptr;
	In_ExecutionMissions = nullptr;
	completedMissions = nullptr;
	mRover = nullptr;
	pRover = nullptr;
	eRover = nullptr;
	CheckUpRovers = nullptr;
	inMaintenanceRover = nullptr;
}

