﻿include_directories(${PROJECT_SOURCE_DIR}/ImGui/inc)
add_library(ImGui imgui.cpp imgui_demo.cpp 
        imgui_draw.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp 
        imgui_tables.cpp imgui_widgets.cpp ../../Window.cpp ../../Window.h)