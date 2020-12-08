add_executable( udp_receiver
	udp_receiver.cpp
)

target_link_libraries(udp_receiver easy-socket)
