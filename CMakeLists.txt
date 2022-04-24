cmake_minimum_required(VERSION 2.8.12)
project(DaddysSoccer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../sdlpp ${CMAKE_BINARY_DIR}/sdlpp)

# Files
set(S src)

set(DADDYS_SOCCER_SOURCE_FILES
    ${S}/bouncedobject.h    ${S}/bouncedobject.cpp
    ${S}/displayobject.h    ${S}/displayobject.cpp
    ${S}/geometry.h         ${S}/geometry.cpp
    ${S}/mainwindow.h       ${S}/mainwindow.cpp
    ${S}/main.cpp
)

# Target
add_executable(DaddysSoccer
    ${DADDYS_SOCCER_SOURCE_FILES}
)

target_link_libraries(DaddysSoccer
    PUBLIC SdlPP
)
