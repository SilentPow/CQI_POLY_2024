#include <stdio.h>
#include "../cjson/cJSON.h"
#include "json.h"

ACKStream *_populate_ACKStream(const cJSON *ackstream_item);
Transmission *_populate_Transmissions(const cJSON *transmissions_item, int transmission_count);
SpaceshipSighting *_populate_SpaceshipSighting(const cJSON *spaceship_item);
Landmark *_populate_Landmark(const cJSON *landmark_item);
Tile **_populate_Tiles(const cJSON *tiles_item, int *rows, int *cols);

cJSON *_ackstream_to_json_obj(ACKStream *ackstream);
cJSON *_transmissions_to_json_obj(Transmission *transmissions, int count);


SpaceshipMessage *spaceship_message_from_json(char *spaceship_message_str) {
    const cJSON *transmissions_item = NULL;
    const cJSON *tiles_item = NULL;
    const cJSON *memory_item = NULL;

    cJSON *json = cJSON_Parse(spaceship_message_str);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    char* type_str = cJSON_GetObjectItem(json, "type")->valuestring;
    SpaceshipType type = get_SpaceshipType_from_string(type_str);
    bool can_see_landmark = cJSON_GetObjectItem(json, "can_see_landmark")->valueint;
    bool can_see_spaceship = cJSON_GetObjectItem(json, "can_see_spaceship")->valueint;
    bool carries_payload = cJSON_GetObjectItem(json, "carries_payload")->valueint;

    transmissions_item = cJSON_GetObjectItem(json, "transmissions");
    int transmission_count = cJSON_GetArraySize(transmissions_item);
    Transmission *transmissions = _populate_Transmissions(transmissions_item, transmission_count);

    tiles_item = cJSON_GetObjectItem(json, "local_tiles");
    int *row_count = malloc(sizeof(int));
    Tile **tiles = _populate_Tiles(tiles_item, row_count, row_count);
    
    memory_item = cJSON_GetObjectItem(json, "memory");
    ACKStream *memory = _populate_ACKStream(memory_item);

    cJSON *gps_item = cJSON_GetObjectItem(json, "gps");
    Coords *gps = NULL;
    if (!cJSON_IsNull(gps_item)) {
        gps = malloc(sizeof(Coords));
        gps->x = cJSON_GetObjectItem(gps_item, "x")->valueint;
        gps->y = cJSON_GetObjectItem(gps_item, "y")->valueint;
    }

    cJSON *objectives_item = cJSON_GetObjectItem(json, "objectives");
    cJSON *objective_item = NULL;

    int objectives_count = cJSON_GetArraySize(objectives_item);
    Objective *objectives = malloc(sizeof(Objective) * objectives_count);
    int i = 0;

    cJSON_ArrayForEach(objective_item, objectives_item) {
        objectives[i].score = cJSON_GetObjectItem(objective_item, "score")->valueint;
        objectives[i].start.x = cJSON_GetObjectItem(objective_item, "start")->valueint;
        objectives[i].start.y = cJSON_GetObjectItem(objective_item, "start")->valueint;
        objectives[i].end.x = cJSON_GetObjectItem(objective_item, "end")->valueint;
        objectives[i++].end.y = cJSON_GetObjectItem(objective_item, "end")->valueint;
    }

    SpaceshipMessage *spaceship_message = malloc(sizeof(SpaceshipMessage));
    spaceship_message->type = type;
    spaceship_message->transmissions = transmissions;
    spaceship_message->transmission_count = transmission_count;
    spaceship_message->local_tiles = tiles;
    spaceship_message->local_tiles_dim = *row_count;
    spaceship_message->can_see_landmark = can_see_landmark;
    spaceship_message->can_see_spaceship = can_see_spaceship;
    spaceship_message->carries_payload = carries_payload;
    spaceship_message->memory = memory;
    spaceship_message->gps = gps;
    spaceship_message->objectives = objectives;
    spaceship_message->objectives_count = objectives_count;

    cJSON_Delete(json);
    free(row_count);

    return spaceship_message;
}

