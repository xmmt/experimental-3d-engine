#include <Config.h>
#include <RenderModule.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <list>

namespace Engine {

struct RenderData {
    int width{ 0 };
    int height{ 0 };
};

class CoreWindow final {
public:
    CoreWindow() = default;
    ~CoreWindow() = default;
    CoreWindow& setOpenGLVersion(Utils::OpenGLVersion oglVersion) noexcept;
    CoreWindow& setWindowResolution(Utils::WindowResolution windowsResolution) noexcept;
    CoreWindow& setWindowTitle(std::string_view windowTitle) noexcept;
    CoreWindow& addRenderModule(RenderModule<RenderData>&& renderModule);
    int run();

private:
    static void errorCallback_(int error, const char* description);

private:
    std::string glslVersionToStr_(int glslVersion) const;
    bool init_();
    void runCycle_();

private:
    bool initSuccess_;
    Utils::OpenGLVersion oglVersion_{ Config::defaultOpenGLVersion };
    Utils::WindowResolution windowResolution_{ Config::defaultWindowResolution };
    std::string windowTitle_{ Config::defaultWindowTitle };
    GLFWwindow* glfwWindow_{ nullptr };
    std::string glslVersionS_{ glslVersionToStr_(Config::defaultOpenGLVersion.shaderVersion()) };
    std::list<RenderModule<RenderData>> modules_;
};

} // namespace Engine