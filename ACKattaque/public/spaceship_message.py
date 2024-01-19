from dataclasses import dataclass
from enum import Enum
from typing import List, Union, Optional

from dataclasses_json import dataclass_json

from ACKattaque.public.game_message import Transmission, Tile, ACKStream, Coords, Objective

@dataclass_json
class SpaceshipType(str, Enum):
    HUMMINGBIRD = "HUMMINGBIRD"
    DUCK = 'DUCK'
    ALBATROSS = 'ALBATROSS'
    FLAMINGO = 'FLAMINGO'

    @staticmethod
    def from_id(id):
        match id:
            case 0:
                return SpaceshipType.HUMMINGBIRD
            case 1:
                return SpaceshipType.DUCK
            case 2:
                return SpaceshipType.ALBATROSS
            case 3:
                return SpaceshipType.FLAMINGO
            case _:
                raise RuntimeError(f'Invalid spaceship type from id: {id}')

@dataclass_json
@dataclass
class SpaceshipMessage:
    type: SpaceshipType
    transmissions: List[Transmission]
    local_tiles: List[List[Tile]]
    can_see_landmark: bool
    can_see_spaceship: bool
    carries_payload: bool
    memory: ACKStream

    gps: Optional[Coords] = None
    objectives: Optional[List[Objective]] = None
