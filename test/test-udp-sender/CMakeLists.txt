add_executable( udp_sender
	udp_sender.cpp
)

target_link_libraries(udp_sender easy-socket)
