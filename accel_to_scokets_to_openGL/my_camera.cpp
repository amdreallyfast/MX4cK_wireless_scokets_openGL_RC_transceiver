#include "my_camera.h"

// for vector and matrix types
#include "my_glm_hpp_include.h"

// for matrix computations
#include <glm/glm/gtx/transform.hpp>

#include <iostream>
using std::cout;
using std::endl;


my_camera::my_camera() :
   m_position(0.0f, 0.0f, 0.0f),
   m_world_up_vector(0.0f, 1.0f, 0.0f),
   m_view_direction(0.0f, 0.0f, -1.0f)
{
   // default position of the camera is the world origin

   // the up vector defaults to the positive Y direction

   // default view direction is negative Z (this must be a unit vector)
}

glm::mat4 my_camera::get_world_to_view_matrix() const
{
   return glm::lookAt(
      m_position, 
      m_position + m_view_direction, 
      m_world_up_vector);
}

int my_camera::set_world_up_vector(glm::vec3 new_world_up_vector)
{
   int this_ret_val = 0;

   // ??anything to check??

   m_world_up_vector = new_world_up_vector;


   return this_ret_val;
}