GameMessage *game_message_from_json(char *game_message_str) {
    const cJSON *map_item = NULL;
    const cJSON *tiles_item = NULL;
    const cJSON *landmarks_item = NULL;
    const cJSON *landmark_item = NULL;
    const cJSON *transmissions_item = NULL;

    cJSON *json = cJSON_Parse(game_message_str);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    map_item = cJSON_GetObjectItem(json, "map");
    tiles_item = cJSON_GetObjectItem(map_item, "tiles");

    int *row_count = malloc(sizeof(int));
    int *col_count = malloc(sizeof(int));
    Tile **tiles = _populate_Tiles(tiles_item, row_count, col_count);

    Map *map = malloc(sizeof(Map));
    map->tiles = tiles;
    map->rows = *row_count;
    map->cols = *col_count;

    free(row_count); free(col_count);

    landmarks_item = cJSON_GetObjectItem(json, "landmarks");
    int landmark_count = cJSON_GetArraySize(landmarks_item);
    Landmark **landmarks = malloc(sizeof(Landmark*) * landmark_count);

    int i = 0;
    cJSON_ArrayForEach(landmark_item, landmarks_item) {
        landmarks[i++] = _populate_Landmark(landmark_item);
    }

    int game_step = cJSON_GetObjectItem(json, "game_step")->valueint;
    int total_steps = cJSON_GetObjectItem(json, "total_steps")->valueint;

    transmissions_item = cJSON_GetObjectItem(json, "transmissions");
    int transmission_count = cJSON_GetArraySize(transmissions_item);
    Transmission *transmissions = _populate_Transmissions(transmissions_item, transmission_count);

    int n_spaceships = cJSON_GetObjectItem(json, "n_spaceships")->valueint;

    const cJSON *teams_item = cJSON_GetObjectItem(json, "teams");
    int teams_count = cJSON_GetArraySize(teams_item);
    Team *teams = malloc(sizeof(Team) * teams_count);

    i = 0;
    const cJSON *team_item = NULL;
    const cJSON *tower_position_item = NULL;

    cJSON_ArrayForEach(team_item, teams_item) {
        tower_position_item = cJSON_GetObjectItem(team_item, "tower_position");

        teams[i].id = get_ACKit_from_string(cJSON_GetObjectItem(team_item, "id")->valuestring);
        teams[i].tower_position.x = cJSON_GetObjectItem(tower_position_item, "x")->valueint;
        teams[i].tower_position.y = cJSON_GetObjectItem(tower_position_item, "y")->valueint;
        teams[i].score = cJSON_GetObjectItem(team_item, "score")->valueint;
    }

    const cJSON *my_team_id_item = cJSON_GetObjectItem(json, "my_team_id");
    ACKit my_team_id = get_ACKit_from_string(my_team_id_item->valuestring);

    const cJSON *objectives_item = cJSON_GetObjectItem(json, "objectives");
    int objectives_count = cJSON_GetArraySize(objectives_item);
    Objective *objectives = malloc(sizeof(Objective) * objectives_count);

    i = 0;
    const cJSON *objective_item = NULL;

    cJSON_ArrayForEach(objective_item, objectives_item) {
        objectives[i].score = cJSON_GetObjectItem(objective_item, "score")->valueint;
        objectives[i].start.x = cJSON_GetObjectItem(objective_item, "start")->valueint;
        objectives[i].start.y = cJSON_GetObjectItem(objective_item, "start")->valueint;
        objectives[i].end.x = cJSON_GetObjectItem(objective_item, "end")->valueint;
        objectives[i++].end.y = cJSON_GetObjectItem(objective_item, "end")->valueint;
    }

    GameMessage *game_message = malloc(sizeof(GameMessage));
    game_message->map = map;
    game_message->landmarks = landmarks;
    game_message->landmarks_count = landmark_count;
    game_message->game_step = game_step;
    game_message->total_steps = total_steps;
    game_message->transmissions = transmissions;
    game_message->transmissions_count = transmission_count;
    game_message->n_spaceships = n_spaceships;
    game_message->teams = teams;
    game_message->teams_count = teams_count;
    game_message->my_team_id = my_team_id;
    game_message->objectives = objectives;
    game_message->objectives_count = objectives_count;

    cJSON_Delete(json);

    return game_message;
}

