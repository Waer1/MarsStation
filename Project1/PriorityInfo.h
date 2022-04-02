#pragma once

class priorityInfo
{
private:
    int SIG,   //mission significance
        FDAY,  //mission formulation day
        DUR,   //mission duration
        LOC;   //mission target location
public:

    priorityInfo(int s = 0, int fd = 0, int dur = 0, int loc = 0) : SIG(s), FDAY(fd), DUR(dur), LOC(loc) {}

    bool operator==(const priorityInfo& PI)
    {
        return LOC == PI.LOC && SIG == PI.SIG && DUR == PI.DUR && FDAY == PI.FDAY;
    }
    bool operator>(const priorityInfo& PI)
    {
        return (SIG < PI.SIG)
            || (SIG == PI.SIG && FDAY > PI.FDAY)
            || (SIG == PI.SIG && FDAY == PI.FDAY && DUR > PI.DUR)
            || (SIG == PI.SIG && FDAY == PI.FDAY && DUR == PI.DUR && LOC > PI.LOC);
    }
    bool operator>=(const priorityInfo& PI)
    {
        return (*this == PI)
            || (*this > PI);
    }

    bool operator<(const priorityInfo& PI)
    {
        return !(*this >= PI);
    }
    bool operator<=(const priorityInfo& PI)
    {
        return (*this < PI)
            || (*this == PI);
    }
};


