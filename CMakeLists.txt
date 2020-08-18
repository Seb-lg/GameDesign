cmake_minimum_required(VERSION 3.13)
project(GameDesign)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads)


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
        ecs/SceneTree.hpp
        ecs/SceneTree.cpp

        components/Position3D.cpp
        components/Position3D.hpp
        components/Shapes.cpp
        components/Shapes.hpp
        components/Particle.cpp
        components/Particle.hpp
        components/Speed3D.cpp
        components/Speed3D.hpp
        components/Keyboard.cpp
        components/Keyboard.hpp
        components/Hitbox.cpp
        components/Hitbox.hpp
        components/Flock.cpp
        components/Flock.hpp
        components/pathfinding.cpp
        components/pathfinding.hpp

        interfaces/LoadObject.cpp
        interfaces/LoadObject.hpp
        interfaces/Movement.cpp
        interfaces/Movement.hpp

        Graphics.cpp
        Graphics.hpp
        Source.cpp
        Config.hpp
        components/Action.cpp components/Action.hpp)

target_link_libraries(GameDesign ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} glfw GLEW)
