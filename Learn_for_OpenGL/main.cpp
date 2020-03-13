#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mat.h"
#include "Define.h"

//窗口回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//键是否按下的小函数
bool KeyPress(GLFWwindow* window,int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

GLFWwindow* init();
void createTexture(unsigned int& texture, unsigned int ID, std::string texturePath, GLenum target, GLenum pname, GLint param, GLint internalformat);

int main()
{
    GLFWwindow* window = init();

    //顶点数组
    float BoxBlockLength = 80 * SRC_ITEM_LENGTH;
    float vertices[] = {
//            x                y                 z            r      g      b      a     tx     ty
        BoxBlockLength,  BoxBlockLength,   BoxBlockLength,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f, //+++
        BoxBlockLength, -BoxBlockLength,   BoxBlockLength,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f, //+-+
       -BoxBlockLength, -BoxBlockLength,   BoxBlockLength,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f, //--+
       -BoxBlockLength,  BoxBlockLength,   BoxBlockLength,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, //-++
        BoxBlockLength,  BoxBlockLength,  -BoxBlockLength,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f, //++-
        BoxBlockLength, -BoxBlockLength,  -BoxBlockLength,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, //+--
       -BoxBlockLength, -BoxBlockLength,  -BoxBlockLength,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, //---
       -BoxBlockLength,  BoxBlockLength,  -BoxBlockLength,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, //-+-
       -BoxBlockLength, -BoxBlockLength,  -BoxBlockLength,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, //---
        BoxBlockLength, -BoxBlockLength,  -BoxBlockLength,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //+--
        BoxBlockLength,  BoxBlockLength,   BoxBlockLength,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, //+++
       -BoxBlockLength,  BoxBlockLength,   BoxBlockLength,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, //-++
       -BoxBlockLength,  BoxBlockLength,   BoxBlockLength,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f, //-++
       -BoxBlockLength,  BoxBlockLength,  -BoxBlockLength,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f, //-+-
        BoxBlockLength, -BoxBlockLength,  -BoxBlockLength,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //+--
        BoxBlockLength, -BoxBlockLength,   BoxBlockLength,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, //+-+
    };

    unsigned int indices[] = {
         8,  9,  4,  4,  7,  8,
         2,  1, 10, 10, 11,  2,
        12, 13,  6,  6,  2, 12,
         0,  4, 14, 14, 15,  0,
         6,  5,  1,  1,  2,  6,
         7,  4,  0,  0,  3,  7
    };

    //VBO&VAO&EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //着色器
    Shader learn("learn.glvs", "learn.glfs");

    //初始化贴图
    unsigned int texture;
    unsigned int texture2;
    stbi_set_flip_vertically_on_load(true);
    createTexture(texture, 1, "./texture/container.jpg", GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR, GL_RGB);
    createTexture(texture2, 1, "./texture/smile.png", GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR, GL_RGBA);

    //设置贴图uniform
    learn.use();
    learn.setInt("texture1", 0);
    learn.setInt("texture2", 1);

    glEnable(GL_DEPTH_TEST);

    //渲染循环 
    while (!glfwWindowShouldClose(window))
    {
        SpaceMat::initSpace();
        //针对按键做出改动
        if (KeyPress(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        if (KeyPress(window, GLFW_KEY_E))
            glClearColor(0.8f, 0.4f, 0.8f, 1.0f);

        if (KeyPress(window, GLFW_KEY_A) || KeyPress(window, GLFW_KEY_LEFT))
        {
            SpaceMat::i -= 0.001f;
            SpaceMat::last = SpaceMat::speed;
        }

        if (KeyPress(window, GLFW_KEY_D) || KeyPress(window, GLFW_KEY_RIGHT))
        {
            SpaceMat::i += 0.001f;
            SpaceMat::last = SpaceMat::speed;
        }

        if (KeyPress(window, GLFW_KEY_W) || KeyPress(window, GLFW_KEY_UP))
            SpaceMat::speed += 0.001f;

        if (KeyPress(window, GLFW_KEY_S) || KeyPress(window, GLFW_KEY_DOWN))
        {
            if (SpaceMat::speed >= 0)
            {
                SpaceMat::speed -= 0.001f;
            }
        }
         

        //改变颜色
        int times = glfwGetTime();
        
        /*vertices[3] = vertices[4] = vertices[5] = sin(glfwGetTime()) / 2.0f + 0.5f;
        vertices[13] = vertices[14] = sin(glfwGetTime()+3.0f) / 2.0f + 0.5f;
        vertices[21] = vertices[13] = sin(glfwGetTime() + 3.0f) / 2.0f + 0.5f;*/

        
        //显示背景颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //绑定贴图
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);  

        //更新状态
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        

        learn.use();
        glUniformMatrix4fv(glGetUniformLocation(learn.ID, "model"), 1, GL_FALSE, glm::value_ptr(SpaceMat::model));
        glUniformMatrix4fv(glGetUniformLocation(learn.ID, "view"), 1, GL_FALSE, glm::value_ptr(SpaceMat::view));
        glUniformMatrix4fv(glGetUniformLocation(learn.ID, "projection"), 1, GL_FALSE, glm::value_ptr(SpaceMat::projection));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void createTexture(unsigned int& texture, unsigned int ID,std::string texturePath, GLenum target, GLenum pname, GLint param, GLint internalformat)
{
    glGenTextures(ID, &texture);
    glBindTexture(target, texture);
    glTexParameteri(target, pname, param);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(target, 0, internalformat, width, height, 0, internalformat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(target);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

GLFWwindow* init()
{
    //初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(SRC_WINDOW_WIDTH, SRC_WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(-200, -200, width, height);
}