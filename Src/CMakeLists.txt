#包括文件
aux_source_directory(${PROJECT_SOURCE_DIR}/Src/cJSON SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/Src/Socket SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/Src/Log SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/Src/Client SRCS_CLIENT)
aux_source_directory(${PROJECT_SOURCE_DIR}/Src/Server SRCS_SERVER)
#客户端库
add_library(${ENSCLIENTLIBNAME} STATIC ${SRCS} ${SRCS_CLIENT})
IF (UNIX)
	target_link_libraries(${ENSCLIENTLIBNAME} pthread)
ELSEIF (WIN32)
	target_link_libraries(${ENSCLIENTLIBNAME} Iphlpapi ws2_32)
ENDIF (UNIX)
#服务端库
add_library(${ENSSERVERLIBNAME} STATIC ${SRCS} ${SRCS_SERVER})

IF (UNIX)
	target_link_libraries(${ENSSERVERLIBNAME} pthread)
ELSEIF (WIN32)
	target_link_libraries(${ENSSERVERLIBNAME} Iphlpapi ws2_32)
ENDIF (UNIX)

