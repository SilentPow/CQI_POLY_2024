#ifndef MEM_H
#define MEM_H

#include "game_message.h"
#include "spaceship_message.h"
#include "spaceship_command.h"
#include "tower_command.h"

SpaceshipCommand* create_empty_spaceship_command();
TowerCommand* create_empty_tower_command();
ACKStream* create_empty_ackstream();
Transmission* create_transmissions(int count);

void free_spaceship_message(SpaceshipMessage *spaceship_message);
void free_spaceship_command(SpaceshipCommand *spaceship_command);
void free_game_message(GameMessage *game_message);
void free_tower_command(TowerCommand *tower_command);
void free_transmissions(Transmission *transmissions, int count);
void free_teams(Team *teams, int count);
void free_map(Map *map);
void free_tiles(Tile **tiles, int dim1, int dim2);
void free_ackstream(ACKStream *ackstream);
void free_objectives(Objective *objectives);
void free_coords(Coords *coords);
void free_landmark(Landmark *landmark);
void free_landmarks(Landmark **landmarks, int count);
void free_spaceship(SpaceshipSighting *spaceship);

#endif /* MEM_H */
