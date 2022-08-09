#include <iostream>
#include <masesk/EasySocket.hpp>
using namespace std;
using namespace masesk;

void handleData(const std::string &data) {
	cout << "Client sent: " + data << endl;
}

int main() {
	EasySocket socketManager;
	socketManager.socketListenUDP<void, std::string>("test_udp", 9090, handleData);
	return 0;
}