#include "my_shape.h"

// for definition of vec4
#include "my_glm_hpp_include.h"

// for typedefs (GLuint, etc.)
#include <glew-1.10.0\include\GL\glew.h>

// for matrix calculations
#include <glm\glm\gtc\matrix_transform.hpp>

#include <glm\glm\trigonometric.hpp>

my_shape::my_shape(my_vertex *vert_arr, GLuint num_vertices, GLushort *index_arr, GLuint num_indices) :
   m_vertices_arr(0),
   m_num_vertices(num_vertices),
   m_indices_arr(0),
   m_num_indices(num_indices),
   m_vertex_buffer_ID(0),
   m_index_buffer_ID(0),
   m_world_up_vector(0.0f, 1.0f, 0.0f)
{

   // the matrices need to be initialized, but I can't pass in all the values to a
   // constructor, so I'll let them call their default constructors, then assign 
   // their first values here
   m_translation_matrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -4.0f));

   //glm::rotate(m_rotation_matrix, (1.0f / 6.0f) * 3.14159f, glm::vec3(0.0f, 1.0f, 1.0f));
   //m_rotation_matrix = glm::rotate(glm::mat4(), (1.0f / 6.0f) * 3.14159f, glm::vec3(0.0f, 1.0f, 1.0f));
   //m_rotation_matrix = glm::rotate(glm::mat4(), 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));


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

      glm::vec4 top_right_vertex = m_vertices_arr[0].position;
      top_right_vertex = m_rotation_matrix * top_right_vertex;

      glm::vec4 bottom_right_vertex = m_vertices_arr[1].position;
      glm::vec4 bottom_left_vertex = m_vertices_arr[2].position;
      glm::vec4 top_left_vertex = m_vertices_arr[3].position;

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


int my_shape::rotate_thineself(float rotate_angle_radians, glm::vec3 rotation_axis)
{
   int this_ret_val = 0;

   // ??check for anything??

   // pass in the existing matrix so that it will building off the previous one
   m_rotation_matrix = glm::rotate(m_rotation_matrix, rotate_angle_radians, rotation_axis);

   //float rotation_angle = 0.0f;
   //rotation_angle = glm::atan(1.0f, 1.0f);      // should be 45 degrees, or 1/4*pi, or ~0.785
   //rotation_angle = glm::atan(5.0f, 8.66f);     // should be 30 degrees, or 1/6*pi, or ~0.524
   //rotation_angle = glm::atan(8.66f, 5.0f);     // should be 60 degrees, or 1/3*pi, or ~1.047


   return this_ret_val;
}


