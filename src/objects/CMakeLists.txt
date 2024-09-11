project(objects)

add_library(objects STATIC 
            objects.cpp
            msg_objects.cpp)

target_link_libraries(objects PUBLIC)
