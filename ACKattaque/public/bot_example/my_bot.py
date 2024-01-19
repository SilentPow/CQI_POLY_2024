from ACKattaque.public.game_message import Coords, GameMessage, ACKStream, Objective, Tile, TileType
from ACKattaque.public.spaceship import Spaceship
from ACKattaque.public.spaceship_command import SpaceshipCommand, Action
from ACKattaque.public.spaceship_message import SpaceshipMessage
from ACKattaque.public.tower import Tower
from ACKattaque.public.tower_command import TowerCommand
from typing import List

import random

class PlayerSpaceship(Spaceship):
    def get_command(self, spaceship_message: str) -> str:
        ######## Keep these lines #########        
        spaceship_message = SpaceshipMessage.from_json(spaceship_message)
        ######## Keep these lines #########

        if spaceship_message.type == "HUMMINGBIRD": 
            return self.get_command_hummingbird()
        elif spaceship_message.type == "DUCK":
            return self.get_command_hummingbird()
        elif spaceship_message.type == "FLAMINGO": 
            return self.get_command_hummingbird()
        elif spaceship_message.type == "ALBATROSS": 
            return self.get_command_hummingbird()
        

        ######## Keep these lines #########
        ######## Keep these lines #########
        
    def get_command_hummingbird(self, spaceship_message: SpaceshipMessage):
        memory = spaceship_message.memory
        action = None
        
        if not spaceship_message.carries_payload:
            objective = self.get_closest_objective_from(spaceship_message.gps, spaceship_message.objectives)
            stepDirection = self.step_towards(spaceship_message.gps, objective)
            nextTile = self.get_tile_in_direction(stepDirection, spaceship_message.local_tiles)
            while nextTile.type == TileType.WALL:
                action_values = list(Action)
                current_action_index = action_values.index(stepDirection)
                next_index = (current_action_index + 1) % len(action_values)
                stepDirection = action_values[next_index]
                nextTile = self.get_tile_in_direction(stepDirection, spaceship_message.local_tiles)
        else:
                    
        
            
        
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
    
    def step_towards(self, startPosition: Coords, targetPosition: Coords):
        dx = abs(targetPosition.x - startPosition.x)
        dy = abs(targetPosition.y - startPosition.y)
        
        if abs(dx) > abs(dy):
            if dx > 0:
                return Action.EAST
            else:
                return Action.WEST
        else:
            if dy > 0:
                return Action.SOUTH
            else:
                return Action.NORTH
            
    def get_tile_in_direction(self, direction: Action, local_tiles: List[List[Tile]]):
        if direction == Action.NORTH:
            return local_tiles[0][1] 
        if direction == Action.SOUTH:
            return local_tiles[2][1] 
        if direction == Action.EAST:
            return local_tiles[1][2] 
        if direction == Action.WEST:
            return local_tiles[1][0] 
        
            
    def get_closest_objective_from(self, position: Coords, objectives: List[Objective]):
        objectiveDistances = []
        for objective in objectives:
            objectiveDistances.append(self.get_travelling_distance(position, objective.start))
            
        return objectiveDistances.index(min(objectiveDistances))
            
            
    def get_travelling_distance(position1: Coords, position2: Coords):
        dx = abs(position1.x - position2.x)
        dy = abs(position1.y - position2.y)
        
        return abs(dx - dy) + min(dx, dy) * 2
    
        
    def get_command_duck(self, spaceship_message: SpaceshipMessage):
        action = random.choice((Action.NORTH, Action.EAST, Action.SOUTH, Action.WEST))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
    
        
    def get_command_flamingo(self, spaceship_message: SpaceshipMessage):
        action = random.choice((Action.NORTH, Action.EAST, Action.SOUTH, Action.WEST))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
    
        
    def get_command_albatross(self, spaceship_message: SpaceshipMessage):
        action = random.choice((Action.NORTH, Action.EAST, Action.SOUTH, Action.WEST))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
        
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