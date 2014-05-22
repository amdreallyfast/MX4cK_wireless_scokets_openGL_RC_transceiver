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
   ret_val = shape_generator::make_cube(&new_shape_ptr);
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
   render_frame();
}

void my_GL_window::render_frame()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0, 0, this->width(), this->height());

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

   //m_shape_ptrs_vector_it = m_shape_ptrs_vector.begin();
   //while (m_shape_ptrs_vector_it != m_shape_ptrs_vector.end())
   //{
   //   (*m_shape_ptrs_vector_it)->draw_thineself(
   //      &projection_matrix,
   //      &camera_world_to_view_matrix);
   //   m_shape_ptrs_vector_it += 1;
   //}

   const float RED_TRIANGLE_Z = +0.5f;
   const float BLUE_TRIANGLE_Z = -0.5f;
   my_vertex verts[] =
   {
      // first triangle
      glm::vec4(-1.0f, -1.0f, RED_TRIANGLE_Z, 1.0f),    // left bottom corner
      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),          // all red
      glm::vec4(+1.0f, -1.0f, RED_TRIANGLE_Z, 1.0f),    // right bottom corner
      glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),          // red + green (apparently this makes yellow)
      glm::vec4(+0.0f, +1.0f, -1.0f, 1.0f),    // center top
      glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),          // red + blue (apparently this makes pink

      // second triangle
      glm::vec4(-1.0f, +1.0f, BLUE_TRIANGLE_Z, 1.0f),   // left top corner
      glm::vec4(0.0f, 0.0f, 1.0f, 0.1f),          // all blue
      glm::vec4(+1.0f, +1.0f, BLUE_TRIANGLE_Z, 1.0f),   // right top corner
      glm::vec4(1.0f, 0.0f, 1.0f, 0.5f),          // blue + red
      glm::vec4(+0.0f, -1.0f, BLUE_TRIANGLE_Z, 1.0f),   // center bottom
      glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),          // blue + green
   };

   GLushort indices[] = { 0, 1, 2, 3, 4, 5 };

   GLint position_bytes_per_vertex = sizeof(glm::vec4) / sizeof(float);
   GLint color_bytes_per_vertex = sizeof(glm::vec4) / sizeof(float);

   GLuint vertex_buffer_ID;;
   glGenBuffers(1, &vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(
      0,
      position_bytes_per_vertex,
      GL_FLOAT,
      GL_FALSE,
      position_bytes_per_vertex + color_bytes_per_vertex,
      0
      );

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(
      1,
      color_bytes_per_vertex,
      GL_FLOAT,
      GL_FALSE,
      position_bytes_per_vertex + color_bytes_per_vertex,
      (char *)(position_bytes_per_vertex)
      );

   GLuint index_buffer_ID;
   glGenBuffers(1, &index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

   glDeleteBuffers(1, &vertex_buffer_ID);
   glDeleteBuffers(1, &index_buffer_ID);
}
