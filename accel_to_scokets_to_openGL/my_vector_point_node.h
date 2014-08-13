#ifndef MY_VECTOR_POINT_NODE_H
#define MY_VECTOR_POINT_NODE_H

#include "my_glm_hpp_include.h"

struct vector_point_node
{
   glm::vec3 vector_to_point_at;
   struct vector_point_node* next;
};

#endif
