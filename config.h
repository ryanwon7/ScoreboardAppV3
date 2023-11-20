#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include "scoreboard.h"

void loadConfigFileAndDefaults( ScoreboardControl* scoreboard_control, ScoreboardDesign* scoreboard_design);

void writeToConfig( ScoreboardControl* scoreboard_control );

void setDefaults( ScoreboardControl* scoreboard_control );

#endif
