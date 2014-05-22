#pragma once

// for definition of vec4
#include "glm\glm\glm.hpp"

// for typedefs (GLuint, etc.)
#include <glew-1.10.0\include\GL\glew.h>

struct my_vertex
{
   glm::vec4 position;
   glm::vec4 color;

   // put in some static const variables that will make it easy for GL pointer 
   // attrib and other functions to figure out the dimensions of a vertex
   // Note: I make the variables static so that they will be independent of any
   // particular instance of this structure.  I checked with an experiment and
   // found that static variables do not count towards the structure's size.  This
   // is a good thing because we will be having a lot of these structures.
   // Note: I make the variables const so that I can declare and initialize them at
   // the same time without having to define any kind of initialization through a
   // constructor.  Besides, these values will not change throughout the program.
   static const GLuint m_num_entries_per_vertex = 8;
   static const GLsizei m_size_bytes_per_vertex = sizeof(glm::vec4) * 2;

   static const GLuint m_num_position_entries_per_vertex = 4;
   static const GLsizei m_size_bytes_per_position_vertex = sizeof(glm::vec4);

   static const GLuint m_num_color_entries_per_vertex = 4;
   static const GLsizei m_size_bytes_per_color_vertex = sizeof(glm::vec4);
};