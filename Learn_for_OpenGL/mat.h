#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Define.h"

namespace SpaceMat
{
	extern glm::mat4 unitMat;
	extern glm::mat4 model;
	extern glm::mat4 view;
	extern glm::mat4 projection;
	extern float i;
	extern float speed;
	extern float last;
	void initSpace();
}