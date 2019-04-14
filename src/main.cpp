#include "graphics.hpp"

// Settings
const unsigned int SCR_WIDTH = 1408; // 800 (LEGACY)
const unsigned int SCR_HEIGHT = 792; // 600 (LEGACY)
const float ASPECT = (float)SCR_WIDTH / (float)SCR_HEIGHT;

int main()
{
    Display display(SCR_WIDTH, SCR_HEIGHT, "OpenGL Test Window", 3);
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    display.setCamera(&camera);

    Shader lightingShader("lightingShader");

    Model nanosuit("res/models/nanosuit/nanosuit.obj");

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // After Setup
    lightingShader.use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Setting up Vertex Shader Uniforms
    lightingShader.setMat4("model", model);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    // Setting up Fragment Shader Uniforms
    lightingShader.setVec3("viewPos", camera.position);

    // Light Parameters
    // Directional Light
    lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);

    // Light Intensity
    lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    // Point Light
    lightingShader.setVec3( "pointLights[0].position", lightPos);

    lightingShader.setVec3( "pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3( "pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3( "pointLights[0].specular", 1.0f, 1.0f, 1.0f);

    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.09f);
    lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

    // Spot Light
    lightingShader.setVec3("spotLight.position", camera.position);
    lightingShader.setVec3("spotLight.direction", camera.getFront());

    lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

    lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

    lightingShader.setFloat("spotLight.constant", 1.0f);
    lightingShader.setFloat("spotLight.linear", 0.09f);
    lightingShader.setFloat("spotLight.quadratic", 0.032f);

    lightingShader.setFloat("material.shininess", 64);

    // glPolygonMode(GL_FRONT, GL_LINE);
    while(display.active())
    {
        // Processing Input
        display.processInput();

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering Process
        // -----------------
        // Camera Processing
        view = camera.getViewMatrix();
        projection = camera.getProjectionMatrix(ASPECT);
        lightPos.y += Time::getDeltaTime() * 10.0f;

        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        lightingShader.setVec3("viewPos", camera.position);
        lightingShader.setVec3("pointLights[0].position", lightPos);
        lightingShader.setVec3("spotLight.position", camera.position);
        lightingShader.setVec3("spotLight.direction", camera.getFront());

        nanosuit.draw(lightingShader);

        // ------------------

        display.swapBuffers();
        display.pollEvents();
    }

    glfwTerminate();
    return 0;
}
