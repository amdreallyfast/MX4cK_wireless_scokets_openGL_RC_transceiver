#pragma once

// for definition of my vertex structure
#include "my_vertex.h"

// for typedefs (GLuint, etc.)
#include <glew-1.10.0\include\GL\glew.h>

class my_shape
{
public:
   my_shape(my_vertex *vert_arr, GLuint num_vertices, GLushort *index_arr, GLuint num_indices);
   ~my_shape();

   // send in various data necessary for this object to compute how it shall draw itself
   int draw_thineself(glm::mat4 *projection_matrix_ptr, glm::mat4 *camera_world_to_view_matrix_ptr);

private:
   // make some default constructors explicitly private to prevent improper initialization
   my_shape();
   my_shape(const my_shape&);

   my_vertex *m_vertices_arr;
   GLuint m_num_vertices;
   GLushort *m_indices_arr;
   GLuint m_num_indices;

   GLuint m_vertex_buffer_ID;
   GLuint m_index_buffer_ID;
   GLuint m_transformation_matrix_buffer_ID;

   glm::mat4 m_translation_matrix;
   glm::mat4 m_rotation_matrix;
};
