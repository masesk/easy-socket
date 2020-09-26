# easy-socket
Modern C++ 11 native, OS agnostic, socket library/base code with a single header include

[![license](https://img.shields.io/github/license/masesk/easy-socket.svg)](https://github.com/masesk/easy-socket/blob/master/LICENSE)
[![stars](https://img.shields.io/github/stars/masesk/easy-socket.svg?style=social)](https://github.com/masesk/easy-socket/stargazers)
[![forks](https://img.shields.io/github/forks/masesk/easy-socket?style=social)](https://github.com/masesk/easy-socket/network/members)

## Features
* Easy: Start using simple socket features by including a single header file
* Portable: Works on Windows and Linux
* Configurable: All sockets configurations and settings can easily accessed and changed from the header file.

## Requirements
* C++ 11 and up

## Namespace, Members, Functions

### Import:
```
#include <masesk/EasySocket.hpp>
```

### Namespace and Initialize

```
masesk::EasySocket easySocket; //or using namespace std;
```

### Functions

#### Server Functions

* `void socketListen(std::string channelName, int port, std::function<void (std::string data)> callback);` 
    * channelName: string identifier of channel
    * port: integer value of port use on server side (eg. 8080)
    * function: pointer of function that will be called to handle data when socket recieves data

#### Client Functions

* `void socketConnect(std::string channelName, std::string ip, int port)` - start a new connection with a server with a channel name 
    * channelName - string identifier of channel
    * ip - string for where the server resides (eg. 127.0.0.1 for local)
    * port - integer value of port use on server side (eg. 8080)
* `void socketSend(std::string channelName,  std::string data)` - send data to server based on channel name
    * channelName: string identifier of channel
    * data: data to be sent through to the server on given channel
* `void closeConnection(std::string channelName)`  - close connection with server using channel name
    * channelName: string identifier of channel


## Example
Check `test/test-server` and `test/test-client` for a working client and server example running locally.