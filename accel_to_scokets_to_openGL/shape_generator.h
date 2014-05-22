#pragma once

#include "my_shape.h"

class shape_generator
{
public:
   static int make_cube(my_shape *shape_ptr_to_assign_to);

private:
   shape_generator();
   shape_generator(const shape_generator&);
   shape_generator& operator=(const shape_generator&);
};

