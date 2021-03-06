#include <iostream>
#include <masesk/EasySocket.hpp>
#include <string>
using namespace std;
using namespace masesk;

int main() {
	EasySocket socketManager;
	socketManager.socketConnect("test", "127.0.0.1", 8080);
	string userInput;
	while (true) {
		cout << "> ";
		getline(cin, userInput);
		if (userInput.size() <= 0) {
			break;
		}
		socketManager.socketSend("test", userInput);
	}
	socketManager.closeConnection("test");
	return 0;
}