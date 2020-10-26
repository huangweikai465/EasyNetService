add_executable(ensclient Client.cpp)
target_link_libraries(ensclient ${ENSCLIENTLIBNAME})
install(TARGETS ensclient DESTINATION bin)

add_executable(ensserver Server.cpp)
target_link_libraries(ensserver ${ENSSERVERLIBNAME})
install(TARGETS ensserver DESTINATION bin)

add_executable(ensmanager Manager.cpp)
target_link_libraries(ensmanager ${ENSSERVERLIBNAME})
install(TARGETS ensmanager DESTINATION bin)
