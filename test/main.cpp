/*
 * Copyright 2015 Daniel Kirchner
 *
 * This file is part of GLGUI.
 *
 * GLGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLGUI.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <oglp/oglp.h>
#include <GLFW/glfw3.h>
#include <glgui/Window.h>
#include <glgui/Box.h>
#include <glgui/renderer_gl3/GL3Renderer.h>
#include <glgui/Font.h>

class Window {
public:
    Window (void) {
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow (1280, 720, "GLGUI TEST", nullptr, nullptr);
        if (window == nullptr) throw std::runtime_error ("Cannot create GLFW window.");
    }
    ~Window (void) {
        glfwDestroyWindow (window);
    }
    operator GLFWwindow* (void) {
        return window;
    }
    operator const GLFWwindow* const (void) {
        return window;
    }
private:
    GLFWwindow *window;
};

class App {
public:
    App (void) : selected (nullptr) {
        glfwSetWindowUserPointer (window, this);
        glfwMakeContextCurrent(window);
        glfwShowWindow (window);

        oglp::Init ((oglp::GetProcAddressCallback)glfwGetProcAddress);

        glfwSetWindowCloseCallback (window, [] (GLFWwindow *window) {
            static_cast<App*> (glfwGetWindowUserPointer (window))->running = false;
        });
        glfwGetWindowSize (window, &width, &height);
        glfwSetWindowSizeCallback (window, [] (GLFWwindow *window, int width, int height) {
            static_cast<App*> (glfwGetWindowUserPointer (window))->OnResize (width, height);
        });
        glfwSetMouseButtonCallback (window, [] (GLFWwindow *window, int button, int action, int mods) {
           static_cast<App*> (glfwGetWindowUserPointer (window))->OnMouseButton (button, action, mods);
        });
        glfwSetCursorPosCallback (window, [] (GLFWwindow *window, double x, double y) {
            static_cast<App*> (glfwGetWindowUserPointer (window))->OnMouseMove (x, y);
        });

        renderer = new glgui::GL3Renderer ();

        guiwindow = new glgui::Window (width, height, 2048, false);
        renderer->ResizeViewport (width, height, 200);

        glgui::Box *box = new glgui::Box (guiwindow);
        box->SetSize (200, 200, 200);
        box->SetPivot (glgui::Widget::TOP|glgui::Widget::RIGHT|glgui::Widget::FRONT);
        box->SetX (50, false);
        box->SetX (0.5, true);
        box->SetY (50, false);
        box->SetZ (0.5, true);
        box->SetColor (1, 0, 0, 0.5);

        box = new glgui::Box (box);
        box->SetSize (200, 200, 100);
        box->SetColor (0, 1, 0, 0.2);
        box->SetX (60, false);
        box->SetY (60, false);
        box->SetZ (0.6, true);
        box->SetPivot (glgui::Widget::TOP|glgui::Widget::RIGHT|glgui::Widget::FRONT);

        glfwGetCursorPos (window, &cursor_x, &cursor_y);

        font = new glgui::Font ("/usr/share/fonts/TTF/DejaVuSans.ttf");
    }
    ~App (void) {
        delete font;
        delete guiwindow;
        delete renderer;
    }
    void OnMouseMove (double x, double y) {
        if (selected != nullptr) {
            double dx = cursor_x - x;
            double dy = y - cursor_y;
            selected->SetX (selected->GetAbsoluteX () + dx, false);
            selected->SetY (selected->GetAbsoluteY () + dy, false);
        }

        cursor_x = x;
        cursor_y = y;

    }
    void OnMouseButton (int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                selected = reinterpret_cast<glgui::Widget*> (renderer->Select (cursor_x, height - cursor_y));
            } else if (action == GLFW_RELEASE) {
                selected = nullptr;
            }
        }
    }
    void OnResize (int _width, int _height) {
        width = _width; height = _height;
        guiwindow->SetSize (width, height, 200);
        renderer->ResizeViewport (width, height, 200);
    }
    void run (void) {
        running = true;
        while (running) {
            glfwPollEvents ();

            gl::Viewport (0, 0, width, height);

            gl::ClearColor (0.1, 0.1, 0.1, 0);
            gl::Clear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            renderer->Reset ();
            renderer->Display (guiwindow);

            glfwSwapBuffers (window);
        }
    }
private:
    Window window;
    int width;
    int height;
    glgui::Window *guiwindow;
    glgui::GL3Renderer *renderer;
    glgui::Widget *selected;
    glgui::Font *font;
    bool running;
    double cursor_x;
    double cursor_y;
};

int main (int argc, char *argv[])
{
    if (!glfwInit ()) {
        std::cerr << "Cannot initialize glfw." << std::endl;
        return EXIT_FAILURE;
    }
    glfwSetErrorCallback ([] (int error, const char *desc) {
        std::cerr << "GLFW error: " << desc << std::endl;
    });
    try {
        App app;
        app.run ();
    } catch (std::exception &e) {
        glfwTerminate ();
        std::cerr << "Exception: " << e.what () << std::endl;
        return EXIT_FAILURE;
    }
    glfwTerminate ();
    return EXIT_SUCCESS;
}
