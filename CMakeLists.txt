cmake_minimum_required(VERSION 3.0...3.31)

project(PlonqCraft)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
)
FetchContent_MakeAvailable(glfw3)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG 1ecd45775d96f35170458e6b148eb0708967e402
        GIT_PROGRESS TRUE
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL Profile" FORCE)
set(GLAD_API "gl=4.6" CACHE STRING "OpenGL API Version" FORCE)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)
FetchContent_MakeAvailable(glm)

add_custom_target(COPY_RESOURCES ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
        COMMENT "Copying res folder to build directory"
)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Graphics/Window.cpp
        src/Graphics/Window.hpp
        src/Graphics/Primitives/Frame.cpp
        src/Graphics/Primitives/Frame.hpp
        src/Graphics/Material.cpp
        src/Graphics/Material.hpp
        src/Graphics/Shader.cpp
        src/Graphics/Shader.hpp
        src/Graphics/Texture.cpp
        src/Graphics/Texture.hpp
        src/Graphics/TimeQuery.hpp
        src/Input/Input.cpp
        src/Input/Input.hpp
        src/Graphics/Ssbo.hpp
        src/Graphics/ObjectBinder.hpp
        src/Camera/Camera.cpp
        src/Camera/Camera.hpp
        src/Voxel/Block.cpp
        src/Voxel/Block.hpp
        src/Voxel/Chunk.cpp
        src/Voxel/Chunk.hpp
        src/Random/Random.cpp
        src/Random/Random.hpp
)

add_dependencies(${PROJECT_NAME} COPY_RESOURCES)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glad glm)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${glad_SOURCE_DIR}/include
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)