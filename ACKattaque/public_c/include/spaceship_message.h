#ifndef SPACESHIP_MESSAGE_H
#define SPACESHIP_MESSAGE_H

#define FOREACH_SPACESHIP_TYPE(TYPE) \
        TYPE(HUMMINGBIRD)   \
        TYPE(DUCK)          \
        TYPE(ALBATROSS)     \
        TYPE(FLAMINGO)      \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_STR2INT(ENUM) { #ENUM, ENUM },

#include <stdbool.h>

#include "game_message.h"


typedef enum {
    FOREACH_SPACESHIP_TYPE(GENERATE_ENUM)
} SpaceshipType;

static const char *SpaceshipTypeStrings[] __attribute__((unused)) = {
    FOREACH_SPACESHIP_TYPE(GENERATE_STRING)
};

static const struct {
    const char *string;
    SpaceshipType type;
} SpaceshipTypeMap[] = {
    FOREACH_SPACESHIP_TYPE(GENERATE_STR2INT)
};

typedef struct {
    SpaceshipType type;
    Transmission* transmissions;
    int transmission_count;
    Tile **local_tiles;
    int local_tiles_dim;
    bool can_see_landmark;
    bool can_see_spaceship;
    bool carries_payload;
    ACKStream *memory;
    Coords *gps;
    Objective *objectives;
    int objectives_count;
} SpaceshipMessage;

SpaceshipType get_SpaceshipType_from_string(const char *spaceship_type_string);

#endif // SPACESHIP_MESSAGE_H
