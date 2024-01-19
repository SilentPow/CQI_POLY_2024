from typing import List

from ACKattaque.public.game_message import GameMessage, Transmission


class Tower:
    def send_messages(self, game_message: str) -> List[Transmission]:
        raise NotImplementedError()
