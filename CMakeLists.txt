cmake_minimum_required(VERSION 3.28)

project(zf4_example_game)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)

add_subdirectory(zf4)

find_package(glfw3 CONFIG REQUIRED)

add_executable(zf4_example_game
	src/entry.c
	src/title_screen.c
	src/world.c
	src/ents.c
	src/tilemap.c

	src/asset_indexes.h
	src/scene_types.h
	src/title_screen.h
	src/world.h
	src/ents.h
	src/tilemap.h
)

target_include_directories(zf4_example_game PRIVATE
    zf4/zf4/include
    zf4/zf4_common/include
	zf4/vendor/glad/include
)

target_link_libraries(zf4_example_game PRIVATE zf4 zf4_common glfw)

target_compile_definitions(zf4_example_game PRIVATE GLFW_INCLUDE_NONE)

add_dependencies(zf4_example_game zf4_asset_packer)

add_custom_command(TARGET zf4_example_game POST_BUILD
	COMMAND $<TARGET_FILE:zf4_asset_packer> ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}
)
