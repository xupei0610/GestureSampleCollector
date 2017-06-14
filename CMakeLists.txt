cmake_minimum_required  (VERSION 3.1.0)
project (GestureSampleCollector)

set (PROJECT_SOURCE_DIR ${CMAKE_BINARY_DIR}/../src)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_include)
find_package (Qt5Core REQUIRED)
find_package (Qt5Widgets REQUIRED)
find_package (OpenCV 3.0.0 REQUIRED
    opencv_core
    opencv_video 
    opencv_videoio
    opencv_imgproc
    opencv_imgcodecs 
)

file (GLOB COLLECTOR_SRC_FILES
    ${PROJECT_SOURCE_DIR}/collector.cpp
    ${PROJECT_SOURCE_DIR}/config.h
    ${PROJECT_SOURCE_DIR}/GestureSampleCollector.cpp
    ${PROJECT_SOURCE_DIR}/CvQtImgConvertor.cpp
    ${PROJECT_SOURCE_DIR}/Singleton.cpp
    ${PROJECT_SOURCE_DIR}/MainView.cpp
    ${PROJECT_SOURCE_DIR}/SettingsView.cpp
    ${PROJECT_SOURCE_DIR}/MonitorView.cpp
    ${PROJECT_SOURCE_DIR}/Settings.cpp
    ${PROJECT_SOURCE_DIR}/HandDetector.cpp
    ${PROJECT_SOURCE_DIR}/SampleCollector.cpp
)
add_executable (collector ${COLLECTOR_SRC_FILES})
target_link_libraries (collector
    ${OpenCV_LIBRARIES}
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
)

# file (GLOB PROCESSOR_SRC_FILES
#     ${PROJECT_SOURCE_DIR}/processor.cpp
#     ${PROJECT_SOURCE_DIR}/config.h
#     ${PROJECT_SOURCE_DIR}/CvQtImgConvertor.cpp
#     ${PROJECT_SOURCE_DIR}/HandDetector.cpp
# )
# add_executable (processor ${PROCESSOR_SRC_FILES})
# target_link_libraries (processor
#     ${OpenCV_LIBRARIES}
#     ${Qt5Core_LIBRARIES}
#     ${Qt5Widgets_LIBRARIES}
# )