char *spaceship_command_to_json(SpaceshipCommand *spaceship_command) {
    cJSON *json = cJSON_CreateObject();
    cJSON *transmissions_item = NULL;
    cJSON *ackstream_item = NULL;

    ACKStream *ackstream_tmp = NULL;
    char *json_str = NULL;

    Transmission *transmissions = spaceship_command->transmissions;
    int transmissions_count = spaceship_command->transmissions_count;
    transmissions_item = _transmissions_to_json_obj(transmissions, transmissions_count);
    if (transmissions_item == NULL) goto end;
    cJSON_AddItemToObject(json, "transmissions", transmissions_item);

    ackstream_tmp = spaceship_command->memory;
    ackstream_item = _ackstream_to_json_obj(ackstream_tmp);
    cJSON_AddItemToObject(json, "memory", ackstream_item);

    const char *action = ActionStrings[spaceship_command->action];
    cJSON_AddStringToObject(json, "action", action);

    json_str = cJSON_PrintUnformatted(json);
    if (json_str == NULL) {
        fprintf(stderr, "Failed to format spaceship command to JSON.\n");
    }

end:
    cJSON_Delete(json);
    return json_str;
}

cJSON *_ackstream_to_json_obj(ACKStream *ackstream) {
    cJSON *ackstream_item = cJSON_CreateObject();
    cJSON *value_item = cJSON_CreateArray();

    for (int i = 0; i < ackstream->size; i++) {
        const char *ackit = ACKitStrings[ackstream->value[i]];
        cJSON_AddItemToArray(value_item, cJSON_CreateString(ackit));
    }

    cJSON_AddItemToObject(ackstream_item, "value", value_item);

    return ackstream_item;
}

cJSON *_transmissions_to_json_obj(Transmission *transmissions, int count) {
    cJSON *transmissions_item = cJSON_CreateArray();
    cJSON *transmission_item = NULL;
    cJSON *ackstream_item = NULL;

    ACKStream *ackstream_tmp = NULL;

    for (int i = 0; i < count; i++) {
        transmission_item = cJSON_CreateObject();
        if (transmission_item == NULL) return NULL;

        ackstream_tmp = transmissions[i].frequency;
        ackstream_item = _ackstream_to_json_obj(ackstream_tmp);
        cJSON_AddItemToObject(transmission_item, "frequency", ackstream_item);

        ackstream_tmp = transmissions[i].message;
        ackstream_item = _ackstream_to_json_obj(ackstream_tmp);
        cJSON_AddItemToObject(transmission_item, "message", ackstream_item);

        cJSON_AddItemToArray(transmissions_item, transmission_item);
    }

    return transmissions_item;
}

char *tower_command_to_json(TowerCommand *tower_command) {
    cJSON *json = cJSON_CreateObject();
    cJSON *transmissions_item = NULL;

    char *json_str = NULL;

    Transmission *transmissions = tower_command->transmissions;
    int transmissions_count = tower_command->transmissions_count;
    transmissions_item = _transmissions_to_json_obj(transmissions, transmissions_count);
    if (transmissions_item == NULL) goto end;
    cJSON_AddItemToObject(json, "transmissions", transmissions_item);

    json_str = cJSON_PrintUnformatted(json);
    if (json_str == NULL) {
        fprintf(stderr, "Failed to format tower command to JSON.\n");
    }

end:
    cJSON_Delete(json);
    return json_str;
}

ACKStream *_populate_ACKStream(const cJSON *ackstream_item) {
    const cJSON *value_item = NULL;
    const cJSON *ackit_item = NULL;

    int value_count = 0;
    
    value_item = cJSON_GetObjectItem(ackstream_item, "value");
    value_count = cJSON_GetArraySize(value_item);
    
    ACKStream *ackstream = malloc(sizeof(ACKStream));
    ackstream->value = malloc(sizeof(ACKit) * value_count);
    ackstream->size = value_count;

    int i = 0;
    cJSON_ArrayForEach(ackit_item, value_item) {
        ackstream->value[i++] = get_ACKit_from_string(ackit_item->valuestring);
    }

    return ackstream;
}

