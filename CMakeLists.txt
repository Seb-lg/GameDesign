cmake_minimum_required(VERSION 3.13)
project(GameDesign)
project(GameTest)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
include_directories(.)
include_directories(ecs)
include_directories(test)

add_executable(GameDesign
        ecs/ComponentList.hpp
        ecs/DataBank.hpp
        ecs/Ecs.cpp
        ecs/Ecs.hpp
        ecs/Entity.cpp
        ecs/Entity.hpp
        ecs/Time.cpp
        ecs/TimedEventAdmin.cpp
        ecs/TimedEventAdmin.hpp
        ecs/TimedEvent.cpp
        ecs/TimedEvent.hpp
        ecs/Time.hpp
        ecs/Vector2.hpp

        Graphics.cpp
        Graphics.h
        Shapes.cpp
        Shapes.h
        Source.cpp LoadObject.cpp LoadObject.hpp ecs/SceneTree.hpp ecs/SceneTree.cpp Particle.cpp Particle.hpp)

add_executable(GameTest
        test/MainTest.cpp
        test/controls.cpp
        test/controls.hpp
        test/texture.cpp
        test/texture.hpp
        test/shader.cpp
        test/shader.hpp
        )

target_link_libraries(GameDesign ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} glfw)
target_link_libraries(GameTest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} glfw)