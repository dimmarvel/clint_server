project(client)

add_library(lclient client.cpp)

add_executable(client 
            main.cpp
            client.cpp)
            
target_link_libraries(lclient PRIVATE
            gcov
            spdlog::spdlog
            objects)

target_link_libraries(client PRIVATE
            gcov
            spdlog::spdlog
            objects)