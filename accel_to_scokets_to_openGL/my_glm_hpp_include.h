#pragma once

// the entire purpose of this file is to provide a common way of including glm's
// glm.hpp with or without GLM_FORCE_RADIANS defined so that I don't have to define
// it in every single source file
#define GLM_FORCE_RADIANS
#include <glm\glm\glm.hpp>