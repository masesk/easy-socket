add_executable( server
	Server.cpp
)

target_link_libraries(server easy-socket)