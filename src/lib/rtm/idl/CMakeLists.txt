cmake_minimum_required (VERSION 3.0.2)

project (${RTCSKEL_PROJECT_NAME}
	VERSION ${RTM_VERSION}
	LANGUAGES CXX)

set(${PROJECT_NAME}_IDLS
	BasicDataType.idl
	DataPort.idl
	DataPort_OpenRTM.idl
	ExtendedDataTypes.idl
	InterfaceDataTypes.idl
	Manager.idl
	OpenRTM.idl
	RTC.idl
	SDOPackage.idl
	SharedMemory.idl
	ExtendedFsmService.idl)

if(CORBA MATCHES "TAO")
	set(${PROJECT_NAME}_IDLS ${${PROJECT_NAME}_IDLS} DataPortUDP.idl)
endif()

# .idl -> .cpp
openrtm_idl_compile_all(${PROJECT_NAME} "${${PROJECT_NAME}_IDLS}" "-I${PROJECT_SOURCE_DIR}" RTM_SKEL "")

# .cpp -> .o .so
link_directories(${ORB_LINK_DIR})

add_library(${PROJECT_NAME}_objlib OBJECT ${${PROJECT_NAME}_IDLSKEL})
openrtm_gencode_set_compile_props(${PROJECT_NAME}_objlib)
target_include_directories(${PROJECT_NAME}_objlib SYSTEM
	PUBLIC ${PROJECT_BINARY_DIR}
	PRIVATE ${CMAKE_BINARY_DIR}/src/lib
	${ORB_INCLUDE_DIR})
add_dependencies(${PROJECT_NAME}_objlib ${PROJECT_NAME}_IDLTGT)

if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
	add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_objlib>)
	openrtm_set_link_props_static(${PROJECT_NAME}-static)
	set_target_properties(${PROJECT_NAME}-static PROPERTIES DEBUG_POSTFIX d)
	set(DEF_FILE_NAME ${PROJECT_NAME}.def)
	add_custom_command(TARGET ${PROJECT_NAME}-static
		POST_BUILD
		COMMAND python -B ${MAKE_DEF_COMMAND} $<TARGET_FILE:${PROJECT_NAME}-static> ${PROJECT_NAME}$<$<CONFIG:debug>:d> ${RTM_SHORT_VERSION} ${DEF_FILE_NAME}
		BYPRODUCTS ${DEF_FILE_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
	set_source_files_properties(${DEF_FILE_NAME} PROPERTIES GENERATED 1)
	set_source_files_properties(${DEBUG_DEF_FILE_NAME} PROPERTIES GENERATED 1)

	add_library(${PROJECT_NAME} SHARED
		$<TARGET_OBJECTS:${PROJECT_NAME}_objlib>
		${CMAKE_CURRENT_BINARY_DIR}/${DEF_FILE_NAME}
		)
	target_link_libraries(${PROJECT_NAME} PRIVATE
		${ORB_LIBRARIES} ${COIL_PROJECT_NAME} ${RTM_LINKER_OPTION})
	openrtm_set_link_props_shared(${PROJECT_NAME})
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)
	add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-static)

	install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${INSTALL_RTM_LIB_DIR}
					ARCHIVE DESTINATION ${INSTALL_RTM_LIB_DIR}
					RUNTIME DESTINATION ${INSTALL_RTM_LIB_DIR}
					COMPONENT openrtm)
	install(TARGETS ${PROJECT_NAME}-static LIBRARY DESTINATION ${INSTALL_RTM_LIB_DIR}
					ARCHIVE DESTINATION ${INSTALL_RTM_LIB_DIR}
					RUNTIME DESTINATION ${INSTALL_RTM_LIB_DIR}
					COMPONENT openrtm)
endif(WIN32)

# install .idl .h
install(FILES ${${PROJECT_NAME}_IDLS} DESTINATION ${INSTALL_RTM_INCLUDE_DIR}/rtm/idl COMPONENT idl)
foreach(hdr ${${PROJECT_NAME}_IDLHDR})
	install(FILES "${PROJECT_BINARY_DIR}/${hdr}" DESTINATION ${INSTALL_RTM_INCLUDE_DIR}/rtm/idl COMPONENT headers)
endforeach()


foreach(idl ${${PROJECT_NAME}_IDLS})
	string(REGEX REPLACE ".idl" "" idl ${idl})
	if(DEPENDENCES_IDLS)
		set(DEPENDENCES_IDLS ${DEPENDENCES_IDLS}:rtm/idl/${idl})
	else()
		set(DEPENDENCES_IDLS rtm/idl/${idl})
	endif()
endforeach()

set(RTM_INCLUDE_IDLS ${DEPENDENCES_IDLS} PARENT_SCOPE)

add_subdirectory(device_interfaces)
