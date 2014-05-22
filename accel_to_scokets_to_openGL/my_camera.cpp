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
   m_view_direction(0.0f, 0.0f, -1.0f),
   m_strafe_direction(0.0f, 0.0f, 0.0f),
   m_prev_mouse_position(0.0f, 0.0f),
   m_camera_move_speed(0.3f)
{
   // default position of the camera is the world origin

   // the up vector defaults to the positive Y direction

   // default view direction is negative Z (this must be a unit vector)

   // default strafe direction is ??

   // default "previous mouse position" will be the world origin

   // default camera move speed is "moderate"
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


void my_camera::mouse_update(const glm::vec2& new_mouse_position)
{
   glm::vec2 mouse_delta = new_mouse_position - m_prev_mouse_position;

   // attempt to prevent "camera jump" when the mouse leaves the window by only 
   // updating it's view direction if the delta is under a limit
   if (glm::length(mouse_delta) < 50.0f)
   {
      const float ROTATION_SENSITIVITY = 0.5f;
      float rotate_angle_rad_x = -mouse_delta.x * (2.0f * 3.14159f) / 360.0f;
      float rotate_angle_rad_y = -mouse_delta.y * (2.0f * 3.14159f) / 360.0f;

      m_strafe_direction = glm::cross(m_view_direction, m_world_up_vector);
      glm::mat4 rotator_matrix =
         glm::rotate(rotate_angle_rad_x * ROTATION_SENSITIVITY, m_world_up_vector) *
         glm::rotate(rotate_angle_rad_y * ROTATION_SENSITIVITY, m_strafe_direction);

      m_view_direction = glm::mat3(rotator_matrix) * m_view_direction;

      cout << "camera view direction: x=" << m_view_direction.x <<
         ", y=" << m_view_direction.y << ", z=" << m_view_direction.z << endl;
   }

   m_prev_mouse_position = new_mouse_position;
}

void my_camera::move_forward()
{
   m_position += m_camera_move_speed * m_view_direction;
   cout << "camera position: x=" << m_position.x <<
      ", y=" << m_position.y << ", z=" << m_position.z << endl;
}

void my_camera::move_back()
{
   m_position -= m_camera_move_speed * m_view_direction;
   cout << "camera position: x=" << m_position.x <<
      ", y=" << m_position.y << ", z=" << m_position.z << endl;
}

void my_camera::strafe_left()
{
   m_position -= m_camera_move_speed * m_strafe_direction;
   cout << "camera position: x=" << m_position.x <<
      ", y=" << m_position.y << ", z=" << m_position.z << endl;
}

void my_camera::strafe_right()
{
   m_position += m_camera_move_speed * m_strafe_direction;
   cout << "camera position: x=" << m_position.x <<
      ", y=" << m_position.y << ", z=" << m_position.z << endl;
}

void my_camera::move_up()
{
   m_position += m_camera_move_speed * m_world_up_vector;
   cout << "camera position: x=" << m_position.x <<
      ", y=" << m_position.y << ", z=" << m_position.z << endl;
}

void my_camera::move_down()
{
   m_position -= m_camera_move_speed * m_world_up_vector;
   cout << "camera position: x=" << m_position.x <<
      ", y=" << m_position.y << ", z=" << m_position.z << endl;
}



