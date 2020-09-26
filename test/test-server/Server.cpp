#include <iostream>
#include <masesk/EasySocket.hpp>
using namespace std;
using namespace masesk;

void handleData(std::string data) {
	cout << "Client sent: " + data << endl;
}

int main() {
	EasySocket socketManager;
	socketManager.socketListen("test", 8080, &handleData);
	return 0;
}