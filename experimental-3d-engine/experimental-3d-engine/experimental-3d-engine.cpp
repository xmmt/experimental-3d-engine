#define GLAD_GL_IMPLEMENTATION
#define GLAD_WGL_IMPLEMENTATION
#include <glad/wgl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <thread>

static const char* vertexShaderSource = R"shader(
#version 460 core

layout(location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
)shader";

static const char* fragmentShaderSource = R"shader(
#version 460 core

out vec4 color;

void main() {
	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)shader";

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    std::thread thr([&] {
        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);
        gladLoadWGL(reinterpret_cast<HDC>(window), glfwGetProcAddress);
        glfwSwapInterval(1);

        GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        const GLuint shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
        glEnableVertexAttribArray(0);

        GLuint VAO;
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        while (!glfwWindowShouldClose(window)) {
            int width, height;

            glfwGetFramebufferSize(window, &width, &height);
            float ratio = width / static_cast<float>(height);

            glViewport(0, 0, width, height);
            glClearColor(0.5f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

            glfwSwapBuffers(window);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
