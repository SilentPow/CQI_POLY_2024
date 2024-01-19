#ifndef SPACESHIP_COMMAND_H
#define SPACESHIP_COMMAND_H

#define FOREACH_ACTION(ACTION) \
        ACTION(NORTH)   \
        ACTION(SOUTH)   \
        ACTION(WEST)    \
        ACTION(EAST)    \
        ACTION(DROP)    \
        ACTION(PICKUP)  \
        ACTION(BUY)     \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_message.h"


typedef enum {
    FOREACH_ACTION(GENERATE_ENUM)
} Action;

static const char *ActionStrings[] __attribute__((unused)) = {
    FOREACH_ACTION(GENERATE_STRING)
};

typedef struct {
    Transmission* transmissions;
    int transmissions_count;
    ACKStream *memory;
    Action action;
} SpaceshipCommand;

#endif // SPACESHIP_COMMAND_H
