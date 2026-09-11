#include "opengl.h"
#include "window.h"
#include "cursor.h"
#include "config.h"
#include <stb_image.h>
#include <stdexcept>

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* user_param);

namespace OpenGL {
  GLFWwindow* init(){
    //Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Set glfw version to 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Set profile to core, to get a smaller set of functions
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
  #ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  #endif

    //Create application window
    GLFWwindow* window = glfwCreateWindow(WINDOW_RESOLUTION, window_name, NULL, NULL);
    if(window == nullptr) throw std::runtime_error("Failed to create GLFW window");

    //Set the window as the main context
    glfwMakeContextCurrent(window);
    //Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Initialize glad before calling glfw functions
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("Failed to initialize GLAD");

  #ifndef NDEBUG
    //Enable debug
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if(flags & GL_CONTEXT_FLAG_DEBUG_BIT){
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
  #endif

    //Set window offset and resolution
    glViewport(0, 0, WINDOW_RESOLUTION);

    //Set input callbacks
    glfwSetFramebufferSizeCallback(window, Window::resizeCallback);
    glfwSetCursorPosCallback(window, Cursor::moveCallback);

    //Set the screen clear color
    glClearColor(BACKGROUND_COLOR, 1.0f);

    //Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //Enable antialiasing
    glEnable(GL_MULTISAMPLE);

    //Load textures upside down
    stbi_set_flip_vertically_on_load(true);

    return window;
  }

  void exit(){
    glfwTerminate();
  }
}
