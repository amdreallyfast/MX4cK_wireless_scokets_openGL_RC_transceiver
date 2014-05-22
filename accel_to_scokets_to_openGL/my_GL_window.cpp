#include "my_GL_window.h"

#include "my_shape.h"
#include "shape_generator.h"
#include "my_shader_handler.h"

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

// for vector and matrix types
#include "my_glm_hpp_include.h"

// for matrix transformation stuff
#include <glm\glm\gtc\matrix_transform.hpp>


my_GL_window::~my_GL_window()
{
   // destroy all shapes that we kept around
   m_shape_ptrs_vector_it = m_shape_ptrs_vector.begin();
   while (m_shape_ptrs_vector_it != m_shape_ptrs_vector.end())
   {
      delete (*m_shape_ptrs_vector_it);
   }
}


void my_GL_window::initializeGL()
{
   int ret_val = 0;
   my_shape *new_shape_ptr = 0;

   // sets up all the openGL pointers
   glewInit();
   glEnable(GL_DEPTH_TEST);

   // make a new shape, which will automatically have it's data sent to openGL
   // during the initialization process
   ret_val = shape_generator::make_cube(new_shape_ptr);
   if (ret_val < 0)
   {
      // problem (??do something??)
   }
   else
   {
      m_shape_ptrs_vector.push_back(new_shape_ptr);
   }

   my_shader_handler& shader_thingy = my_shader_handler::get_instance();
   ret_val = shader_thingy.install_shaders();
   if (ret_val < 0)
   {
      // something didn't compile or link correctly (??do something??)
      cout << "something bad happened during shader initialization" << endl;
   }
}

void my_GL_window::paintGL()
{

}

void my_GL_window::render_frame()
{
   // field of view is 90 degrees
   float fov_radians = (1.0f / 2.0f) * 3.14159f;

   // aspect ratio is determined by window dimensions 
   // Note: The return values of width() and height() are integers, so they need to
   // be cast to floats so that there are no implicit conversions that result in 
   // lost data, such as an integer dividing by an integer, resulting in an integer,
   // which is then shoved into the float "aspect ratio".  That would be bad.
   float aspect_ratio = (float)this->width() / (float)this->height();

   // other things
   float near_plan_dist = 0.1f;
   float far_plane_dist = 10.0f;

   // create the projection matrix
   glm::mat4 projection_matrix = glm::perspective(
      fov_radians,
      aspect_ratio,
      near_plan_dist,
      far_plane_dist);

   glm::mat4 camera_world_to_view_matrix = m_camera.get_world_to_view_matrix();

   m_shape_ptrs_vector_it = m_shape_ptrs_vector.begin();
   while (m_shape_ptrs_vector_it != m_shape_ptrs_vector.end())
   {
      (*m_shape_ptrs_vector_it)->draw_thineself(
         &projection_matrix,
         &camera_world_to_view_matrix);
      m_shape_ptrs_vector_it += 1;
   }
}
