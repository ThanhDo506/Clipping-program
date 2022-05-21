//
// Created by SeiSho on 10/05/2022.
//
#include "src/libs.h"

#ifndef CLIPPING_PROGRAM__WINDOW_H
#define CLIPPING_PROGRAM__WINDOW_H

class Window {
  private:
    GLFWwindow *window               = nullptr;
    int        screenWidth;
    int        screenHeight;
    GLuint     VertexArrayID;
    GLuint     vertexbuffer[5];
    GLuint     programID;
    GLfloat    lineWidth             = 1.0;
    bool       drawBorder            = true;
    bool       drawTriangle          = false;
    bool       drawLine              = true;
    bool       drawCohenClippingLine = false;
    bool       drawLyangClippingLine = false;
    GLfloat    vertices[5 * 3]         = {
        -1.0f, -1.0f, 0.5, 0.5, 0.5,
         1.0f, -1.0f, 0.5, 0.5, 0.5,
        0.0f, 1.0f, 0.5, 0.5, 0.5
    };
    GLfloat    borderVerticals[5 * 4]  = {
        -0.1,  -0.1,  0.5, 0.65, 0.73, //Down left
        -0.1,   0.1,  0.5, 0.65, 0.73,  //Top left
        0.1,  0.1, 0.5, 0.65, 0.73,   //Top Right
        0.1, -0.1, 0.5, 0.65, 0.73   //Down right
    };
    GLfloat    lineDrawVerticals[5 * 2]  = { 0.0, 0.0, 0.65, 0.75, 0.23, 
                                             0.5, 0.5, 0.65, 0.88, 0.25
    };
    GLfloat    lineCohenVerticals[5 * 2] = {0.0, 0.0, 0.85, 0.5, 0.10, 
                                           -0.5, -0.5, 0.85, 0.5, 0.10
    };
    GLfloat    lineLyangVerticals[5 * 2] = {0.0, 0.0, 0.85, 0.95, 0.55, 
                                            -0.5, -0.5, 0.85, 0.95, 0.55
    };
  public:
    Window (int w, int h);
    ~Window ();
    void run ();
    static void framebuffer_size_callback (GLFWwindow *window, int width, int height);
    void processInput (GLFWwindow *window);
    GLuint LoadShaders (const char *vertex_file_path, const char *fragment_file_path);
    void initImGui ();
    void controlerImGui ();
    void renderImGui ();
    void initGLEW ();
    void initGLFW ();
    void cleanGLFW ();
    void cleanImGui ();
    void initScreen ();
    void bindBuffer ();
    void cleanBuffer ();
  private:
    // * Cohen
    int computeCode(double x, double y);
    void cohenSutherlandClip(double x1, double y1,
                              double x2, double y2,
                              double x_min, double x_max, 
                              double y_min, double y_max);
    // * Lyang Barsky
    void clipline(double x1, double y1, 
                   double x2, double y2, 
                   double x_min, double y_min, 
                   double x_max, double y_max);
    bool cliptest(double p, double q, double *u0, double *u1);
    // Định nghĩa code các vùng
    const int INSIDE = 0; // 0000
    const int LEFT   = 1; // 0001
    const int RIGHT  = 2; // 0010
    const int BOTTOM = 4; // 0100
    const int TOP    = 8; // 1000
};

#endif //CLIPPING_PROGRAM__WINDOW_H