Transmission *_populate_Transmissions(const cJSON *transmissions_item, int transmission_count) {
    const cJSON *transmission_item = NULL;
    const cJSON *frequency_item = NULL;
    const cJSON *message_item = NULL;

    Transmission *transmission = malloc(sizeof(Transmission) * transmission_count);

    int i = 0;
    cJSON_ArrayForEach(transmission_item, transmissions_item) {
        frequency_item = cJSON_GetObjectItem(transmission_item, "frequency");
        message_item = cJSON_GetObjectItem(transmission_item, "message");
        transmission[i].frequency = _populate_ACKStream(frequency_item);
        transmission[i++].message = _populate_ACKStream(message_item);
    }

    return transmission;
}

SpaceshipSighting *_populate_SpaceshipSighting(const cJSON *spaceship_item) {
    if (cJSON_IsNull(spaceship_item)) {
        return NULL;
    }

    SpaceshipSighting *spaceship = malloc(sizeof(SpaceshipSighting));

    char *team_id_string = cJSON_GetObjectItem(spaceship_item, "team_id")->valuestring;
    spaceship->team_id = get_ACKit_from_string(team_id_string);
    spaceship->spaceship_id = cJSON_GetObjectItem(spaceship_item, "spaceship_id")->valueint;

    return spaceship;
}

Landmark *_populate_Landmark(const cJSON *landmark_item) {
    if (cJSON_IsNull(landmark_item)) {
        return NULL;
    }

    Landmark *landmark = malloc(sizeof(Landmark));

    ACKStream *id = NULL;
    const char *name = NULL;
    const cJSON *coords_item = NULL;
    const cJSON *x_item = NULL;
    const cJSON *y_item = NULL;

    id = _populate_ACKStream(cJSON_GetObjectItem(landmark_item, "id"));
    name = cJSON_GetObjectItem(landmark_item, "name")->valuestring;
    coords_item = cJSON_GetObjectItem(landmark_item, "coords");
    x_item = cJSON_GetObjectItem(coords_item, "x");
    y_item = cJSON_GetObjectItem(coords_item, "y");

    landmark->id = id;
    landmark->name = malloc(strlen(name) + 1);
    strcpy(landmark->name, name);
    landmark->coords.x = x_item->valueint;
    landmark->coords.y = y_item->valueint;

    return landmark;
}

Tile **_populate_Tiles(const cJSON *tiles_item, int *rows, int *cols) {
    const cJSON *tiles_row_item = NULL;
    const cJSON *tile_item = NULL;
    const cJSON *landmark_item = NULL;
    const cJSON *spaceship_item = NULL;

    *rows = cJSON_GetArraySize(tiles_item);

    Tile **tiles = malloc(sizeof(Tile) * (*rows));
    Tile *tile_row = NULL;
    char *type;

    int i = 0;
    cJSON_ArrayForEach(tiles_row_item, tiles_item) {
        *cols = cJSON_GetArraySize(tiles_row_item);
        tile_row = malloc(sizeof(Tile) * (*cols));

        int j = 0;
        cJSON_ArrayForEach(tile_item, tiles_row_item) {
            type = cJSON_GetObjectItem(tile_item, "type")->valuestring;
            tile_row[j].type = get_TileType_from_string(type);

            landmark_item = cJSON_GetObjectItem(tile_item, "landmark");
            spaceship_item = cJSON_GetObjectItem(tile_item, "spaceship");

            tile_row[j].landmark = _populate_Landmark(landmark_item);
            tile_row[j].spaceship = _populate_SpaceshipSighting(spaceship_item);
            tile_row[j].contains_pickup = cJSON_GetObjectItem(tile_item, "contains_pickup")->valueint;
            tile_row[j].contains_dropoff = cJSON_GetObjectItem(tile_item, "contains_dropoff")->valueint;
            j++;
        }

        tiles[i++] = tile_row;
    }

    return tiles;
}
