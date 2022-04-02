#ifndef __INFO_H__
#define __INFO_H__
enum MISSIONS_TYPES {
    MOUNTANIOUS,
    POLAR,
    EMERGENCY,
    COUNT_MISSIONS_TYPES
};

__declspec(selectany) //This line to prevent "redefinition error"
struct project_info {
    int rovers_num[COUNT_MISSIONS_TYPES], rovers_speed[COUNT_MISSIONS_TYPES],
        missions_before_checkup, checkup_durations[COUNT_MISSIONS_TYPES],
        AutoP, CurrentDay, mode, totalMount;
    char openBrackets[COUNT_MISSIONS_TYPES], closedBrackets[COUNT_MISSIONS_TYPES];
} INFO;
#endif

