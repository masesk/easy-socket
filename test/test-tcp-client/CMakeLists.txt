add_executable( tcp_client
	tcp_client.cpp
)

target_link_libraries(tcp_client easy-socket)
