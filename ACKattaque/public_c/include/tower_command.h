#ifndef TOWER_COMMAND_H
#define TOWER_COMMAND_H

#include "game_message.h"

typedef struct {
    Transmission* transmissions;
    int transmissions_count;
} TowerCommand;

#endif // TOWER_COMMAND_H
