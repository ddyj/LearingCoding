get_filename_component(tarName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE CPP_SOURCES  "*.cpp")
foreach(cpp_path ${CPP_SOURCES})
    get_filename_component(filename ${cpp_path} NAME_WE)

    add_executable(${filename} ${cpp_path})
    set_target_properties(${filename} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/${tarName}")
endforeach()

