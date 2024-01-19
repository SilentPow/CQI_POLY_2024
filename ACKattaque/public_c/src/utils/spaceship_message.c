#include "spaceship_message.h"

SpaceshipType get_SpaceshipType_from_string(const char *spaceship_type_string)
{
    for (long unsigned int i = 0; i < sizeof(SpaceshipTypeMap) / sizeof(SpaceshipTypeMap[0]); i++)
    {
        if (strcmp(spaceship_type_string, SpaceshipTypeMap[i].string) == 0)
        {
            return SpaceshipTypeMap[i].type;
        }
    }
    return -1;
}
