

#include "glm/glm.hpp"

namespace Core
{
	glm::mat4 createPerspectiveMatrix(float zNear = 0.1f, float zFar = 500.0f);

	glm::mat4 createViewMatrix(glm::vec3 position, glm::vec3 forward, glm::vec3 up);
}
