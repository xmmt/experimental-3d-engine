#include <Engine.h>

#include <iostream>

int main(void) {
    return Engine::CoreWindow()
      .setOpenGLVersion({ 4, 6, 460 })
      .setWindowResolution({ 1600, 900 })
      .setWindowTitle("Simple example")
      .addRenderModule({ []([[maybe_unused]] auto const& data) mutable {
          GLfloat vertices[] = {
              -0.5f, -0.5f, 0.0f,
              0.5f, -0.5f, 0.0f,
              0.0f, 0.5f, 0.0f
          };
          GLuint VBO;
          glGenBuffers(1, &VBO);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
          auto shaderProgram = Engine::makeShaderProgram()
                                 .addVertexShader(ShaderSources::vertexShaderSource)
                                 .addFragmentShader(ShaderSources::fragmentShaderSource)
                                 .link();
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

          return [VAO, shaderProgram{ std::move(shaderProgram) }](auto const& data) {
              auto ratio = static_cast<float>(data.width) / static_cast<float>(data.height);

              glViewport(0, 0, data.width, data.height);
              glClearColor(0.5f, 0.f, 0.f, 1.f);
              glClear(GL_COLOR_BUFFER_BIT);
              shaderProgram.use();
              glBindVertexArray(VAO);
              glDrawArrays(GL_TRIANGLES, 0, 3);
              glBindVertexArray(0);
          };
      } })
      .addRenderModule({ []([[maybe_unused]] auto const& data) mutable {
          return
            [show_demo_window = true,
              show_another_window = false,
              clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f)](
              [[maybe_unused]] auto const& data) mutable {
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
      .addRenderModule({ [](auto const& data) mutable {
          struct State {
              std::string shaderInputStr{ ShaderSources::textureFragmentShaderSource };
              Engine::Program<Engine::ProgramState::ReadyToUse> program{
                  Engine::makeShaderProgram()
                    .addVertexShader(ShaderSources::textureVertexShaderSource)
                    .addFragmentShader(shaderInputStr)
                    .link()
              };
              GLuint framebufferName = 0;
              GLuint renderedTexture = 0;
              GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

              /*GLfloat vertices[9] = {
                  -0.5f, -0.5f, 0.0f,
                  0.5f, -0.5f, 0.0f,
                  0.0f, 0.5f, 0.0f
              };*/
              GLfloat vertices[12] = {
                  -0.75f, -0.75f, 0.0f,
                  0.75f, -0.75f, 0.0f,
                  0.75f, 0.75f, 0.0f,
                  -0.75f, 0.75f, 0.0f
              };
              GLuint VBO = 0;
              GLuint VAO = 0;
          } state;
          glGenFramebuffers(1, &state.framebufferName);
          glBindFramebuffer(GL_FRAMEBUFFER, state.framebufferName);
          glGenTextures(1, &state.renderedTexture);
          glBindTexture(GL_TEXTURE_2D, state.renderedTexture);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, state.renderedTexture, 0);
          glDrawBuffers(1, state.drawBuffers);

          glGenBuffers(1, &state.VBO);
          //glBindBuffer(GL_ARRAY_BUFFER, state.VBO);
          //glBufferData(GL_ARRAY_BUFFER, sizeof(state.vertices), state.vertices, GL_STATIC_DRAW);
          //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
          //glEnableVertexAttribArray(0);

          glGenVertexArrays(1, &state.VAO);
          glBindVertexArray(state.VAO);
          glBindBuffer(GL_ARRAY_BUFFER, state.VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(state.vertices), state.vertices, GL_STATIC_DRAW);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
          glEnableVertexAttribArray(0);
          glBindVertexArray(0);

          glBindFramebuffer(GL_FRAMEBUFFER, 0);

          return [state{ std::move(state) }](
                   auto const& data) mutable {
              glBindFramebuffer(GL_FRAMEBUFFER, state.framebufferName);
              glViewport(0, 0, 1024, 1024);

              glClearColor(0.0f, 0.f, 0.6f, 1.f);
              glClear(GL_COLOR_BUFFER_BIT);
              state.program.use();
              glBindVertexArray(state.VAO);
              glDrawArrays(GL_QUADS, 0, 4);
              glBindVertexArray(0);

              glBindFramebuffer(GL_FRAMEBUFFER, 0);
              glViewport(0, 0, data.width, data.height);

              ImGui::InputTextMultiline("##shader_input", &state.shaderInputStr);
              if (ImGui::Button("Compile")) {
                  state.program = Engine::makeShaderProgram()
                                    .addVertexShader(ShaderSources::textureVertexShaderSource)
                                    .addFragmentShader(state.shaderInputStr)
                                    .link();
              }
              ImGui::Image((void*)(intptr_t)state.renderedTexture, ImVec2(512, 512));
          };
      } })
      .run();
}
