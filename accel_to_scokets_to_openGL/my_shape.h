#pragma once

// for definition of my vertex structure
#include "my_vertex.h"

// for typedefs (GLuint, etc.)
#include <glew-1.10.0\include\GL\glew.h>

// use a struct, not a class, to communicate that I want everything to default to
// being public
// Note: This is an organizational data structur for a bunch of data and therefore
// does not need anything to be private.  Use "struct" instead of "class" to 
// clarify intent.
struct my_shape
{
public:
   my_shape();
   ~my_shape();

   // these inquisitive functions will be used for glBufferData(...)
   // Note: Use a GLsizeiptr because that is what glBufferData(...) wants for the
   // size of the buffer.
   GLsizeiptr vertex_buffer_size() const;
   GLsizeiptr index_buffer_size() const;

   int draw_thineself(void(*gl_context_drawing_function_ptr)(void));

   my_vertex *m_vertices_arr;
   GLuint m_num_vertices;
   GLushort *m_indices_arr;
   GLuint m_num_indices;

   GLuint m_vertex_buffer_ID;
   GLuint m_index_buffer_ID;
};
