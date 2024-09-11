project(server)

add_library(lserver
            app.cpp
            connect.cpp
            hub.cpp
            server.cpp
            logger.cpp)

add_executable(server
            main.cpp
            server.cpp
            connect.cpp
            hub.cpp
            app.cpp
            logger.cpp)

target_link_libraries(lserver PRIVATE
            spdlog::spdlog
            objects)

target_link_libraries(server PRIVATE   
            spdlog::spdlog
            objects)
