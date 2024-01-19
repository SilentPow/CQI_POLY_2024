#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H

// X macros to prevent retyping

#define FOREACH_ACKIT(ACKIT) \
        ACKIT(TINKWINKI) \
        ACKIT(LALAR)     \
        ACKIT(DISPI)     \
        ACKIT(POHO)      \
        ACKIT(NONONO)    \

#define FOREACH_TILE_TYPE(TILE_TYPE) \
        TILE_TYPE(EMPTY) \
        TILE_TYPE(WALL)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_STR2INT(ENUM) { #ENUM, ENUM },

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum {
    FOREACH_ACKIT(GENERATE_ENUM)
} ACKit;

static const char *ACKitStrings[] __attribute__((unused)) = {
    FOREACH_ACKIT(GENERATE_STRING)
};

static const struct {
    const char *string;
    ACKit type;
} ACKitMap[] = {
    FOREACH_ACKIT(GENERATE_STR2INT)
};

typedef struct {
    ACKit* value;
    int size;
} ACKStream;

typedef struct {
    int x;
    int y;
} Coords;

typedef enum {
    FOREACH_TILE_TYPE(GENERATE_ENUM)
} TileType;

static const char *TileTypeStrings[] __attribute__((unused)) = {
    FOREACH_TILE_TYPE(GENERATE_STRING)
};

static const struct {
    const char *string;
    TileType type;
} TileTypeMap[] = {
    FOREACH_TILE_TYPE(GENERATE_STR2INT)
};

typedef struct {
    ACKStream *id;
    char *name;
    Coords coords;
} Landmark;

typedef struct {
    ACKit team_id;
    int spaceship_id;
} SpaceshipSighting;

typedef struct {
    TileType type;
    Landmark* landmark;
    SpaceshipSighting* spaceship;
    bool contains_pickup;
    bool contains_dropoff;
} Tile;

typedef struct {
    Tile** tiles;
    int rows;
    int cols;
} Map;

typedef struct {
    ACKStream *frequency;
    ACKStream *message;
} Transmission;

typedef struct {
    int score;
    Coords start;
    Coords end;
} Objective;

typedef struct {
    ACKit id;
    Coords tower_position;
    int score;
} Team;

typedef struct {
    Map* map;
    Landmark** landmarks;
    int landmarks_count;
    int game_step;
    int total_steps;
    Transmission* transmissions;
    int transmissions_count;
    int n_spaceships;
    Team* teams;
    int teams_count;
    ACKit my_team_id;
    Objective* objectives;
    int objectives_count;
} GameMessage;


// Functions

ACKit get_ACKit_from_string(char *string);
TileType get_TileType_from_string(char *string);
const char **get_ACKit_values();
bool contains_landmark(Tile* tile);
bool contains_spaceship(Tile* tile);
Tile* get_tile(Map *map, Coords *coords);
TileType get_tile_type(Map *map, Coords *coords);

#endif // GAME_MESSAGE_H
