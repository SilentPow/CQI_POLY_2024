from ACKattaque.public.game_message import GameMessage, ACKStream
from ACKattaque.public.spaceship import Spaceship
from ACKattaque.public.spaceship_command import SpaceshipCommand, Action
from ACKattaque.public.spaceship_message import SpaceshipMessage
from ACKattaque.public.tower import Tower
from ACKattaque.public.tower_command import TowerCommand

import random

class PlayerSpaceship(Spaceship):
    def get_command(self, spaceship_message: str) -> str:
        ######## Keep these lines #########        
        spaceship_message = SpaceshipMessage.from_json(spaceship_message)
        ######## Keep these lines #########

        action = random.choice((Action.NORTH, Action.EAST, Action.SOUTH, Action.WEST))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')

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