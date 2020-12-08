add_executable( tcp_server
	tcp_server.cpp
)

target_link_libraries(tcp_server easy-socket)
