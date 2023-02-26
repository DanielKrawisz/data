#!/usr/bin/env python

import asyncio
from websockets import serve
import random
import string

def get_random_string(length):
    # choose from all lowercase letter
    letters = string.ascii_lowercase
    result_str = ''.join(random.choice(letters) for i in range(random.randint(1,10)))
    return result_str


async def periodic(websocket):
    while True:
        await websocket.send("{end:'"+get_random_string(5)+"'}")
        await asyncio.sleep(5)


async def echo(websocket):
    await websocket.send("{start:'"+get_random_string(5)+"'}")
    await periodic(websocket)


async def main():
    async with serve(echo, "localhost", 8765):
        await asyncio.Future()  # run forever


asyncio.run(main())
