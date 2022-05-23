//
// Created by SeiSho on 10/05/2022.
//

#include "Window.h"

Window::Window (int w, int h) {
    screenWidth  = w;
    screenHeight = h;
    initGLFW();
    initGLEW();
    initScreen();
    initImGui();
}

Window::~Window () {
    cleanImGui();
    glfwTerminate();
}

void Window::initGLFW () {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __MACOS__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    // --------------------
    window = glfwCreateWindow(screenWidth, screenHeight,
                              "Visual Clipping program",
                              NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::string("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    
    // glViewport(0, 0, screenWidth, screenHeight);
    // glMatrixMode (GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho (0, screenWidth, 0, screenHeight, 0, 1);
    // glMatrixMode (GL_MODELVIEW);
    // glLoadIdentity();
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Window::framebuffer_size_callback (GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

void Window::initGLEW () {
    glewExperimental = GL_TRUE;
    glewInit();
}

void Window::initImGui () {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui::SetCurrentContext();
    // ImGuiIO &io = ImGui::GetIO();
    // (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Window::renderImGui () {
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    // io.DisplaySize = ImVec2((float) screenWidth, (float) screenHeight);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // *** manage object Imgui in this function
    controlerImGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::processInput (GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::cleanImGui () {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

void Window::cleanGLFW () {
    glfwTerminate();
}

void Window::run () {
    bindBuffer();
    programID = LoadShaders("..\\GLSL\\VertexShader.vert",
                            "..\\GLSL\\FragmentShader.frag");
    glUseProgram(programID);
    
    float vertices1[] =
        {
            300, 300, 0.0, // top right corner
            0, 300, 0.0, // top left corner
            0, 0, 0.0, // bottom left corner
            300, 0, 0.0 // bottom right corner
        };
    
    while (!glfwWindowShouldClose(window)) {
        
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glEnableVertexAttribArray(0);

        if (drawBorder) {
            glEnableVertexAttribArray(0);
            // glPointSize(1);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }

        if (drawLine) {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
            glDrawArrays(GL_LINES, 0, 2);
        }

        if (drawCohenClippingLine) {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
            glDrawArrays(GL_LINES, 0, 2);
        }
        
        if (drawLyangClippingLine) {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[4]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
            glDrawArrays(GL_LINES, 0, 2);
        }
        glDisableVertexAttribArray(0);

        renderImGui();
        // glUseProgram(programID);

        // glUniform4f(glGetUniformLocation(programID, "cl"), color[0], color[1], color[2], color[3]);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// Manage object Imgui in this function
void Window::controlerImGui () {
    ImGui::Begin("Control");
    ImGui::Checkbox("Draw border", &drawBorder);
    // ImGui::Checkbox("Draw triangle", &drawTriangle);
    if(ImGui::ColorEdit3("Border color", (borderVerticals + 2))) {
        borderVerticals[7] = borderVerticals[12] = borderVerticals[17] = borderVerticals[2];
        borderVerticals[8] = borderVerticals[13] = borderVerticals[18] = borderVerticals[3];
        borderVerticals[9] = borderVerticals[14] = borderVerticals[19] = borderVerticals[4];
        bindBuffer();
    }
    ImGui::End();
    
    ImGui::Begin("Border controller");
    // ImGui::TextColored(ImVec4(0.45,0.65,0.95,1.0),"Left edge");

    if(ImGui::InputFloat("Top edge", &borderVerticals[6], 0.005, 0.0, "%.3f")) {
        borderVerticals[11] = borderVerticals[6];
        bindBuffer();
    }
    if(ImGui::InputFloat("Bottom edge", &borderVerticals[1], 0.005, 0.0, "%.3f")) {
        borderVerticals[16] = borderVerticals[1];
        bindBuffer();
    }
    if(ImGui::InputFloat("Right edge", &borderVerticals[10], 0.005, 0.0, "%.3f")) {
        borderVerticals[15] = borderVerticals[10];
        bindBuffer();
    }
    if(ImGui::InputFloat("Left edge", &borderVerticals[0], 0.005, 0.0, "%.3f")) {
        borderVerticals[5] = borderVerticals[0];
        bindBuffer();
    }
    if(ImGui::SliderFloat("Line width", &lineWidth,0.0, 10.0)) {
        glLineWidth(lineWidth);
    }
    
    ImGui::End();
    
    ImGui::Begin("Control line");
    ImGui::Checkbox ("Draw", &drawLine);
    if(ImGui::InputFloat2("Pos 1", lineDrawVerticals)){
        bindBuffer();
    }
    if(ImGui::InputFloat2 ("Pos 2", (lineDrawVerticals + 5))){
        bindBuffer();
    }
    if(ImGui::Checkbox ("Draw Cohen Clipping Line", &drawCohenClippingLine)){
        cohenSutherlandClip (lineDrawVerticals[0], lineDrawVerticals[1], lineDrawVerticals[5], lineDrawVerticals[6],
                             borderVerticals[0], borderVerticals[10], borderVerticals[1], borderVerticals[6]);
        // bindBuffer();
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof (lineCohenVerticals), lineCohenVerticals, GL_STATIC_DRAW);
    }
    if(ImGui::ColorEdit4 ("Color Cohen line",(lineCohenVerticals + 2)) ){
        lineCohenVerticals[7] = lineCohenVerticals[2];
        lineCohenVerticals[8] = lineCohenVerticals[3];
        lineCohenVerticals[9] = lineCohenVerticals[4];
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof (lineCohenVerticals), lineCohenVerticals, GL_STATIC_DRAW);
    }
    
    if(ImGui::Checkbox ("Draw Lyang clipping line", &drawLyangClippingLine)) {
        clipline (lineDrawVerticals[0], lineDrawVerticals[1], lineDrawVerticals[5], lineDrawVerticals[6],
                  borderVerticals[0], borderVerticals[1], borderVerticals[10], borderVerticals[6]);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof (lineLyangVerticals), lineLyangVerticals, GL_STATIC_DRAW);
    }
    if(ImGui::ColorEdit4 ("Color lyang line",(lineLyangVerticals + 2)) ){
        lineLyangVerticals[7] = lineLyangVerticals[2];
        lineLyangVerticals[8] = lineLyangVerticals[3];
        lineLyangVerticals[9] = lineLyangVerticals[4];
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof (lineLyangVerticals), lineLyangVerticals, GL_STATIC_DRAW);
    }

    ImGui::End();
    // if (!ImGui::Begin("Settings")) {
    //     ImGui::End();
    // } else {
    //     ImGui::BeginChild("item view",
    //                       ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
    //     ImGui::Text("Settings");
    //     ImGui::Separator();
    //     if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
    //         if (ImGui::BeginTabItem("Standard settings")) {
    //             ImGui::TextWrapped(
    //                 "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
    //             ImGui::EndTabItem();
    //         }
    //         if (ImGui::BeginTabItem("Render settings")) {
    //             // ImGui::ColorPicker3("Background color", MainManager::getInstance().getProgramSettings()->backgroundColor.e);
    //             ImGui::EndTabItem();
    //         }
    //         ImGui::EndTabBar();
    //     }
    //     ImGui::EndChild();
    //     ImGui::End();
    // }
}

void Window::initScreen () {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(5, vertexbuffer);
    
    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(clippingScreenVertex), clippingScreenVertex, GL_STATIC_DRAW);
}

void Window::bindBuffer () {
    cout << "[LOG]: Bind buffer\n";
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(borderVerticals), borderVerticals, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof (lineDrawVerticals), lineDrawVerticals, GL_STATIC_DRAW);
}

GLuint Window::LoadShaders (const char *vertex_file_path, const char *fragment_file_path) {
    // Create the shaders
    GLuint VertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string   VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
               vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string   FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int   InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

// Tìm vùng của điểm
int Window::computeCode (double x, double y) {
    int code = INSIDE;
    // borderVerticals[0] = x_min
    if (x < borderVerticals[0]) // to the left of window
        code |= LEFT;
    // borderVerticals[10] = x_max
    else if (x > borderVerticals[10]) // to the right of window
        code |= RIGHT;
    // borderVerticals[1] = y_min
    if (y < borderVerticals[1]) // below the window
        code |= BOTTOM;
    // borderVerticals[6] = y_max
    else if (y > borderVerticals[6]) // above the window
        code |= TOP;
    return code;
}

void Window::cohenSutherlandClip (double x1, double y1, double x2, double y2,
                                  double x_min, double x_max, double y_min, double y_max) {
    // Tính vùng của 2 diểm
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
 
    // Initialize line as outside the rectangular window
    bool accept = false;
 
    while (true) {
        if ((code1 == INSIDE) && (code2 == INSIDE)) {
            // Nếu cả 2 điểm nằm trong cửa sổ
            accept = true;
            break;
        } else if (code1 & code2) {
            // Nếu cả 2 điểm nằm ngoài cửa sổ
            // và cùng trong 1 vùng
            break;
        } else {
            // Có 1 phần nằm trong cửa sổ hiển thị
            int code_out;
            double x, y;
            // Có ít nhất 1 điểm nằm ngoài cửa sổ
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;
 
            // Tìm điểm giao giữa đường thằng và biên;
            // y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP) {
                // điểm ở trên cửa sổ xén tỉa
                // borderVerticals[6] = y_max
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            } else if (code_out & BOTTOM) {
                // điểm ở dưới cửa sổ xén tỉa
                // borderVerticals[1] = y_min
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            } else if (code_out & RIGHT) {
                // điểm ở phía bên phải của cửa sổ xén tỉa
                // borderVerticals[10] = x_max
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            } else if (code_out & LEFT) {
                // điểm ở phía bên trái của cửa sổ xén tỉa
                // borderVerticals[0] = x_min
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }
            // Now intersection point x, y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept) {
        cout << "Line accepted from " << x1 << ", "
             << y1 << " to " << x2 << ", " << y2 << endl;
        // Here the user can add code to display the rectangle
        // along with the accepted (portion of) lines
        lineCohenVerticals[0] = x1;
        lineCohenVerticals[1] = y1;
        lineCohenVerticals[5] = x2;
        lineCohenVerticals[6] = y2;
    } else {
        cout << "Line rejected" << endl;
        lineCohenVerticals[0] = 0.0;
        lineCohenVerticals[1] = 0.0;
        lineCohenVerticals[5] = 0.0;
        lineCohenVerticals[6] = 0.0;
    }
}
void Window::cleanBuffer () {
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1,&vertexbuffer[0]);
    glDeleteBuffers(1,&vertexbuffer[1]);
    glDeleteBuffers(1,&vertexbuffer[2]);
    glDeleteBuffers(1,&vertexbuffer[3]);
    glDeleteProgram(programID);
}

bool Window::cliptest (double p, double q, double *u0, double *u1) {
    double r;
    bool res = true;
    if (p < 0.0) {
        r = q / p;
        if (r > *u1) {
            res = false;
        } else {
            if (r > *u0) {
                *u0 = r;
            }
        }
    } else {
        if (p > 0.0) {
            r = q / p;
            if (r < *u0) {
                res = false;
            } else {
                if (r < *u1) {
                    *u1 = r;
                }
            }
        } else {
            if (q < 0.0) {
                res = false;
            }
        }
    }
    return res;
}

void Window::clipline(double x1, double y1, double x2, double y2, double x_min,
               double y_min, double x_max, double y_max) {
    double u0 = 0.0, u1 = 1.0, dx, dy;
    dx = x2 - x1;
    if (cliptest(-dx, x1 - x_min, &u0, &u1))
        if (cliptest(dx, x_max - x1, &u0, &u1)) {
            dy = y2 - y1;
            if (cliptest(-dy, y1 - y_min, &u0, &u1))
                if (cliptest(dy, y_max - y1, &u0, &u1)) {
                    // tọa độ điiểm sau khi xén tỉa
                    if (u1 < 1.0) {
                        x2 = x1 + u1 * dx;
                        y2 = y1 + u1 * dy;
                    }
                    if (u0 > 0.0) {
                        x1 += u0 * dx;
                        y1 += u0 * dy;
                    }
                    // line(x1, y1, x2, y2);
                    lineLyangVerticals[0] = x1;
                    lineLyangVerticals[1] = y1;
                    lineLyangVerticals[5] = x2;
                    lineLyangVerticals[6] = y2;
                    cout << x1 << "," << y1 << "|" << x2 << "," << y2 << "\n";
                }
        }
}
