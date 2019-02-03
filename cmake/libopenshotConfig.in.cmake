include(CMakeFindDependencyMacro)

# Any extra setup
find_dependency(libopenshot-audio)
find_dependency(Qt5 REQUIRED COMPONENTS Core Widgets Gui Multimedia MultimediaWidgets)
find_dependency(ZeroMQ)
find_dependency(jsoncpp)
find_dependency(OpenMP)
find_dependency(BZip2)

# Add the targets file
include("${CMAKE_CURRENT_LIST_DIR}/libopenshotTargets.cmake")