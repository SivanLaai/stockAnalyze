cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

# if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
# endif()

include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include/XTPApiDemo)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include/XTPApiDemo/api)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/src/XTPApiDemo)
include_directories(${PROJECT_SOURCE_DIR}/src/data_process)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include)
link_directories(${PROJECT_SOURCE_DIR}/thirdparty/lib)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)


aux_source_directory(data_process DATA_PROCESS)
aux_source_directory(${PROJECT_SOURCE_DIR}/thirdparty/src/XTPApiDemo XTPApiDemo)
aux_source_directory(. ROOT)
file(GLOB resources_ui *.ui)
file(GLOB resources_qrc *.qrc)

set(Analyze_SRC
    ${ROOT}
    ${DATA_PROCESS}
    ${XTPApiDemo})
set(XTP_BASE_LIBS
          debug xtptraderapi debug xtpquoteapi
           optimized xtptraderapi optimized xtpquoteapi
        )

add_executable(StockAnalyze
   ${Analyze_SRC}
   ${resources_ui}
   ${resources_qrc}
)

target_link_libraries(StockAnalyze Qt5::Widgets Qt5::Network ${XTP_BASE_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(StockAnalyze PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(StockAnalyze PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
