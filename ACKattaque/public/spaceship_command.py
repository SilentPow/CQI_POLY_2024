from dataclasses import dataclass
from enum import Enum
from typing import List, Optional

from dataclasses_json import dataclass_json

from ACKattaque.public.game_message import Transmission, ACKStream


@dataclass_json
class Action(str, Enum):
    NORTH = "NORTH"
    SOUTH = "SOUTH"
    WEST = "WEST"
    EAST = "EAST"
    DROP = "DROP"
    PICKUP = "PICKUP"


@dataclass_json
@dataclass
class SpaceshipCommand:
    transmissions: List[Transmission]
    memory: ACKStream
    action: Optional[Action] = None
