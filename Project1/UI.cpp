#include "UI.h"
void UI::readLine(int* ptr, ifstream& reader) {
	for (int i = 0; i < COUNT_MISSIONS_TYPES; i++) {
		reader >> ptr[i];
	}
}

void UI::receiveMode()
{

	mciSendString(TEXT("open \"Welcome to Mars Station.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3"), NULL, 0, NULL);

	int mode{1},c;
	printtype("Select The Program Mode  ");
	printtype("->> Interactive Mode");
	printtype("    Step-By-Step Mode");
	printtype("    Silent Mode");

	bool flag{ 1 };

	while(flag){
		c = _getch();
		system("CLS");
			switch (c ) {
	case KEY_UP:
		mode = max(1,mode-1);
		break;
	case KEY_DOWN:
		mode = min(3, mode + 1);
		break;
	case KEY_ENTER:
		flag = 0;
		break;
	}

			if (mode == 1) {
				cout << "Select The Program Mode "<<endl;
				cout << "->> Interactive Mode" <<endl;
				cout << "    Step-By-Step Mode"<<endl;
				cout << "    Silent Mode"<<endl;
			}
			else if (mode == 2) {
				cout << "Select The Program Mode " << endl;
				cout << "    Interactive Mode" << endl;
				cout << "->> Step-By-Step Mode" << endl;
				cout << "    Silent Mode" << endl;
			}
			else {
				cout << "Select The Program Mode " << endl;
				cout << "    Interactive Mode" << endl;
				cout << "    Step-By-Step Mode" << endl;
				cout << "->> Silent Mode" << endl;
			}
	}
	mciSendString(TEXT("close mp3"), NULL, 0, NULL);
	switch (mode)
	{
	case 1: 
		mciSendString(TEXT("open \"interactive mode is selected.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
		mciSendString(TEXT("play mp3"), NULL, 0, NULL);
		printtype("Interactive Mode IS Selected");
		break;
	case 2:
		mciSendString(TEXT("open \"Step by step mode is selected.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
		mciSendString(TEXT("play mp3"), NULL, 0, NULL);
		printtype("Step by step mode is selected");
		break;
	case 3:
		mciSendString(TEXT("open \"Silent mode is selected.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
		mciSendString(TEXT("play mp3"), NULL, 0, NULL);
		printtype("Silent mode is selected");
		break;
	}

	system("CLS");
	INFO.mode = mode;
}

void UI::readInput(LinkedQueue<Event*>* events) {
	int events_CNT;
	ifstream reader;
	reader.open("info.txt", ios::in);
	if (reader.is_open()) {
		readLine(INFO.rovers_num, reader);
		readLine(INFO.rovers_speed, reader);
		reader >> INFO.missions_before_checkup;
		readLine(INFO.checkup_durations, reader);
		reader >> INFO.AutoP;
		INFO.totalMount = 0;

		reader >> events_CNT;
		for (int i = 0; i < events_CNT; i++) {
			char event_type;
			reader >> event_type;
			Event* temp = nullptr;
			char mission_type;
			MISSIONS_TYPES missionType;
			int ED, ID, TLOC, MDUR, SIG;
			switch (event_type) {
			case 'F':
				reader >> mission_type >> ED >> ID >> TLOC >> MDUR >> SIG;
				missionType = mission_type == 'E' ? EMERGENCY : (mission_type == 'P' ? POLAR : MOUNTANIOUS);
				temp = new F_event(missionType, ED, ID, TLOC, MDUR, SIG);
				break;
			case 'P':
				reader >> ED >> ID;
				temp = new P_event(ED, ID);
				break;
			case 'X':
				reader >> ED >> ID;
				temp = new C_event(ED, ID);
				break;
			default:
				cout << "something wrong in file input........";
				break;
			}
			events->enqueue(temp);
		}
	}
	else {
		cout << "missed input file\n";
	}
	SetConsoleTitle(TEXT("Mars Station"));

	system("Color 00");
	SetColor(9);
}

void UI::printAvailableRovers(LinkedQueue<Rover* >* mR, LinkedQueue<Rover* >* pR, LinkedQueue<Rover* >* eR) {
	LinkedQueue<Rover* >* mRTemp = new LinkedQueue<Rover* >();
	LinkedQueue<Rover* >* pRTemp = new LinkedQueue<Rover* >();
	LinkedQueue<Rover* >* eRTemp = new LinkedQueue<Rover* >();
	int mRCnt = 0; int pRCnt = 0; int eRCnt = 0;

	while (!mR->isEmpty()) {
		Rover* mTemp;
		mR->dequeue(mTemp);
		mRTemp->enqueue(mTemp);
		mRCnt++;
	}
	while (!pR->isEmpty()) {
		Rover* pTemp;
		pR->dequeue(pTemp);
		pRTemp->enqueue(pTemp);
		pRCnt++;
	}
	while (!eR->isEmpty()) {
		Rover* eTemp;
		eR->dequeue(eTemp);
		eRTemp->enqueue(eTemp);
		eRCnt++;
	}

	cout << (mRCnt + pRCnt + eRCnt) << " Available Rovers: ";
	
	SetColor(11);
	if (!eRTemp->isEmpty()) {
		int i = 0;
		cout << "[";
		while (!eRTemp->isEmpty()) {
			i++;
			Rover* eTemp;
			eRTemp->dequeue(eTemp);
			cout << eTemp->getID();
			if (i == eRCnt)
				cout << "] ";
			else
				cout << ", ";
			eR->enqueue(eTemp);

		}
	}

	SetColor(6);;
	if (!pRTemp->isEmpty()) {
		int i = 0;
		cout << "(";
		while (!pRTemp->isEmpty()) {
			i++;
			Rover* pTemp;
			pRTemp->dequeue(pTemp);
			cout << pTemp->getID();
			if (i == pRCnt)
				cout << ") ";
			else
				cout << ", ";
			pR->enqueue(pTemp);

		}
	}

	SetColor(10);
	if (!mRTemp->isEmpty()) {
		int i = 0;
		cout << "{";
		while (!mRTemp->isEmpty()) {
			i++;
			Rover* mTemp;
			mRTemp->dequeue(mTemp);
			cout << mTemp->getID();
			if (i == mRCnt)
				cout << "} ";
			else
				cout << ", ";
			mR->enqueue(mTemp);

		}
	}
	SetColor(9);
}

void UI::printInCheckUpRovers(priorityQueue<Rover*>* CUR) {

	priorityQueue<Rover*>* Templist = new priorityQueue<Rover*>();
	CUR->CloneList(Templist);

	LinkedQueue<Rover*>* CURE = new LinkedQueue<Rover*>();
	LinkedQueue<Rover*>* CURP = new LinkedQueue<Rover*>();
	LinkedQueue<Rover*>* CURM = new LinkedQueue<Rover*>();
	int InCheckCnt = 0;
	while (!Templist->empty()) {
		int pr = 0;
		Rover* Temp;
		Templist->getFront(Temp);
		if (Temp->getType() == EMERGENCY)
			CURE->enqueue(Temp);
		if (Temp->getType() == POLAR)
			CURP->enqueue(Temp);
		if (Temp->getType() == MOUNTANIOUS)
			CURM->enqueue(Temp);
		InCheckCnt++;
	}

	cout << InCheckCnt << " In-Checkup Rovers: ";
	SetColor(11);
	if (!CURE->isEmpty()) {
		cout << "[";
		while (!CURE->isEmpty()) {
			int prE = 0;
			Rover* Tmp;
			CURE->dequeue(Tmp);
			cout << Tmp->getID();
			if (!CURE->isEmpty())
				cout << ", ";
			else
				cout << "] ";
		}
	}
	SetColor(6);;
	if (!CURP->isEmpty()) {
		cout << "(";
		while (!CURP->isEmpty()) {
			int prP = 0;
			Rover* Tmp;
			CURP->dequeue(Tmp);
			cout << Tmp->getID();
			if (!CURP->isEmpty())
				cout << ", ";
			else
				cout << ") ";
		}
	}
	SetColor(10);
	if (!CURM->isEmpty()) {
		cout << "{";
		while (!CURM->isEmpty()) {
			int prM = 0;
			Rover* Tmp;
			CURM->dequeue(Tmp);
			cout << Tmp->getID();
			if (!CURM->isEmpty())
				cout << ", ";
			else
				cout << "} ";
		}
	}
	SetColor(9);
}

void UI::printCompletedMissions(LinkedQueue<Mission*>* cM)
{
	LinkedQueue<Mission*>* CME = new LinkedQueue<Mission*>();
	LinkedQueue<Mission*>* CMP = new LinkedQueue<Mission*>();
	LinkedQueue<Mission*>* CMM = new LinkedQueue<Mission*>();
	int completedCnt = 0;

	LinkedQueue<Mission*>* Temp = new LinkedQueue<Mission*>();


	while (!cM->isEmpty()) {
		Mission* Tmp;
		cM->dequeue(Tmp);
		if (Tmp->GetTYP() == EMERGENCY)
			CME->enqueue(Tmp);
		if (Tmp->GetTYP() == POLAR)
			CMP->enqueue(Tmp);
		if (Tmp->GetTYP() == MOUNTANIOUS)
			CMM->enqueue(Tmp);
		completedCnt++;
		Temp->enqueue(Tmp);
	}

	cout << completedCnt << " Completed Missions: ";
	SetColor(11);
	if (!CME->isEmpty()) {
		cout << "[";
		while (!CME->isEmpty()) {
			Mission* TmpE;
			CME->dequeue(TmpE);
			cout << TmpE->GetID();
			if (!CME->isEmpty())
				cout << ", ";
			else
				cout << "] ";
		}
	}
	SetColor(6);
	if (!CMP->isEmpty()) {
		cout << "(";
		while (!CMP->isEmpty()) {
			Mission* TmpP;
			CMP->dequeue(TmpP);
			cout << TmpP->GetID();
			if (!CMP->isEmpty())
				cout << ", ";
			else
				cout << ") ";
		}

	}
	SetColor(10);
	if (!CMM->isEmpty()) {
		cout << "{";
		while (!CMM->isEmpty()) {
			Mission* TmpM;
			CMM->dequeue(TmpM);
			cout << TmpM->GetID();
			if (!CMM->isEmpty())
				cout << ", ";
			else
				cout << "} ";
		}
	}

	while (!Temp->isEmpty()) {
		Mission* TMP;
		Temp->dequeue(TMP);
		cM->enqueue(TMP);
	}

	SetColor(9);

}

void UI::outputFile(ofstream &myFile , LinkedQueue<Mission*>* cM)
{
	myFile << "CD    ID    FD    WD    ED\n"; // write in output file
	while (!cM->isEmpty()) {
		Mission* Temp;	cM->dequeue(Temp);
		myFile << Temp->GetCD() << "    " << Temp->GetID() << "     " << Temp->GetFD() << "     " << Temp->GetWD()
			<< "     " << Temp->GetED() << endl;
	}
	myFile << "………………………………………………………………………\n" << "………………………………………………………………………\n";
}

void UI::printStatics(ofstream& myFile, int* roversNum, int* missionsNum, int autoPmissionNum , double  AvgWait , double AvgExc)
{
	int missionNum = missionsNum[MOUNTANIOUS] + missionsNum[POLAR] + missionsNum[EMERGENCY];
	int roverNum = INFO.rovers_num[MOUNTANIOUS] + INFO.rovers_num[POLAR] + INFO.rovers_num[EMERGENCY];
	myFile << "Missions: " << missionNum << " [M: " << missionsNum[MOUNTANIOUS] << ", " << "P: " << missionsNum[POLAR] << ", " << "E: " << missionsNum[EMERGENCY] << "]" << endl;
	myFile << "Rovers: " << roverNum << " [M: " << INFO.rovers_num[MOUNTANIOUS] << ", " << "P: " << INFO.rovers_num[POLAR] << ", " << "E: " << INFO.rovers_num[EMERGENCY] << "]" << endl;
	float P = 0;
	if(INFO.totalMount != 0)
		P = (float(autoPmissionNum) / float(INFO.totalMount)) * 100;
	myFile << "Avg Wait : " << AvgWait << "\t" << " , Avg Exc : " << AvgExc << endl;
	myFile << "Auto-promoted: " << P << " %" << endl;

	mciSendString(TEXT("close mp3"), NULL, 0, NULL);
	mciSendString(TEXT("open \"Program ran successfully.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3"), NULL, 0, NULL);
	Sleep(3000);

}

void UI::printCurrentDay(int d)
{
	//system("Color F5");
	cout << endl << endl << "Current Day: " << d << endl;
}

void UI::printBreak()
{
	cout << endl;
	cout << "----------------------------------------------------" << endl;
}

void UI::printSilentMode()
{
	printtype("Silent Mode");
	printtype("Simulation Starts...");
	printtype("Simulation Ends, Output File Created");
}

void UI::printtype(string s)
{
	for (int i = 0; i < s.length(); i++) {
		Sleep(60);
		cout << s[i];
	}
	cout << endl;
}







void UI::printIn_ExecMissionsAndRovers(priorityQueue<Mission*>* I_EM) {

	priorityQueue<Mission*>* TempI = new priorityQueue<Mission*>();
	I_EM->CloneList(TempI);

	LinkedQueue<Mission*>* I_EME = new LinkedQueue<Mission*>();
	LinkedQueue<Mission*>* I_EMP = new LinkedQueue<Mission*>();
	LinkedQueue<Mission*>* I_EMM = new LinkedQueue<Mission*>();
	int InExecCnt = 0;
	while (!TempI->empty()) {
		int pr = 0;
		Mission* Temp = new Mission();
		TempI->getFront(Temp);
		if (Temp->GetTYP() == EMERGENCY)
			I_EME->enqueue(Temp);
		if (Temp->GetTYP() == POLAR)
			I_EMP->enqueue(Temp);
		if (Temp->GetTYP() == MOUNTANIOUS)
			I_EMM->enqueue(Temp);
		InExecCnt++;
	}


	cout << InExecCnt << " In-Execution Missions/Rovers: ";
	SetColor(11);
	if (!I_EME->isEmpty()) {
		cout << "[";
		while (!I_EME->isEmpty()) {
			int prE = 0;
			Mission* Tmp = new Mission();
			I_EME->dequeue(Tmp);
			cout << Tmp->GetID() << "/" << Tmp->getRoverIn_ExecutionID();
			if (!I_EME->isEmpty())
				cout << ", ";
			else
				cout << "] ";
		}
	}

	SetColor(6);;
	if (!I_EMP->isEmpty()) {
		cout << "(";
		while (!I_EMP->isEmpty()) {
			int prP = 0;
			Mission* Tmp = new Mission();
			I_EMP->dequeue(Tmp);
			cout << Tmp->GetID() << "/" << Tmp->getRoverIn_ExecutionID();
			if (!I_EMP->isEmpty())
				cout << ", ";
			else
				cout << ") ";
		}
	}

	SetColor(10);
	if (!I_EMM->isEmpty() ) {
		cout << "{";
		while (!I_EMM->isEmpty()) {
			int prM = 0;
			Mission* Tmp = new Mission();
			I_EMM->dequeue(Tmp);
			cout << Tmp->GetID() << "/" << Tmp->getRoverIn_ExecutionID();
			if (!I_EMM->isEmpty())
				cout << ", ";
			else
				cout << "} ";
		}
	}
	SetColor(9);
}

void UI::printWaitingMissions(LinkedQueue<Mission*>* mM, LinkedQueue<Mission*>* pM, priorityQueue<Mission*, priorityInfo>* eM)
{
	LinkedQueue<Mission* >* mMTemp = new LinkedQueue<Mission* >();
	LinkedQueue<Mission*>* pMTemp = new LinkedQueue<Mission*>();
	priorityQueue<Mission*, priorityInfo>* eMTempCount = new priorityQueue<Mission*, priorityInfo>();
	eM->CloneList(eMTempCount);
	priorityQueue<Mission*, priorityInfo>* eMTempDisplay = new priorityQueue<Mission*, priorityInfo>();
	eM->CloneList(eMTempDisplay);
	int mMCnt = 0; int pMCnt = 0; int eMCnt = 0;


	while (!mM->isEmpty()) {
		Mission* TempM;
		mM->dequeue(TempM);
		mMTemp->enqueue(TempM);
		mMCnt++;
	}
	while (!pM->isEmpty()) {
		Mission* pTemp;
		pM->dequeue(pTemp);
		pMTemp->enqueue(pTemp);
		pMCnt++;
	}
	while (!eMTempCount->empty()) {
		eMTempCount->del();
		eMCnt++;
	}
	cout << (mMCnt + pMCnt + eMCnt) << " Waiting Missions: ";
	SetColor(11);
	if (!eMTempDisplay->empty()) {
		int i = 0;
		cout << "[";
		while (!eMTempDisplay->empty()) {
			i++;
			Mission* eTemp;
			eMTempDisplay->getFront(eTemp);
			cout << eTemp->GetID();
			if (i == eMCnt)
				cout << "] ";
			else
				cout << ", ";
		}
	}

	SetColor(6);;
	if (!pMTemp->isEmpty()) {
		int i = 0;
		cout << "(";
		while (!pMTemp->isEmpty()) {
			i++;
			Mission* pTemp;
			pMTemp->dequeue(pTemp);
			cout << pTemp->GetID();
			if (i == pMCnt)
				cout << ") ";
			else
				cout << ", ";
			pM->enqueue(pTemp);

		}
	}



	SetColor(10);
	if (!mMTemp->isEmpty()) {
		int i = 0;
		cout << "{";
		while (!mMTemp->isEmpty()) {
			i++;
			Mission* TmpM;
			mMTemp->dequeue(TmpM);
			cout << TmpM->GetID();
			if (i == mMCnt)
				cout << "} ";
			else
				cout << ", ";
			mM->enqueue(TmpM);

		}
	}

	SetColor(9);
}



void UI::SetColor(int ForgC)
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd     color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
