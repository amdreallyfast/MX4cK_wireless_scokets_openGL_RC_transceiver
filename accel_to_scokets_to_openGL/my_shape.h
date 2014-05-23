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
   int draw_thineself();
   int draw_thineself(const glm::mat4 * const projection_matrix_ptr, glm::mat4 *camera_world_to_view_matrix_ptr);

   int rotate_thineself(float rotate_angle_radians, glm::vec3 rotation_axis);

   int point_thineself_mouse_update(const glm::vec2& new_mouse_position, int window_width, int window_height);

   int reset_thineself();

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

   glm::vec3 m_world_up_vector;
   glm::vec2 m_prev_mouse_position;
};
