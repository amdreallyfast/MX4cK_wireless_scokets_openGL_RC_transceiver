#pragma once

#include "my_shape.h"

class shape_generator
{
public:
   // use a double pointer for the argument because we will need to assign the 
   // result of a "new" call to it
   // Note: I discovered by accident that I was assigning the result to a copy of
   // a pointer, which resulted in a dangling pointer.
   static int make_cube(my_shape **shape_ptr_to_assign_to);

private:
   shape_generator();
   shape_generator(const shape_generator&);
   shape_generator& operator=(const shape_generator&);
};

