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

namespace masesk
{
	const int BUFF_SIZE = 4096;
	struct socket_error_exception : public std::exception
	{
		const char *what() const throw()
		{
			return "Can't start socket!";
		}
	};
	struct invalid_socket_exception : public std::exception
	{
		const char *what() const throw()
		{
			return "Can't create a socket!";
		}
	};
	struct data_size_exception : public std::exception
	{
		const char *what() const throw()
		{
			return "Data size is above the maximum allowed by the buffer";
		}
	};
	struct sock_save {
		SOCKET sock;
		sockaddr_in addr;
	};
	class EasySocket
	{
	public:
		template <typename T, typename M>
		void socketListenTCP(const std::string &channelName, const std::uint16_t &port, std::function<T(const M &data)> callback)
		{

			if (sockInit() != 0)
			{
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
			bind(listening, (sockaddr *)&hint, sizeof(hint));
			listen(listening, SOMAXCONN);
			sockaddr_in client;
#ifdef _WIN32
			int clientSize = sizeof(client);
#else
			unsigned int clientSize = sizeof(client);
#endif
			SOCKET clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
			sock_save tcp_save;
			tcp_save.sock = clientSocket;
			server_sockets[channelName] = tcp_save;
			char host[NI_MAXHOST];
			char service[NI_MAXSERV];

			memset(host, 0, NI_MAXHOST);
			memset(service, 0, NI_MAXSERV);
			if (getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			{
				//std::cout << host << " connected on port " << service << std::endl;
			}
			else
			{
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
				//std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
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
				if (bytesReceived > BUFF_SIZE)
				{
					throw masesk::data_size_exception();
				}
				if (bytesReceived > 0)
				{
					callback(std::string(buff, 0, bytesReceived));
				}
				else
				{
					break;
				}
			}
			sockClose(clientSocket);
			sockQuit();
		}

		void socketSendTCP(const std::string &channelName, const std::string &data)
		{
			if (data.size() > BUFF_SIZE)
			{
				throw masesk::data_size_exception();
			}

			if (client_sockets.find(channelName) != client_sockets.end())
			{
				SOCKET sock = client_sockets.at(channelName).sock;
				int sendResult = send(sock, data.c_str(), (int)data.size() + 1, 0);
				if (sendResult == SOCKET_ERROR)
				{
					throw masesk::socket_error_exception();
				}
			}
		}

		void socketConnectTCP(const std::string &channelName, const std::string &ip, const std::uint16_t &port)
		{
			if (sockInit() != 0)
			{
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
			int connResult = connect(sock, (sockaddr *)&hint, sizeof(hint));
			if (connResult == SOCKET_ERROR)
			{
				sockClose(sock);
				sockQuit();
				throw socket_error_exception();
			}
			client_sockets[channelName].sock = sock;
		}
		void closeConnection(const std::string &channelName)
		{
			if (client_sockets.find(channelName) != client_sockets.end())
			{
				SOCKET s = client_sockets.at(channelName).sock;
				sockClose(s);
				sockQuit();
			}
		}


		void socketInitSendUDP(const std::string &channelName, const std::string &ip, const std::uint16_t &port) {

			if (client_sockets.find(channelName) == client_sockets.end()) {
				if (sockInit() != 0)
				{
					throw masesk::socket_error_exception();
					return;
				}
				SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
				sockaddr_in servaddr;
				memset(&servaddr, 0, sizeof(servaddr));
				servaddr.sin_family = AF_INET;
				servaddr.sin_port = htons(port);
				inet_pton(AF_INET, ip.c_str(), &(servaddr.sin_addr));
				sock_save udp_sock;
				udp_sock.sock = sock;
				udp_sock.addr = servaddr;
				client_sockets[channelName] = udp_sock;
			}
		}

		void socketSendUDP(const std::string &channelName, const std::string &data)
		{
			if (client_sockets.find(channelName) != client_sockets.end())
			{
				sock_save udp_sock = client_sockets.at(channelName);
				SOCKET sock = udp_sock.sock;
				sockaddr_in servaddr = udp_sock.addr;
				int result = sendto(sock, data.c_str(), (int)data.length(),
					0, (const struct sockaddr *)&servaddr,
					sizeof(servaddr));
				if (result == SOCKET_ERROR)
				{
					throw masesk::socket_error_exception();
				}
			}
		}

		template <typename T, typename M>
		void socketListenUDP(const std::string &channelName, const std::uint16_t &port, std::function<T(const std::string &M)> callback)
		{
			if (sockInit() != 0)
			{
				throw masesk::socket_error_exception();
				return;
			}
			sockaddr_in servaddr;
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(port);
#ifdef _WIN32
			int serverSize = sizeof(servaddr);
#else
			unsigned int serverSize = sizeof(servaddr);
#endif
			inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
			SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
			bind(sock, (sockaddr *)&servaddr, serverSize);

			server_sockets[channelName].sock = sock;
			char buff[BUFF_SIZE];
			sockaddr_in clientAddr;
			servaddr.sin_port = htons(port);
#ifdef _WIN32
			int clientSize = sizeof(servaddr);
#else
			unsigned int clientSize = sizeof(servaddr);
#endif

			while (1)
			{
				memset(buff, 0, BUFF_SIZE);
				int bytesRecieved = recvfrom(sock, (char *)buff, BUFF_SIZE,
											 0, (struct sockaddr *)&clientAddr,
											 &clientSize);
				if (bytesRecieved > 0)
				{
					callback(std::string(buff, 0, bytesRecieved));
				}
			}
		}

	private:
		std::unordered_map<std::string, sock_save> client_sockets;
		std::unordered_map<std::string, sock_save> server_sockets;
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
			if (status == 0)
			{
				status = closesocket(sock);
			}
#else
			status = shutdown(sock, SHUT_RDWR);
			if (status == 0)
			{
				status = close(sock);
			}
#endif

			return status;
		}
	};
} // namespace masesk
#endif
