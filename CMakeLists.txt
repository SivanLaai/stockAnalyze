cmake_minimum_required(VERSION 3.5)

project(StockAnalyze LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include/XTPApiDemo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include/XTPApiDemo/api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib)
# include_directories(${PROJECT_SOURCE_DIR}/api)
# link_directories(${PROJECT_SOURCE_DIR}/api)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

# file(GLOB APP_HEAD *.h)
# file(GLOB APP_SRC *.cpp)
set(XTP_BASE_LIBS
          debug xtptraderapi debug xtpquoteapi
           optimized xtptraderapi optimized xtpquoteapi
        )

add_executable(StockAnalyze
  main.cpp
  mainwindow.cpp
  image_widget.cpp
  info_widget.cpp
  option_widget.cpp
  search_widget.cpp
  title_logo.cpp
  title_menu.cpp
  title_quitbox_widget.cpp
  title_slots.cpp
  title_widget.cpp
  mouse.cpp
  frameless.cpp
  time.cpp
  search_person_widget.cpp
  person_dialog.cpp
  person_dialog_slot.cpp
  login_dialog.cpp
  register_dialog.cpp
  image_line_show.cpp
  stockDataGet.cpp
  painter.cpp
  dataCal.cpp
  completelineedit.cpp
  image_info_show.cpp
  selectstockwidget.cpp
  qwebdata.cpp
  selectstock_widget.cpp
  mainwindow.ui
  login_dialog.ui
  register_dialog.ui
  image_line_show.ui
  selectstockwidget.ui
  imgrec.qrc
  webgets.cpp
  quote_spi.cpp
  trade_spi.cpp
  FileUtils.cpp
)

target_link_libraries(StockAnalyze Qt5::Widgets Qt5::Network ${XTP_BASE_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(StockAnalyze PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(StockAnalyze PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
