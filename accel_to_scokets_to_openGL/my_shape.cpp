#include "my_shape.h"

// for definition of vec4
#include "my_glm_hpp_include.h"

// for typedefs (GLuint, etc.)
#include <glew-1.10.0\include\GL\glew.h>

// for matrix calculations
#include <glm\glm\gtc\matrix_transform.hpp>

my_shape::my_shape(my_vertex *vert_arr, GLuint num_vertices, GLushort *index_arr, GLuint num_indices) :
   m_vertices_arr(0),
   m_num_vertices(num_vertices),
   m_indices_arr(0),
   m_num_indices(num_indices),
   m_vertex_buffer_ID(0),
   m_index_buffer_ID(0)
{
   // the matrices need to be initialized, but I can't pass in all the values to a
   // constructor, so I'll let them call their default constructors, then assign 
   // their first values here
   //glm::translate(m_translation_matrix, glm::vec3(1.0f, 0.0f, -3.0f));
   m_translation_matrix = glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, -9.0f));
   //glm::rotate(m_rotation_matrix, (1.0f / 6.0f) * 3.14159f, glm::vec3(0.0f, 1.0f, 1.0f));
   m_rotation_matrix = glm::rotate(glm::mat4(), (1.0f / 6.0f) * 3.14159f, glm::vec3(0.0f, 1.0f, 1.0f));


   unsigned int size_bytes_vertices = num_vertices * sizeof(my_vertex);
   unsigned int size_bytes_indices = num_indices * sizeof(GLushort);

   // cross your fingers and hope that none of these values are broken
   // Note: If they are, then let the programmer eat cake.
   m_vertices_arr = new my_vertex[num_vertices];
   memcpy(m_vertices_arr, vert_arr, size_bytes_vertices);

   m_indices_arr = new GLushort[num_indices];
   memcpy(m_indices_arr, index_arr, size_bytes_indices);

   
   // create some buffer objects for the data and send the data to openGL

   // first the vertex buffer, which includes position and color data
   // Note: State that the data is for "static drawing" because this data will 
   // not be changing.
   glGenBuffers(1, &m_vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);
   glBufferData(
      GL_ARRAY_BUFFER, 
      (GLsizeiptr)size_bytes_vertices, 
      this->m_vertices_arr, 
      GL_STATIC_DRAW);

   // second, the indices
   // Note: State that the data is for "static drawing" because this data will 
   // not be changing.
   glGenBuffers(1, &m_index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_ID);
   glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, 
      (GLsizeiptr)size_bytes_indices, 
      this->m_indices_arr, 
      GL_STATIC_DRAW);

   // thirdly, the transformation matrix
   // Note: State that the data is for "dynamic drawing" because this data will
   // be sent anew on each "draw" call.
   // Also Note: Unlike the vertices and indices, do not send any initial data.
   // Allocate space for it by declaring sizeof(glm::mat4), but do not send a valid
   // pointer to data.
   glGenBuffers(1, &m_transformation_matrix_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, m_transformation_matrix_buffer_ID);
   glBufferData(
      GL_ARRAY_BUFFER, 
      sizeof(glm::mat4), 
      0, 
      GL_DYNAMIC_DRAW);

   // do a little cleanup by unbinding the buffers
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

my_shape::~my_shape()
{
   //??why is it coming in here again??

   // delete the dynamically allocated memory for the vertices and indices
   delete[] m_vertices_arr;
   delete[] m_indices_arr;

   // delete any buffers that may have been generated
   // Note: According to this documentation, the deletion will swallow 0s, so
   // you don't need to check for unallocated handles.
   glDeleteBuffers(1, &m_vertex_buffer_ID);
   glDeleteBuffers(1, &m_index_buffer_ID);
   glDeleteBuffers(1, &m_transformation_matrix_buffer_ID);
}


int my_shape::draw_thineself(const glm::mat4 * const projection_matrix_ptr, glm::mat4 *camera_world_to_view_matrix_ptr)
{
   int this_ret_val = 0;

   // check for input shenanigans
   if (0 == projection_matrix_ptr)
   {
      this_ret_val = -1;
   }
   else if (0 == camera_world_to_view_matrix_ptr)
   {
      this_ret_val = -2;
   }

   if (0 == this_ret_val)
   {
      // start binding and drawing

      // first the vertex buffer, which includes position and color data
      glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);
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
      // Note: State that the data is for "static drawing" because this data will 
      // not be changing.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_ID);

      // thirdly, the transformation matrix
      // Note: Unlike the vertices and indices, this data will be changing on each
      // draw call, so we need to call glBufferData(...) on every draw call.
      // Also Note: Stride between draw instances = one full matrix, 
      // offset between vertex draws = size of 1 row * row number
      glm::mat4 full_transform_matrix =
         (*projection_matrix_ptr) *
         (*camera_world_to_view_matrix_ptr) *
         m_translation_matrix *
         m_rotation_matrix;

      glBindBuffer(GL_ARRAY_BUFFER, m_transformation_matrix_buffer_ID);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &full_transform_matrix, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(2);
      glEnableVertexAttribArray(3);
      glEnableVertexAttribArray(4);
      glEnableVertexAttribArray(5);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * 0));
      glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * 1));
      glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * 2));
      glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * 3));

      // we need to use this same matrix across all indices
      // Note: I don't entirely understand this, but I do know that the draws won't
      // work (that is, the shape doesn't show up) if we don't specify this.
      glVertexAttribDivisor(2, 1);
      glVertexAttribDivisor(3, 1);
      glVertexAttribDivisor(4, 1);
      glVertexAttribDivisor(5, 1);


      // only draw one instance of the shape
      glDrawElementsInstanced(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_SHORT, 0, 1);

      // clean up the bindings
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }

   return this_ret_val;
}

int my_shape::draw_thineself()
{
   int this_ret_val = 0;

   if (0 == this_ret_val)
   {
      // start binding and drawing

      // first the vertex buffer, which includes position and color data
      glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);
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
      // Note: State that the data is for "static drawing" because this data will 
      // not be changing.
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_ID);

      // only draw one instance
      glDrawElementsInstanced(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_SHORT, 0, 1);

      // clean up the bindings
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }

   return this_ret_val;
}

