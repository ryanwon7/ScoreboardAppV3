#ifndef DIALOG_HEADER
#define DIALOG_HEADER

#include "scoreboard.h"
#include "resource.h"

typedef struct DialogData {
	timer_t timer_current;
	char homeName[28];
	char awayName[28];
	int homeScore;
	int awayScore;
	int homeTimeouts;
	int awayTimeouts;
	int period;
	timer_t timer_default;
	timer_t timeout_default;
	timer_t ot_default;
	char defaultHomeName[28];
	char defaultAwayName[28];
	int max_timeouts;
	int max_periods;
} DialogData;

void configurationDialog( HWND hwndParent );

#endif
