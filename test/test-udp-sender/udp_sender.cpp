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