#include "Mission.h"

Mission::Mission(int id, int cd, int fd, int tloc, int mdur, int sig, bool assigned, bool completed, MISSIONS_TYPES typ)
	: ID(id), CD(cd), FD(fd), TLOC(tloc), MDUR(mdur), SIG(sig), Assigned(assigned), Completed(completed), TYP(typ)
{
	WD = -1;
}

void Mission::SetID(int id) {
	ID = id;
}

int Mission::GetID() {
	return ID;
}

void Mission::SetFD(int fd) {
	FD = fd;
}


int Mission::GetFD() {
	return FD;
}


int Mission::GetWD() {
	return WD;
}

void Mission::SetTLOC(int tloc) {
	TLOC = tloc;
}

int Mission::GetTLOC() {
	return TLOC;
}

void Mission::SetMDUR(int mdur) {
	MDUR = mdur;
}

int Mission::GetMDUR() {
	return MDUR;
}

void Mission::SetSIG(int sig) {
	SIG = sig;
}

int Mission::GetSIG() {
	return SIG;
}

void Mission::SetAssigned(bool assigned) {
	Assigned = assigned;
}

bool Mission::GetAssigned() {
	return Assigned;
}

void Mission::SetCompleted(bool completed) {
	Completed = completed;
}

bool Mission::GetCompleted() {
	return Completed;
}

void Mission::SetTYP(MISSIONS_TYPES typ) {
	TYP = typ;
}

MISSIONS_TYPES Mission::GetTYP() {
	return TYP;
}


void Mission::print() {

	std::cout << GetTYP() << "  " << GetID() << "  " << GetFD() << std::endl;

}

Rover* Mission::getRover()
{
	return inExecutionRover;
}

void Mission::addRover(Rover* r)
{
	inExecutionRover = r;

}

void Mission::setWD(int wd) {
	WD = wd;
};

