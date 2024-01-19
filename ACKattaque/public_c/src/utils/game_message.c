#include "game_message.h"


ACKit get_ACKit_from_string(char *string) {
    for (long unsigned int i = 0; i < sizeof(ACKitMap) / sizeof(ACKitMap[0]); i++) {
        if (strcmp(string, ACKitMap[i].string) == 0) {
            return ACKitMap[i].type;
        }
    }
    return -1;
}

TileType get_TileType_from_string(char *string) {
    for (long unsigned int i = 0; i < sizeof(TileTypeMap) / sizeof(TileTypeMap[0]); i++) {
        if (strcmp(string, TileTypeMap[i].string) == 0) {
            return TileTypeMap[i].type;
        }
    }
    return -1;
}

const char **get_ACKit_values() {
    return ACKitStrings;
}

bool contains_landmark(Tile* tile) {
    return tile->landmark != NULL;
}

bool contains_spaceship(Tile* tile) {
    return tile->spaceship != NULL;
}

Tile* get_tile(Map *map, Coords *coords) {
    return &map->tiles[coords->x][coords->y];
}

TileType get_tile_type(Map *map, Coords *coords) {
    return map->tiles[coords->x][coords->y].type;
}
