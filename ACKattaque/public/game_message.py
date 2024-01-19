from dataclasses import dataclass, field

from dataclasses_json import dataclass_json
from enum import Enum
from typing import List, Union, Optional

import random

@dataclass_json
class ACKit(str, Enum):
    TINKWINKI = "TINKWINKI"
    LALAR = "LALAR"
    DISPI = "DISPI"
    POHO = "POHO"
    NONONO = "NONONO"


def get_ACKit_values():
    return [ACKit.TINKWINKI, ACKit.LALAR, ACKit.DISPI, ACKit.POHO, ACKit.NONONO]


@dataclass_json
@dataclass
class ACKStream:
    value: List[ACKit]


@dataclass_json
@dataclass(frozen=True)
class Coords:
    x: int
    y: int


@dataclass_json
class TileType(str, Enum):
    EMPTY = "EMPTY"
    WALL = "WALL"


@dataclass_json
@dataclass
class Landmark:
    id: ACKStream
    name: str
    coords: Coords


@dataclass_json
@dataclass
class SpaceshipSighting:
    team_id: ACKit
    spaceship_id: int

@dataclass_json
@dataclass
class Tile:
    type: TileType
    landmark: Optional[Landmark] = None
    spaceship: Optional[SpaceshipSighting] = None # Always None in game messages, will be set in spaceship messages
    
    contains_pickup: bool = False # Always False in game messages, will be set in get_local_tiles
    contains_dropoff: bool = False # Same thing as pickup. Can only be seen for the objective currently held by the spaceship

    def contains_landmark(self) -> bool:
        return isinstance(self.landmark, Landmark)

    def contains_spaceship(self) -> bool:
        return isinstance(self.spaceship, SpaceshipSighting)

@dataclass_json
@dataclass
class Map:
    tiles: List[List[Tile]]
    rows: int
    cols: int

    def get_tile(self, coords: Coords) -> Tile:
        return self.tiles[coords.x][coords.y]

    def get_tile_type(self, coords: Coords) -> TileType:
        return self.tiles[coords.x][coords.y].type

@dataclass_json
@dataclass
class Transmission:
    frequency: ACKStream  # 2 ACKits
    message: ACKStream


@dataclass_json
@dataclass
class Objective:
    score: int
    start: Coords
    end: Coords


@dataclass_json
@dataclass
class Team:
    id: ACKit
    tower_position: Coords
    score: int

@dataclass_json
@dataclass
class GameMessage:
    map: Map
    landmarks: List[Landmark]
    game_step: int
    total_steps: int
    transmissions: List[Transmission]
    n_spaceships: int
    teams: List[Team]
    my_team_id: ACKit
    objectives: List[Objective]


if __name__ == "__main__":
    schema = GameMessage.schema()
