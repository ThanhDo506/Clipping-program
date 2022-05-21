//
// Created by SeiSho on 06/05/2022.
//

#ifndef CLIPPING_PROGRAM_SRC_LIBS_H
#define CLIPPING_PROGRAM_SRC_LIBS_H

/*==========OpenGL==========*/
// #define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

//* OpenGL MATH
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
//* ImGUI
#include "../ImGui/inc/imgui.h"
#include "../ImGui/inc/imgui_impl_glfw.h"
#include "../ImGui/inc/imgui_impl_opengl3.h"

/*==========NOPE==========*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

#endif //OPENGLLEARN_SRC_LIBS_H
