#ifndef JSON_H
#define JSON_H

#include "spaceship_message.h"
#include "spaceship_command.h"
#include "tower_command.h"
#include "game_message.h"

SpaceshipMessage *spaceship_message_from_json(char *spaceship_message_str);
GameMessage *game_message_from_json(char *game_message_str);

char *spaceship_command_to_json(SpaceshipCommand *spaceship_command);
char *tower_command_to_json(TowerCommand *tower_command);

#endif // JSON_H
