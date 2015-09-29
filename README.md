# APIEngine
APIEngine is a C++ program and framework for creating JSON API's in the easiest way possible.  It was created by the baldr.io team when we got sick of the other solutions out there and decided to just create out own.  We didn't need a large scalable API, just something simple.

## Features
* Built in HTTP server to serve the content.
* LUA scripting to create the endpoints
* Easy to use flat file organization system

## Dependencies
* Lua 5.3
* Jsoncpp
* A lua JSON library of your choice
* microhttpd

## How to use
All you need to get up an running is the binary, and an api/ folder with at least 1 LUA script in it.  The engine works by translating calls to serverip:serverport/api/something to ./api/something.lua, calls the function runapi() inside of that script (which is our entry point), and grabs the return value.  The return value must be a string, and the function must take no arguments.  If you want to layer your APIs, simply create folders and place the scripts inside of them.  If you want your api endpoint to be at /api/serverdata/diskspace, create a serverdata folder in ./api, and inside of that folder create your diskspace.lua script. Yes, it really is that easy.
