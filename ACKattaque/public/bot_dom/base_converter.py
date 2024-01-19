# convert ACKStream to base 10 int
from typing import List

from ACKattaque.public.game_message import ACKStream, ACKit

def get_ACKit_values() -> list:
    return [ACKit.TINKWINKI, ACKit.LALAR, ACKit.DISPI, ACKit.POHO, ACKit.NONONO]


def ACKStream_to_int(stream: ACKStream) -> int:
    stream = stream[::-1]
    stream_int = 0
    for i in range(len(stream)):
        stream_int += get_ACKit_values().index(stream[i]) * (5 ** i)
    return stream_int

def int_to_ACKStream(stream_int: int) -> List[ACKit]:
    stream = []
    while stream_int > 0:
        stream.append(get_ACKit_values()[stream_int % 5])
        stream_int //= 5
    return stream[::-1]

if __name__ == '__main__':
    print(ACKStream_to_int([ACKit.TINKWINKI, ACKit.LALAR, ACKit.DISPI, ACKit.POHO, ACKit.NONONO]))
