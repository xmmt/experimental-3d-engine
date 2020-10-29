#include <CoreWindow.h>

#include <glad/gl.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <string>

using namespace std;
using namespace Engine;

CoreWindow& CoreWindow::setOpenGLVersion(Utils::OpenGLVersion oglVersion) noexcept {
    oglVersion_ = oglVersion;
    glslVersionS_ = glslVersionToStr_(oglVersion.shaderVersion());
    return *this;
}

CoreWindow& CoreWindow::setWindowResolution(Utils::WindowResolution windowResolution) noexcept {
    windowResolution_ = windowResolution;
    return *this;
}

CoreWindow& CoreWindow::setWindowTitle(string_view windowTitle) noexcept {
    windowTitle_ = windowTitle;
    return *this;
}

CoreWindow& CoreWindow::addRenderModule(RenderModule<RenderData>&& renderModule) {
    modules_.emplace_back(std::move(renderModule));
    return *this;
}

int CoreWindow::run() {
    if (glfwInit() != GLFW_TRUE) {
        return EXIT_FAILURE;
    }
    if (init_() == false) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    runCycle_();
    glfwDestroyWindow(glfwWindow_);
    glfwTerminate();
    return EXIT_SUCCESS;
}

std::string CoreWindow::glslVersionToStr_(int glslVersion) const {
    return "#version " + to_string(glslVersion);
}


bool CoreWindow::init_() {
    glfwSetErrorCallback(errorCallback_);
    glfwWindow_ = glfwCreateWindow(windowResolution_.width(), windowResolution_.height(), windowTitle_.data(), nullptr, nullptr);
    if (glfwWindow_ == nullptr) {
        return false;
    }
    glfwMakeContextCurrent(glfwWindow_);
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        return false;
    }
    //gladLoadWGL(reinterpret_cast<HDC>(glfwWindow_), glfwGetProcAddress);
    glfwSwapInterval(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow_, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::GetIO().Fonts->AddFontFromFileTTF(Engine::Config::droidSansPath.data(), 16.0f);

    RenderData data;
    glfwGetFramebufferSize(glfwWindow_, &data.width, &data.height);
    for (auto&& m : modules_) {
        m.init(data);
    }

    return true;
}

void CoreWindow::runCycle_() {
    while (!glfwWindowShouldClose(glfwWindow_)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderData data;
        glfwGetFramebufferSize(glfwWindow_, &data.width, &data.height);
        for (auto&& m : modules_) {
            m.draw(data);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(glfwWindow_);
        glfwPollEvents();
    }
}

void CoreWindow::errorCallback_(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GLFW_TRUE);
//}