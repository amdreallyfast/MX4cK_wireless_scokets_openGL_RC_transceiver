#include "my_shape.h"

// for definition of vec4
#include "glm\glm\glm.hpp"

// for typedefs (GLuint, etc.)
#include <glew-1.10.0\include\GL\glew.h>


my_shape::my_shape() :
   m_vertices_arr(0),
   m_num_vertices(0),
   m_indices_arr(0),
   m_num_indices(0),
   m_vertex_buffer_ID(0),
   m_index_buffer_ID(0)
{
}

my_shape::~my_shape()
{
   // delete the dynamically allocated memory for the vertices and indices
   delete[] m_vertices_arr;
   delete[] m_indices_arr;

   // delete any buffers that may have been generated
   // Note: According to this documentation, the deletion will swallow 0s, so
   // you don't need to check for unallocated handles.
   glDeleteBuffers(1, &m_vertex_buffer_ID);
   glDeleteBuffers(1, &m_index_buffer_ID);
}

GLsizeiptr my_shape::vertex_buffer_size() const
{
   return (m_num_vertices * my_vertex::m_size_bytes_per_vertex);
}

GLsizeiptr my_shape::index_buffer_size() const
{
   return (m_num_indices * my_vertex::m_size_bytes_per_vertex);
}

int my_shape::draw_thineself(void(*gl_context_drawing_function_ptr)(void))
{
   int this_ret_val = 0;

   // check for input shenanigans and other problems
   if (0 == gl_context_drawing_function_ptr)
   {
      this_ret_val = -1;
   }
   else if (0 == m_vertices_arr)
   {
      this_ret_val = -2;
   }
   else if (0 == m_num_indices)
   {
      this_ret_val = -3;
   }

   if (0 == this_ret_val)
   {
      // start binding and drawing

      // first the vertex buffer,which includes position and color data
      glGenBuffers(1, &m_vertex_buffer_ID);
      glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);
      glBufferData(GL_ARRAY_BUFFER, this->vertex_buffer_size(), this->m_vertices_arr, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0, 
         my_vertex::m_num_position_entries_per_vertex,
         GL_FLOAT,
         GL_FALSE,
         my_vertex::m_size_bytes_per_vertex,
         0
         );

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(
         1,
         my_vertex::m_num_color_entries_per_vertex,
         GL_FLOAT,
         GL_FALSE,
         my_vertex::m_size_bytes_per_vertex,
         (char *)(my_vertex::m_size_bytes_per_position_vertex)
         );

      // second, the indices
      glGenBuffers(1, &m_index_buffer_ID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_ID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_size(), this->m_indices_arr, GL_STATIC_DRAW);

      // only draw one instance of the cube
      glDrawElementsInstanced(GL_TRIANGLES, this->m_num_indices, GL_UNSIGNED_SHORT, 0, 1);
   }

   return this_ret_val;
}

