from dataclasses import dataclass
from typing import List

from dataclasses_json import dataclass_json
from ACKattaque.public.game_message import Transmission

@dataclass_json
@dataclass
class TowerCommand:
    transmissions: List[Transmission]