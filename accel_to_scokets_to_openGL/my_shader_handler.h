#pragma once

// for parameter types
#include <glew-1.10.0\include\GL\glew.h>
#include <string>

class my_shader_handler
{
public:
   static my_shader_handler& get_instance();
   int install_shaders();
   GLuint get_shader_program_ID();

   ~my_shader_handler();

private:
   // declared private for the sake of singleton-ness
   my_shader_handler();
   my_shader_handler(const my_shader_handler&);
   my_shader_handler& operator=(const my_shader_handler&);

   int check_shader_program_status(
      GLuint object_ID,
      PFNGLGETSHADERIVPROC object_property_getter_funct_ptr,
      GLenum object_status_type,
      PFNGLGETSHADERINFOLOGPROC info_log_getter_func_ptr
      );
   int check_shader_status(GLuint shader_ID);
   int check_program_status(GLuint program_ID);
   int read_text_from_file(const char *file_path, std::string &put_text_here);

   GLuint m_shader_program_ID;
};

