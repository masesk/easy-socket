#include <iostream>
#include <masesk/EasySocket.hpp>
#include <string>
using namespace std;
using namespace masesk;

int main() {
	EasySocket socketManager;
	string userInput;
	socketManager.socketInitSendUDP("test_udp", "127.0.0.1", 9090);
	while (true) {
		cout << "> ";
		getline(cin, userInput);
		if (userInput.size() <= 0) {
			break;
		}
		try {
			socketManager.socketSendUDP("test_udp", userInput);
		}catch (socket_error_exception &e) {
			cout << e.what() << endl;
		}
		
	}
	socketManager.closeConnection("test");
	return 0;
}