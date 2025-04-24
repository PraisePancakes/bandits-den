cmake_minimum_required(VERSION 3.28)

project(BanditsDen VERSION 0.0.1 DESCRIPTION "BanditsDen : a bandits adventure" HOMEPAGE_URL "https://github.com/PraisePancakes/bandits-den" LANGUAGES CXX)


set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)

if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

add_executable(${PROJECT_NAME} main.cpp) 
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-missing-field-initializers)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)


