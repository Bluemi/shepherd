#ifndef __CLASS_UTIL__
#define __CLASS_UTIL__

#include <ostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

std::ostream& operator<<(std::ostream& stream, const glm::vec3& v);

glm::mat4 basis_perspective(float fovy, float aspect_ratio, float near, float far, const glm::vec3& basis);

#endif
