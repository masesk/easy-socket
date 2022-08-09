#include <iostream>
#include <masesk/EasySocket.hpp>
using namespace std;
using namespace masesk;

void handleData(const std::string &data) {
	cout << "Client sent: " + data << endl;
}

int main() {
	EasySocket socketManager;
	socketManager.socketListenTCP<void, std::string>("test", 8080, &handleData);
	return 0;
}