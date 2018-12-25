cmake_minimum_required(VERSION 3.1)

add_executable(final_gui
  ${CMAKE_CURRENT_LIST_DIR}/ADIPFinalProject1/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ADIPFinalProject1/mainwindow.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ADIPFinalProject1/mainwindow.ui
  ${CMAKE_CURRENT_LIST_DIR}/ADIPFinalProject1/widget.cpp
  )
sweet_target_add(final_gui LIBRARIES PRIVATE common kp_extractor)
sweet_target_add(final_gui CONFIGS Qt5Widgets Qt5Core Threads)
sweet_target_add(final_gui INCLUDES PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ADIPFinalProject1")

# qt5_wrap_ui(final_gui "${CMAKE_CURRENT_LIST_DIR}/ADIPFinalProject1/mainwindow.ui")
