from ACKattaque.public.game_message import ACKit, Coords, GameMessage, ACKStream, Objective, Tile, TileType
from ACKattaque.public.spaceship import Spaceship
from ACKattaque.public.spaceship_command import SpaceshipCommand, Action
from ACKattaque.public.spaceship_message import SpaceshipMessage, SpaceshipType
from ACKattaque.public.tower import Tower
from ACKattaque.public.tower_command import TowerCommand
from typing import List

import random

class PlayerSpaceship(Spaceship):
    def get_command(self, spaceship_message: str) -> str:
        ######## Keep these lines #########        
        spaceship_message = SpaceshipMessage.from_json(spaceship_message)
        ######## Keep these lines #########
        
        print(f"Type: {spaceship_message.type}")

        if spaceship_message.type == SpaceshipType.HUMMINGBIRD: 
            return self.get_command_hummingbird(spaceship_message)
        elif spaceship_message.type == SpaceshipType.DUCK:
            return self.get_command_duck(spaceship_message)
        elif spaceship_message.type == SpaceshipType.FLAMINGO: 
            return self.get_command_flamingo(spaceship_message)
        elif spaceship_message.type == SpaceshipType.ALBATROSS: 
            return self.get_command_albatross(spaceship_message)
        else:
            return self.get_command_albatross(spaceship_message)
            
        

        ######## Keep these lines #########
        ######## Keep these lines #########
        
    def get_command_hummingbird(self, spaceship_message: SpaceshipMessage):
        memory = spaceship_message.memory
        action = None
        
        # if spaceship_message.local_tiles[1][1].contains_pickup:
        #     action = Action.PICKUP
        #     memory[:3] = self.get_stream_from_coord(spaceship_message.gps.x)
        #     memory[3:6] = self.get_stream_from_coord(spaceship_message.gps.y)
        #     print(f"Memory: {memory}")
        # elif spaceship_message.local_tiles[1][1].contains_dropoff:
        #     action = Action.DROP
        # elif not spaceship_message.carries_payload:
        objective = self.get_closest_objective_from(spaceship_message.gps, spaceship_message.objectives)
        print(f"Objective: {objective.start}")
        stepDirection = self.step_towards(spaceship_message.gps, objective.start)
        print(f"Step: {stepDirection}")
        action = self.take_step_in_direction(stepDirection, spaceship_message.local_tiles)
        print(f"Action: {action}")
        # else:
        #     currentObjectiveStartX = self.get_coord_from_stream(memory[:3])
        #     currentObjectiveStartY = self.get_coord_from_stream(memory[3:6])
        #     currentObjective: Objective = self.get_objective_from_start_position(Coords(currentObjectiveStartX, currentObjectiveStartY))
            
        #     stepDirection = self.step_towards(spaceship_message.gps, currentObjective.end)
        #     action = self.take_step_in_direction(stepDirection, spaceship_message.local_tiles)
            
        
        command = SpaceshipCommand(transmissions=[], memory=ACKStream(memory), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
    
    def take_step_in_direction(self, stepDirection: Action, local_tiles: List[List[Tile]]):
        nextTile = self.get_tile_in_direction(stepDirection, local_tiles)
        i = 0
        while nextTile.type == TileType.WALL or nextTile.contains_spaceship or 1 == 4:
            stepDirection = self.get_next_direction(stepDirection)
            nextTile = self.get_tile_in_direction(stepDirection, local_tiles)
            i += 1
        return stepDirection
        
    def get_next_direction(self, direction: Action):
        if direction == Action.NORTH:
            return Action.EAST
        if direction == Action.EAST:
            return Action.SOUTH
        if direction == Action.SOUTH:
            return Action.WEST
        if direction == Action.WEST:
            return Action.NORTH
    
    def get_objective_from_start_position(self, startPosition: Coords, objectives: List[Objective]):
        for objective in objectives:
            if objective.start.x == startPosition.x and objective.start.y == startPosition.y:
                return objective
        return None
    
    def get_stream_from_coord(self, position: Coords):
        bit1 = position % 25
        bit2 = (position - bit1) % 5
        bit3 = (bit1 - bit2)
        ackitValues = ACKit.values
        
        return [ackitValues[bit1], ackitValues[bit2], ackitValues[bit3]]
    
    def get_coord_from_stream(self, stream: List[ACKit]):
        ackitValues = ACKit.values
        bit1 = ackitValues.index(stream[0])
        bit2 = ackitValues.index(stream[1])
        bit3 = ackitValues.index(stream[2])
        
        return bit1 * 25 + bit2 * 5 + bit3
    
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
            
        return objectives[objectiveDistances.index(min(objectiveDistances))]
            
            
    def get_travelling_distance(self, position1: Coords, position2: Coords):
        dx = abs(position1.x - position2.x)
        dy = abs(position1.y - position2.y)
        
        return abs(dx - dy) + min(dx, dy) * 2
    
        
    def get_command_duck(self, spaceship_message: SpaceshipMessage):
        action = random.choice((Action.NORTH))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
    
        
    def get_command_flamingo(self, spaceship_message: SpaceshipMessage):
        action = random.choice((Action.NORTH))
        command = SpaceshipCommand(transmissions=[], memory=ACKStream([]), action=action)
        print(f'Sending command {command}')
        
        assert(SpaceshipCommand.schema().validate(command.to_dict()) == {})
        return command.to_json()
    
        
    def get_command_albatross(self, spaceship_message: SpaceshipMessage):
        action = random.choice((Action.NORTH))
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