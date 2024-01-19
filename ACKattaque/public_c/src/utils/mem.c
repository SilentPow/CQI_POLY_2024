#include "mem.h"

SpaceshipCommand* create_empty_spaceship_command()
{
    SpaceshipCommand *spaceship_command = malloc(sizeof(SpaceshipCommand));
    spaceship_command->transmissions = NULL;
    spaceship_command->transmissions_count = 0;
    spaceship_command->memory = create_empty_ackstream();
    return spaceship_command;
}

TowerCommand* create_empty_tower_command()
{
    TowerCommand *tower_command = malloc(sizeof(TowerCommand));
    tower_command->transmissions = NULL;
    tower_command->transmissions_count = 0;
    return tower_command;
}

ACKStream* create_empty_ackstream()
{
    ACKStream *ackstream = malloc(sizeof(ACKStream));
    ackstream->value = NULL;
    ackstream->size = 0;
    return ackstream;
}

Transmission* create_transmissions(int count)
{
    Transmission *transmissions = malloc(count * sizeof(Transmission));
    for (int i = 0; i < count; i++)
    {
        transmissions[i].frequency = create_empty_ackstream();
        transmissions[i].message = create_empty_ackstream();
    }
    return transmissions;
}

void free_spaceship_message(SpaceshipMessage *spaceship_message)
{
    int tiles_dim = spaceship_message->local_tiles_dim;

    free_transmissions(spaceship_message->transmissions, spaceship_message->transmission_count);
    free_tiles(spaceship_message->local_tiles, tiles_dim, tiles_dim);
    free_ackstream(spaceship_message->memory);
    free_coords(spaceship_message->gps);
    free_objectives(spaceship_message->objectives);
    free(spaceship_message);
}

void free_spaceship_command(SpaceshipCommand *spaceship_command)
{
    free_transmissions(spaceship_command->transmissions, spaceship_command->transmissions_count);
    free_ackstream(spaceship_command->memory);
    free(spaceship_command);
}

void free_game_message(GameMessage *game_message)
{
    free_map(game_message->map);
    free_landmarks(game_message->landmarks, game_message->landmarks_count);
    free_transmissions(game_message->transmissions, game_message->transmissions_count);
    free_teams(game_message->teams, game_message->teams_count);
    free_objectives(game_message->objectives);
    free(game_message);
}

void free_tower_command(TowerCommand *tower_command)
{
    free_transmissions(tower_command->transmissions, tower_command->transmissions_count);
    free(tower_command);
}

void free_transmissions(Transmission *transmissions, int count)
{
    for (int i = 0; i < count; i++)
    {
        free_ackstream(transmissions[i].frequency);
        free_ackstream(transmissions[i].message);
    }
    free(transmissions);
}

void free_teams(Team *teams, int count __attribute__((unused)))
{
    free(teams);
}

void free_map(Map *map)
{
    free_tiles(map->tiles, map->rows, map->cols);
    free(map);
}

void free_tiles(Tile **tiles, int dim1, int dim2)
{
    for (int i = 0; i < dim1; i++)
    {
        for (int j = 0; j < dim2; j++)
        {
            free_landmark(tiles[i][j].landmark);
            free_spaceship(tiles[i][j].spaceship);
        }
        free(tiles[i]);
    }
    free(tiles);
}

void free_objectives(Objective *objectives)
{
    if (objectives != NULL)
    {
        free(objectives);
    }
}

void free_coords(Coords *coords)
{
    if (coords != NULL)
    {
        free(coords);
    }
}

void free_landmark(Landmark *landmark)
{
    if (landmark != NULL)
    {
        free_ackstream(landmark->id);
        free(landmark->name);
        free(landmark);
    }
}

void free_landmarks(Landmark **landmarks, int count)
{
    for (int i = 0; i < count; i++)
    {
        free_landmark(landmarks[i]);
    }
    free(landmarks);
}

void free_spaceship(SpaceshipSighting *spaceship)
{
    if (spaceship != NULL)
    {
        free(spaceship);
    }
}

void free_ackstream(ACKStream *ackstream)
{
    free(ackstream->value);
    free(ackstream);
}
