#pragma once

// for vector and matrix types
#include "my_glm_hpp_include.h"

class my_camera
{
public:
   my_camera();
   glm::mat4 get_world_to_view_matrix() const;

   int set_world_up_vector(glm::vec3 new_world_up_vector);

private:
   glm::vec3 m_world_up_vector;
   glm::vec3 m_position;
   glm::vec3 m_view_direction;
};
