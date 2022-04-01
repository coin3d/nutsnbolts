# tests performed on SoQt only
set(Gui Qt)
set(WIDGET QWidget*)

find_package(SoQt REQUIRED)

add_executable(soqtviewer soqtviewer.cpp)
target_link_libraries(soqtviewer SoQt::SoQt NutsnBolts)

configure_file(viewer.cpp.in viewer.cpp @ONLY)
add_executable(viewer ${CMAKE_CURRENT_BINARY_DIR}/viewer.cpp)
target_link_libraries(viewer SoQt::SoQt NutsnBolts)
