#ifndef EASYSOCKET
#define EASYSOCKET

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
typedef int SOCKET;
const int INVALID_SOCKET = 0;
const int SOCKET_ERROR = -1;
#endif
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <atomic>
#include <functional>
#include <exception>


namespace masesk {
	const int BUFF_SIZE = 4096;
	struct socket_error_exception : public std::exception
	{
		const char* what() const throw ()
		{
			return "Can't start socket!";
		}
	};
	struct invalid_socket_exception : public std::exception
	{
		const char* what() const throw ()
		{
			return "Can't create a socket!";
		}
	};
	struct data_size_exception : public std::exception
	{
		const char* what() const throw ()
		{
			return "Data size is above the maximum allowed by the buffer";
		}
	};
	class EasySocket {
	public:
		void socketListen(const std::string& channelName, int port, std::function<void(const std::string& data)> callback) {

			if (sockInit() != 0) {
				throw masesk::socket_error_exception();
			}
			SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
			if (listening == INVALID_SOCKET || listening == SOCKET_ERROR)
			{
				throw masesk::socket_error_exception();
			}
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(port);
#ifdef _WIN32
			hint.sin_addr.S_un.S_addr = INADDR_ANY;
#else
			hint.sin_addr.s_addr = INADDR_ANY;
#endif
			bind(listening, (sockaddr*)&hint, sizeof(hint));
			listen(listening, SOMAXCONN);
			sockaddr_in client;
#ifdef _WIN32
			int clientSize = sizeof(client);
#else
			unsigned int clientSize = sizeof(client);
#endif
			SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
			server_sockets[channelName] = clientSocket;
			char host[NI_MAXHOST];
			char service[NI_MAXSERV];

			memset(host, 0, NI_MAXHOST);
			memset(service, 0, NI_MAXSERV);
			if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			{
				std::cout << host << " connected on port " << service << std::endl;
			}
			else
			{
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
				std::cout << host << " connected on port " <<
					ntohs(client.sin_port) << std::endl;
			}
			sockClose(listening);
			char buff[BUFF_SIZE];
			while (true)
			{
				memset(buff, 0, BUFF_SIZE);

				int bytesReceived = recv(clientSocket, buff, BUFF_SIZE, 0);
				if (bytesReceived == SOCKET_ERROR)
				{
					throw socket_error_exception();
				}
				if (bytesReceived > BUFF_SIZE) {
					throw masesk::data_size_exception();
				}
				if (bytesReceived > 0) {
					callback(std::string(buff, 0, bytesReceived));
				}
				else {
					break;
				}


			}
			sockClose(clientSocket);
			sockQuit();
		}

		void socketSend(const std::string& channelName, const std::string& data) {
			if (data.size() > BUFF_SIZE) {
				throw masesk::data_size_exception();
			}

			if (client_sockets.find(channelName) != client_sockets.end()) {
				SOCKET sock = client_sockets.at(channelName);
				int sendResult = send(sock, data.c_str(), data.size(), 0); //if you get errors add + 1 to end of data.size()
				if (sendResult == SOCKET_ERROR)
				{
					throw masesk::socket_error_exception();
				}
			}
		}

		void socketRecv(const std::string& channelName, void* buffer, UINT len) {
			if (len > BUFF_SIZE) {
				throw masesk::data_size_exception();
			}

			if (client_sockets.find(channelName) != client_sockets.end()) {
				SOCKET sock = client_sockets.at(channelName);
				int recvResult = recv(sock, (char*)buffer, len, NULL);
				if (recvResult == SOCKET_ERROR)
				{
					throw masesk::socket_error_exception();
				}
			}
			
		}

		void socketConnect(const std::string& channelName, const std::string& ip, std::uint16_t port) {
			if (sockInit() != 0) {
				throw masesk::socket_error_exception();
				return;
			}
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET || sock == SOCKET_ERROR)
			{
				sockQuit();
				throw masesk::socket_error_exception();
			}
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(port);
			inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);
			int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
			if (connResult == SOCKET_ERROR)
			{
				sockClose(sock);
				sockQuit();
				throw socket_error_exception();
			}
			client_sockets[channelName] = sock;
			

		}
		void closeConnection(const std::string& channelName) {
			if (client_sockets.find(channelName) != client_sockets.end()) {
				SOCKET s = client_sockets.at(channelName);
				sockClose(s);
				sockQuit();
			}

		}
	private:

		std::unordered_map<std::string, SOCKET> client_sockets;
		std::unordered_map<std::string, SOCKET> server_sockets;
		int sockInit(void)
		{
#ifdef _WIN32
			WSADATA wsa_data;
			return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
			return 0;
#endif
		}

		int sockQuit(void)
		{
#ifdef _WIN32
			return WSACleanup();
#else
			return 0;
#endif
		}
		int sockClose(SOCKET sock)
		{

			int status = 0;

#ifdef _WIN32
			status = shutdown(sock, SD_BOTH);
			if (status == 0) { status = closesocket(sock); }
#else
			status = shutdown(sock, SHUT_RDWR);
			if (status == 0) { status = close(sock); }
#endif

			return status;

		}
	};
}
#endif