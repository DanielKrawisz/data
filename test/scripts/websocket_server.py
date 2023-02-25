#!/usr/bin/env python

import asyncio
from websockets import serve


async def periodic(websocket):
    while True:
        await websocket.send("{start:0}")
        await asyncio.sleep(5)


async def echo(websocket):
    await websocket.send("{start:1}")
    await periodic(websocket)


async def main():
    async with serve(echo, "localhost", 8765):
        await asyncio.Future()  # run forever


asyncio.run(main())