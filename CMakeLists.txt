cmake_minimum_required(VERSION 3.13)
project(GameDesign)
project(GameTest)

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

#        particle/CubeEmitter.cpp
#        particle/ParticleEffect.cpp
#        particle/Particle.hpp
#        particle/Random.hpp
#        particle/CubeEmitter.hpp
#        particle/ParticleEffect.hpp
#        particle/ParticleSystemPCH.cpp
#        particle/SphereEmitter.cpp
#        particle/ParticleEmitter.hpp
#        particle/ParticleSystemPCH.hpp
#        particle/SphereEmitter.hpp
#        particle/Interpolator.hpp


        Particle.cpp
        Particle.hpp

        Graphics.cpp
        Graphics.hpp
        Shapes.cpp
        Source.cpp LoadObject.cpp LoadObject.hpp ecs/SceneTree.hpp ecs/SceneTree.cpp component/pathfinding.cpp component/pathfinding.hpp component/rigidbody.cpp component/rigidbody.hpp
        Shapes.hpp
        Source.cpp
        LoadObject.cpp
        LoadObject.hpp
        Config.hpp)

add_executable(GameTest
        test/MainTest.cpp
        test/controls.cpp
        test/controls.hpp
        test/texture.cpp
        test/texture.hpp
        test/shader.cpp
        test/shader.hpp
        )

target_link_libraries(GameDesign ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} glfw GLEW)
target_link_libraries(GameTest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} glfw GLEW)
