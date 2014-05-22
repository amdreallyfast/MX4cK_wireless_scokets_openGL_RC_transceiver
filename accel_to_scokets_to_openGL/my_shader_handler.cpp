#include "my_shader_handler.h"

// for reader shader text from files
#include <fstream>
#include <sstream>

// for printing to the console
#include <iostream>
using std::cout;
using std::endl;


// I declared constructor, even though it doesn't really do anything, so I need to
// define it
my_shader_handler::my_shader_handler() :
   m_shader_program_ID(0)
{

}

my_shader_handler::~my_shader_handler()
{
   // tell the GL context to unbind using its current program so that we can delete 
   // it
   // Note: Nothing will happen if we tell GL to delete a program that is currently 
   // bound to the context, so we need to unbind it before we can delete it.
   // Also Note: If, for some reason, the shader program ID was never set, that is
   // ok because the "delete program" function will silently swallow 0s.
   glUseProgram(0);
   glDeleteProgram(m_shader_program_ID);
}

my_shader_handler& my_shader_handler::get_instance()
{
   static my_shader_handler S;

   return S;
}

int my_shader_handler::install_shaders()
{
   int this_ret_val = 0;
   const char* old_GL_double_pointer_adapter[1];
   std::string shader_code_text;

   // create the shader objects
   GLuint vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
   
   // starting with the vertex shader, give it the source material, then compile it
   this_ret_val = read_text_from_file("vertex_shader_.glsl", shader_code_text);
   if (0 == this_ret_val)
   {
      old_GL_double_pointer_adapter[0] = shader_code_text.c_str();
      glShaderSource(vertex_shader_ID, 1, old_GL_double_pointer_adapter, 0);
      glCompileShader(vertex_shader_ID);
      this_ret_val = check_shader_status(vertex_shader_ID);
   }

   if (0 == this_ret_val)
   {
      // repeat for the fragment shader;
      this_ret_val = read_text_from_file("fragment_shader.glsl", shader_code_text);
      if (0 == this_ret_val)
      {
         old_GL_double_pointer_adapter[0] = shader_code_text.c_str();
         glShaderSource(fragment_shader_ID, 1, old_GL_double_pointer_adapter, 0);
         glCompileShader(fragment_shader_ID);
         this_ret_val = check_shader_status(fragment_shader_ID);
      }
   }

   if (0 == this_ret_val)
   {
      // create a shader executable for the GPU out of the compiled shader objects 
      // that have been created so far
      m_shader_program_ID = glCreateProgram();
      glAttachShader(m_shader_program_ID, vertex_shader_ID);
      glAttachShader(m_shader_program_ID, fragment_shader_ID);
      glLinkProgram(m_shader_program_ID);
      this_ret_val = check_program_status(m_shader_program_ID);

      if (0 == this_ret_val)
      {
         // program linking went ok, so set the context to use this shader program
         glUseProgram(m_shader_program_ID);
      }
   }


   // regardless of what happened, delete the shader objects, which we do not need
   // to keep around
   // Note: It is ok if no shader object was created and the IDs are still 0.  The
   // "delete shader" function will silently swallow 0s.
   glDeleteShader(vertex_shader_ID);
   glDeleteShader(fragment_shader_ID);


   return this_ret_val;
}


int my_shader_handler::check_shader_program_status(
   GLuint object_ID,
   PFNGLGETSHADERIVPROC object_property_getter_funct_ptr,
   GLenum object_status_type,
   PFNGLGETSHADERINFOLOGPROC info_log_getter_func_ptr
   )
{
   int this_ret_val = 0;

   // ??any input shenanigans to check??

   GLint status;
   object_property_getter_funct_ptr(object_ID, object_status_type, &status);
   if (GL_TRUE != status)
   {
      this_ret_val = -1;

      // problem; try to report it

      // find out how long the error log is, allocate space for it, and print it
      // Note: The "actual error log length" variable is left over from when people 
      // would put a magic number into the error log memory allocation, and then the 
      // next function would put the actual length into this variable instead of 
      // returning anything.  We can get the actual length up front, so this 
      // variable is a formality to keep the compiler happy by supplying the 
      // apropriate arguments.
      GLint err_log_len = 0;
      GLchar *err_log_buffer = 0;
      GLsizei actual_err_log_len = 0;

      object_property_getter_funct_ptr(object_ID, GL_INFO_LOG_LENGTH, &err_log_len);
      err_log_buffer = new GLchar[err_log_len];
      info_log_getter_func_ptr(object_ID, err_log_len, &actual_err_log_len, err_log_buffer);
      cout << err_log_buffer << endl;

      delete[] err_log_buffer;
   }

   return this_ret_val;
}

int my_shader_handler::check_shader_status(GLuint shader_ID)
{
   int this_ret_val = 0;

   this_ret_val = check_shader_program_status(
      shader_ID,
      glGetShaderiv,
      GL_COMPILE_STATUS,
      glGetShaderInfoLog);
   if (0 == this_ret_val)
   {
      cout << "shader ID '" << shader_ID << "' compiled of" << endl;
   }
   else
   {
      // problem (??error already reported in "check shader program status", so do anything??)
   }


   return this_ret_val;
}

int my_shader_handler::check_program_status(GLuint program_ID)
{
   int this_ret_val = 0;

   this_ret_val = check_shader_program_status(
      program_ID,
      glGetProgramiv,
      GL_LINK_STATUS,
      glGetProgramInfoLog);
   if (0 == this_ret_val)
   {
      // things are ok
      cout << "program ID '" << program_ID << "' linked ok" << endl;
   }
   else
   {
      // problem (??error already reported in "check shader program status", so do anything??)
   }


   return this_ret_val;
}

int my_shader_handler::read_text_from_file(const char *file_path, std::string &put_text_here)
{
   int this_ret_val = 0;
   bool ret_val = false;
   std::ifstream in_file_stream;

   in_file_stream.open(file_path, std::ifstream::in);
   ret_val = in_file_stream.is_open();
   if (!ret_val)
   {
      // problem
      this_ret_val = -1;
      cout << "could not open shader file '" << file_path << "'" << endl;
   }
   else
   {
      // read the entire file contents into the argument string
      std::ostringstream file_contents;
      file_contents << in_file_stream.rdbuf();
      in_file_stream.close();
      put_text_here = file_contents.str();
   }


   return this_ret_val;
}