int my_shape::point_thineself_mouse_update(const glm::vec2& new_mouse_position, int window_width, int window_height)
{

   //// O is your object's position
   //// P is the position of the object to face
   //// U is the nominal "up" vector (typically Vector3.Y)
   //// Note: this does not work when O is straight below or straight above P
   //Matrix RotateToFace(Vector3 O, Vector3 P, Vector3 U)
   //{
   //   Vector3 D = (O - P);
   //   Vector3 Right = Vector3.Cross(U, D);
   //   Vector3.Normalize(ref Right, out Right);
   //   Vector3 Backwards = Vector3.Cross(Right, U);
   //   Vector3.Normalize(ref Backwards, out Backwards);
   //   Vector3 Up = Vector3.Cross(Backwards, Right);
   //   Matrix rot = new Matrix(Right.X, Right.Y, Right.Z, 0, Up.X, Up.Y, Up.Z, 0, Backwards.X, Backwards.Y, Backwards.Z, 0, 0, 0, 0, 1);
   //   return rot;
   //}

   //glm::vec3 new_mouse_pos_vec3 = glm::vec3(new_mouse_position, 0.0f);
   //glm::vec3 object_position = glm::vec3(m_translation_matrix[3].x, m_translation_matrix[3].y, m_translation_matrix[3].z);
   //glm::vec3 point_vector = object_position - new_mouse_pos_vec3;
   //
   //glm::vec3 orthogonal_rotation_vector = glm::cross(point_vector, m_world_up_vector);
   ////orthogonal_rotation_vector = glm::normalize(orthogonal_rotation_vector);
   //
   //glm::vec3 backwards_vector = glm::cross(orthogonal_rotation_vector, m_world_up_vector);
   ////backwards_vector = glm::normalize(backwards_vector);

   //glm::vec3 new_up_vector = glm::cross(backwards_vector, orthogonal_rotation_vector);

   //glm::mat4 new_rot_mat = glm::mat4(
   //   orthogonal_rotation_vector.x, orthogonal_rotation_vector.y, orthogonal_rotation_vector.z, 0.0f,
   //   new_up_vector.x, new_up_vector.y, new_up_vector.z, 0.0f,
   //   backwards_vector.x, backwards_vector.y, backwards_vector.z, 0.0f,
   //   0.0f, 0.0f, 0.0f, 0.0f);

   //m_rotation_matrix = new_rot_mat;

   

   
   // recalculate the new mouse position to be in the window world in which the 
   // origin is the window center (instead of the upper left corner) and in which
   // +x is right and +y is towards the top of the window (instead of towards the
   // bottom of the window)
   glm::vec2 window_center = glm::vec2(window_width / 2.0f, window_height / 2.0f);
   float new_x = (new_mouse_position.x - window_center.x) / (window_center.x);
   float new_y = (new_mouse_position.y - window_center.y) / (window_center.y);

   glm::vec3 new_mouse_position_in_centered_coordinates = glm::vec3(new_x, new_y, 0.0f); 
   glm::vec3 object_position = glm::vec3(m_translation_matrix[3].x, m_translation_matrix[3].y, -1.0f);
   glm::vec3 point_vector = new_mouse_position_in_centered_coordinates - object_position;

   float rotation_angle_around_x = glm::atan(point_vector.y, point_vector.z);

   // I don't understand quote how this arctan(...) function is calculating the 
   // angle, but it is inverted and 90 degrees less than what I want.  Magically
   // account for it.
   float rotation_angle_around_y = (-1.0f) * glm::atan(point_vector.z, point_vector.x) + ((1.0f / 2.0f) * 3.14159);
   float rotation_angle_around_z = glm::atan(point_vector.y, point_vector.x);
   m_rotation_matrix = glm::rotate(glm::mat4(), rotation_angle_around_x, glm::vec3(1.0f, 0.0f, 0.0f));
   m_rotation_matrix = glm::rotate(m_rotation_matrix, rotation_angle_around_y, glm::vec3(0.0f, 1.0f, 0.0f));
   //m_rotation_matrix = glm::rotate(m_rotation_matrix, rotation_angle_around_z, glm::vec3(0.0f, 0.0f, 1.0f));

   //glm::vec2 mouse_delta = new_mouse_position - m_prev_mouse_position;





   //// attempt to prevent "camera jump" when the mouse leaves the window by only 
   //// updating it's view direction if the delta is under a limit
   //if (glm::length(mouse_delta) < 50.0f)
   //{
   //   const float ROTATION_SENSITIVITY = 0.5f;
   //   float rotate_angle_rad_x = -mouse_delta.x * (2.0f * 3.14159f) / 360.0f;
   //   float rotate_angle_rad_y = -mouse_delta.y * (2.0f * 3.14159f) / 360.0f;

   //   m_strafe_direction = glm::cross(m_view_direction, m_world_up_vector);
   //   glm::mat4 rotator_matrix =
   //      glm::rotate(rotate_angle_rad_x * ROTATION_SENSITIVITY, m_world_up_vector) *
   //      glm::rotate(rotate_angle_rad_y * ROTATION_SENSITIVITY, m_strafe_direction);

   //   m_view_direction = glm::mat3(rotator_matrix) * m_view_direction;

   //   cout << "camera view direction: x=" << m_view_direction.x <<
   //      ", y=" << m_view_direction.y << ", z=" << m_view_direction.z << endl;
   //}

   //m_prev_mouse_position = new_mouse_position;

   return 0;
}

int my_shape::reset_thineself()
{
   int this_ret_val = 0;

   // ??check for things??

   // reset to the identity matrix
   m_rotation_matrix = glm::mat4();


   return this_ret_val;
}

