#pragma once

#include "my_shape.h"

class shape_generator
{
public:
   int make_cube(my_shape *shape_data_to_fill_ptr);

private:
   shape_generator();
   shape_generator(const shape_generator&);
   shape_generator& operator=(const shape_generator&);
};

