#include <Engine.h>

int main(void) {
    return Engine::CoreWindow()
      .setOpenGLVersion({ 4, 6, 460 })
      .setWindowResolution({ 1600, 900 })
      .setWindowTitle("Simple example")
      .addRenderModule({ [] {
          GLfloat vertices[] = {
              -0.5f, -0.5f, 0.0f,
              0.5f, -0.5f, 0.0f,
              0.0f, 0.5f, 0.0f
          };
          GLuint VBO;
          glGenBuffers(1, &VBO);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
          auto vertexShader = make_shared<Engine::Shader<Engine::ShaderType::VertexShader>>(ShaderSources::vertexShaderSource);
          auto fragmentShader = make_shared<Engine::Shader<Engine::ShaderType::FragmentShader>>(ShaderSources::fragmentShaderSource);
          const GLuint shaderProgram = glCreateProgram();
          glAttachShader(shaderProgram, vertexShader->get());
          glAttachShader(shaderProgram, fragmentShader->get());
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

          return [=](int width, int height) {
              auto ratio = static_cast<float>(width) / static_cast<float>(height);

              glViewport(0, 0, width, height);
              glClearColor(0.5f, 0.f, 0.f, 1.f);
              glClear(GL_COLOR_BUFFER_BIT);

              glUseProgram(shaderProgram);
              glBindVertexArray(VAO);
              glDrawArrays(GL_TRIANGLES, 0, 3);
              glBindVertexArray(0);
          };
      } })
      .addRenderModule({ [] {
          bool show_demo_window = true;
          bool show_another_window = false;
          ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

          return [=]([[maybe_unused]] int width, [[maybe_unused]] int height) mutable {
              if (show_demo_window) {
                  ImGui::ShowDemoWindow(&show_demo_window);
              }

              {
                  static float f = 0.0f;
                  static int counter = 0;

                  ImGui::Begin("Hello, world!");

                  ImGui::Text("This is some useful text.");
                  ImGui::Checkbox("Demo Window", &show_demo_window);
                  ImGui::Checkbox("Another Window", &show_another_window);

                  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                  ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clear_color));

                  if (ImGui::Button("Button"))
                      counter++;
                  ImGui::SameLine();
                  ImGui::Text("counter = %d", counter);

                  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                  ImGui::End();
              }

              if (show_another_window) {
                  ImGui::Begin("Another Window", &show_another_window);
                  ImGui::Text("Hello from another window!");
                  if (ImGui::Button("Close Me"))
                      show_another_window = false;
                  ImGui::End();
              }
          };
      } })
      .run();
}
