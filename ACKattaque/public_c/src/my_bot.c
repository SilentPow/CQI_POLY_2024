#include "my_bot.h"
#include "game_message.h"
#include "spaceship_message.h"
#include "utils/json.h"
#include "utils/mem.h"


char *get_command(char *spaceship_message_json) {
    /* You shouldn't have to touch this */
    SpaceshipMessage *spaceship_message = spaceship_message_from_json(spaceship_message_json);
    SpaceshipCommand *spaceship_command = create_empty_spaceship_command();

    // TODO: implement your bot here
    // Example: move north and keep "TINKWINKI" in memory
    spaceship_command->action = NORTH;

    spaceship_command->transmissions = create_transmissions(1);
    spaceship_command->transmissions_count = 1;

    ACKStream *frequency = spaceship_command->transmissions[0].frequency;
    frequency->value = malloc(sizeof(ACKit));
    frequency->size = 1;
    frequency->value[0] = TINKWINKI;

    ACKStream *message = spaceship_command->transmissions[0].message;
    message->value = malloc(sizeof(ACKit));
    message->size = 1;
    message->value[0] = POHO;

    spaceship_command->memory->value = malloc(sizeof(ACKit));
    spaceship_command->memory->size = 1;
    spaceship_command->memory->value[0] = TINKWINKI;

    /* You shouldn't have to touch this */
    char *command_json = spaceship_command_to_json(spaceship_command);
    free_spaceship_message(spaceship_message);
    free_spaceship_command(spaceship_command);

    return command_json;
}

char *send_messages(char *game_message_json) {
    /* You shouldn't have to touch this */
    GameMessage *game_message = game_message_from_json(game_message_json);
    TowerCommand *tower_command = create_empty_tower_command();

    // TODO: implement your bot here
    // Example: send "POHO" on frequency "TINKWINKI"
    tower_command->transmissions = create_transmissions(1);
    tower_command->transmissions_count = 1;

    ACKStream *frequency = tower_command->transmissions[0].frequency;
    frequency->value = malloc(sizeof(ACKit));
    frequency->size = 1;
    frequency->value[0] = TINKWINKI;

    ACKStream *message = tower_command->transmissions[0].message;
    message->value = malloc(sizeof(ACKit));
    message->size = 1;
    message->value[0] = POHO;

    /* You shouldn't have to touch this */
    char *command_json = tower_command_to_json(tower_command);
    free_game_message(game_message);
    free_tower_command(tower_command);

    return command_json;
}
