from ACKattaque.public.game_message import Coords, GameMessage, ACKStream
from ACKattaque.public.spaceship import Spaceship
from ACKattaque.public.spaceship_command import SpaceshipCommand, Action
from ACKattaque.public.spaceship_message import SpaceshipMessage, SpaceshipType
from ACKattaque.public.tower import Tower
from ACKattaque.public.tower_command import TowerCommand
from logger import web_logger

import random

def get_next_direction(current_pos: Coords, target_pos: Coords, carries_payload: bool) -> Action:
    if current_pos.x < target_pos.x:
        return Action.EAST
    elif current_pos.x > target_pos.x:
        return Action.WEST
    elif current_pos.y < target_pos.y:
        return Action.NORTH
    elif current_pos.y > target_pos.y:
        return Action.SOUTH
    else:
        return Action.PICKUP if not carries_payload else Action.DROP

class PlayerSpaceship(Spaceship):
    def get_command(self, spaceship_message: str) -> str:
        ######## Keep these lines #########
        spaceship_message = SpaceshipMessage.from_json(spaceship_message)
        ######## Keep these lines #########

        if spaceship_message.type == SpaceshipType.HUMMINGBIRD:
            objective = spaceship_message.objectives[-2]
            action = get_next_direction(spaceship_message.gps, objective.start, spaceship_message.carries_payload)
        else:
            action = random.choice((Action.NORTH, Action.EAST, Action.SOUTH, Action.WEST))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        
        print(f'Sending command {command}')

        web_logger({'command': f'{command}'})

        ######## Keep these lines #########
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
        ######## Keep these lines #########
        
class PlayerTower(Tower):
    def send_messages(self, game_message: str) -> str:
        ######## Keep these lines #########
        game_message = GameMessage.from_json(game_message)
        ######## Keep these lines #########

        command = TowerCommand([])

        ########## Keep these lines ##########
        assert(TowerCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
        ########## Keep these lines ##########

########## Keep these lines ##########
if __name__ == '__main__':
    import argparse
    from ACKattaque.public.socket_communication import start_python_socket_server

    parser = argparse.ArgumentParser('Starts the main socket server')
    parser.add_argument('port', type=int, help='Specifies on which port to start the server')
    args = parser.parse_args()

    start_python_socket_server(args.port, PlayerTower, PlayerSpaceship)
########## Keep these lines ##########