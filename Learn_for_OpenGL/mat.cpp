#include "mat.h"

namespace SpaceMat
{
    glm::mat4 unitMat = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    float i = 0.0f;
    float speed = 0.000001f;
    float last = 0.000001f;

    void initSpace()
    {
        model = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)i * glm::radians(last), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SRC_WINDOW_WIDTH / (float)SRC_WINDOW_HEIGHT, 0.1f, 100.0f);
    }
}

