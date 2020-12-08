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