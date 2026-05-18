cmake_minimum_required(VERSION 3.21)
project(DungeonTavern LANGUAGES CXX)

# Project layout
set(MAIN_DIR ${CMAKE_SOURCE_DIR})
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${MAIN_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES}
        include/sceneObjects.hpp
        src/sceneObjects.cpp
        include/NPCInteraction.hpp
        src/NPCInteraction.cpp
        include/DialogBox.hpp
        src/DialogBox.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "${MAIN_DIR}/include")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Disable debug info for Libs.cpp to speed up compilation
set_source_files_properties(${MAIN_DIR}/src/Libs.cpp PROPERTIES COMPILE_OPTIONS "-g0")

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# GLFW + GLM via FetchContent
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glm)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm)

# Compile shaders to SPIR-V
file(GLOB SHADERS CONFIGURE_DEPENDS
        "${MAIN_DIR}/shaders/*.vert"
        "${MAIN_DIR}/shaders/*.frag")

find_program(GLSLC glslc REQUIRED)

set(SPIRV_OUTPUTS "")
foreach(SHADER ${SHADERS})
  get_filename_component(NAME "${SHADER}" NAME)
  set(OUT "${CMAKE_BINARY_DIR}/shaders/${NAME}.spv")
  add_custom_command(
          OUTPUT "${OUT}"
          COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders"
          COMMAND ${GLSLC} -o "${OUT}" "${SHADER}"
          DEPENDS "${SHADER}"
          VERBATIM)
  list(APPEND SPIRV_OUTPUTS "${OUT}")
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPIRV_OUTPUTS})
add_dependencies(${PROJECT_NAME} shaders)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${MAIN_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")