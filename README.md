# easy-socket
Modern C++ 11 native, OS agnostic, socket library/starter code with a single header include

[![license](https://img.shields.io/github/license/masesk/easy-socket.svg)](https://github.com/masesk/easy-socket/blob/master/LICENSE)
[![stars](https://img.shields.io/github/stars/masesk/easy-socket.svg?style=social)](https://github.com/masesk/easy-socket/stargazers)
[![forks](https://img.shields.io/github/forks/masesk/easy-socket?style=social)](https://github.com/masesk/easy-socket/network/members)

## Features
* Easy: Start using simple socket features by including a single header file
* Portable: Works on Windows and Linux
* Configurable: All sockets configurations and settings can easily be accessed and changed from the header file.

## Requirements
* C++ 11 and up

## Namespace, Usecase, Members, Functions

### Import:
```
#include <masesk/EasySocket.hpp>
```

### Namespace and Initialize

```
masesk::EasySocket easySocket; //or using namespace masesk;
```

### Usecase
* EasySocket wraps the code for the server and client, so that a single header can be used to initialize and start a server or a client.
* EasySocket supports both UDP and TCP. [Refer to this StackOverflow answer for difference between the two and when to use each one.](https://stackoverflow.com/questions/1099672/when-is-it-appropriate-to-use-udp-instead-of-tcp#answer-1099734)
#### TCP
```cpp
//server example 

#include <iostream>
#include <masesk/EasySocket.hpp>
using namespace std;
using namespace masesk;

void handleData(const std::string &data) {
	cout << "Client sent: " + data << endl;
}

int main() {
	EasySocket socketManager;
	socketManager.socketListenTCP("test", 8080, &handleData);
	return 0;
}
```

```cpp
// client example

#include <iostream>
#include <masesk/EasySocket.hpp>
#include <string>
using namespace std;
using namespace masesk;

int main() {
	EasySocket socketManager;
	socketManager.socketConnectTCP("test", "127.0.0.1", 8080);
	string userInput;
	while (true) {
		cout << "> ";
		getline(cin, userInput);
		if (userInput.size() <= 0) {
			break;
		}
		socketManager.socketSendTCP("test", userInput);
	}
	socketManager.closeConnection("test");
	return 0;
}
```

#### UDP

```cpp
// receiver example
#include <iostream>
#include <masesk/EasySocket.hpp>
using namespace std;
using namespace masesk;

void handleData(const std::string &data) {
	cout << "Client sent: " + data << endl;
}

int main() {
	EasySocket socketManager;
	socketManager.socketListenUDP("test", 9090, handleData);
	return 0;
}
```

```cpp
// send example
#include <iostream>
#include <masesk/EasySocket.hpp>
#include <string>
using namespace std;
using namespace masesk;

int main() {
	EasySocket socketManager;
	string userInput;
	while (true) {
		cout << "> ";
		getline(cin, userInput);
		if (userInput.size() <= 0) {
			break;
		}
		socketManager.socketSendUDP("127.0.0.1", 9090, userInput);
	}
	socketManager.closeConnection("test");
	return 0;
}
```

### Functions

### TCP

#### Server Functions

* `void socketListenTCP(const std::string &channelName, const std::uint16_t &port, std::function<void (const std::string &data)> callback);` 
    * channelName: string identifier of channel
    * port: integer value of port used on server side (eg. 8080)
    * function: pointer of function that will be called to handle the data when the server recieves data from the client
* `void closeConnection(const std::string &channelName)`  - close socket port on server
    * channelName: string identifier of channel


#### Client Functions

* `void socketConnectTCP(const std::string &channelName, const std::string &ip, const std::uint16_t &port)` - start a new connection with a server with a channel name, ip address of the server, and the port the server would be listening on.
    * channelName - string identifier of channel
    * ip - string for where the server resides (eg. 127.0.0.1 for local)
    * port - integer value of port used on server side (eg. 8080)
* `void socketSendTCP(const std::string &channelName,  const std::string &data)` - send data to server based on channel name
    * channelName: string identifier of channel
    * data: data to be sent through to the server on given channel
* `void closeConnection(const std::string &channelName)`  - close connection with server using channel name
    * channelName: string identifier of channel


### UDP

#### Receiver Functions

* `void socketListenUDP(const std::string &channelName, const std::uint16_t &port, std::function<void (const std::string & data)> callback);` 
    * channelName: string identifier of channel
    * port: integer value of port used on server side (eg. 9090)
    * function: pointer of function that will be called to handle the data when the server recieves data from the client
* `void closeConnection(const std::string &channelName)`  - close UDP socket port
    * channelName: string identifier of channel

#### Sender Functions
* `void socketSendUDP(const std::string &ip, const std::uint16_t &port, const std::string &data)` - send data to server based on channel name
    * ip - string for where the server resides (eg. 127.0.0.1 for local)
    * port - integer value of port used on server side (eg. 9090)
    * data: data to be sent through to the server on given channel

## Example
Check `test/test-tcp-server` / `test/test-tcp-client` / `test/test-udp-receiver` / `test/test-udp-sender` for a working client and server example running locally.

### Build Tests on Windows
1. Open `easy-socket.sln` in Visual Studio 2017
2. Right click on `test-tcp-client`, select `properites`, and change `Windows SDK Version` to your installed 10.x
3. Right click on `test-tcp-server`, select `properites`, and change `Windows SDK Version` to your installed 10.x
4. Right click on `test-udp-receiver`, select `properites`, and change `Windows SDK Version` to your installed 10.x
5. Right click on `test-udp-sender`, select `properites`, and change `Windows SDK Version` to your installed 10.x
6. Right click on ```Solution 'easy-socket' ``` and select `Rebuild entire solution`.
7. Select desired `Configuration` (Debug/Release) and `Platform` (x64/Win32) from the top-bar dropdowns next to the start button.
8. Right click on `Solution easy-socket` and select two projects to start: either `test-tcp-server` and `test-tcp-client` OR `test-udp-receiver` and `test-udp-sender`
9. Executables will be available at `[x64 or Win32]/[Debug or Release]`

### Build Tests on Linux

Requirements: 
* CMake (> v3.5)
* make
* g++ (> v6.1)

```
mkdir build
```
```
cd build
```
```
cmake ..
```
```
make
```

Executables will be available at `build/tests/test-tcp-client/tcp_client` and `build/tests/test-tcp-client/tcp_server` and `build/tests/test-udp-receiver/udp_receiver` and `build/tests/test-udp-sender/udp_sender`

Disable building tests: `cmake -D BUILD_TESTS=OFF ..`
