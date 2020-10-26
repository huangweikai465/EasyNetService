aux_source_directory(. DIR_SERVER_SRCS)
aux_source_directory(. DIR_MANAGER_SRCS)

#Server程序不包含Manager.cpp
list(REMOVE_ITEM DIR_SERVER_SRCS "./Manager.cpp")
#Manager程序不包含Server.cpp
list(REMOVE_ITEM DIR_MANAGER_SRCS "./Server.cpp")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/Server)

add_executable(EasyNetServiceServer ${DIR_SERVER_SRCS} ${SRCS})

add_executable(EasyNetServiceManager ${DIR_MANAGER_SRCS} ${SRCS})
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    TARGET_LINK_LIBRARIES( EasyNetServiceManager
    pthread
    )
    TARGET_LINK_LIBRARIES( EasyNetServiceServer
    pthread
    )
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
