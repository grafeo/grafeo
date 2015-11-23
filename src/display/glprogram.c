/* ===================================================================
#   Copyright (C) 2015-2015
#   Anderson Tavares <nocturne.pe at gmail.com> PK 0x38e7bfc5c2def8ff
#   Lucy Mansilla    <lucyacm at gmail.com>
#   Caio de Braz     <caiobraz at gmail.com>
#   Hans Harley      <hansbecc at gmail.com>
#   Paulo Miranda    <pavmbr at yahoo.com.br>
#
#   Institute of Mathematics and Statistics - IME
#   University of Sao Paulo - USP
#
#   This file is part of Grafeo.
#
#   Grafeo is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version
#   3 of the License, or (at your option) any later version.
#
#   Grafeo is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty
#   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#   See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with Grafeo.  If not, see
#   <http://www.gnu.org/licenses/>.
# ===================================================================*/
#include <grafeo/display.h>
/*=================================
 * PRIVATE API
 *=================================*/
typedef struct _GrfGlProgramPrivate{
  GLuint       id;
  GrfGlShader* vertexShader;
  GrfGlShader* fragmentShader;
  GrfGlShader* geometryShader;
}GrfGlProgramPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfGlProgram, grf_gl_program, G_TYPE_OBJECT)

static void
grf_gl_program_init(GrfGlProgram *self){
  GrfGlProgramPrivate* priv = grf_gl_program_get_instance_private(self);
  priv->vertexShader = NULL;
  priv->fragmentShader = NULL;
  priv->geometryShader = NULL;
}
static void
grf_gl_program_class_init(GrfGlProgramClass *klass){

}
/*=================================
 * PUBLIC API
 *=================================*/
GrfGlProgram*
grf_gl_program_new(){
  return g_object_new(GRF_TYPE_GL_PROGRAM, NULL);
}

GrfGlProgram*
grf_gl_program_new_from_files(const char* vertexfile,
                              const char* fragmentfile,
                              const char* geometryfile){
  GError* error;
  char* vertexsource = NULL, *fragmentsource = NULL, *geometrysource = NULL;
  gsize vertexlength, fragmentlength, geometrylength;
  // Read files
  if(vertexfile)
    g_file_get_contents(vertexfile,&vertexsource,&vertexlength,&error);
  if(fragmentfile)
    g_file_get_contents(fragmentfile,(gchar**)&fragmentsource,&fragmentlength,&error);
  if(geometryfile)
    g_file_get_contents(geometryfile,(gchar**)&geometrysource,&geometrylength,&error);

  // Create and compile shaders from contents
  return grf_gl_program_new_from_text(vertexsource, fragmentsource, geometrysource);
}

GrfGlProgram*
grf_gl_program_new_from_text(char*  vertexsource,
                             char*  fragmentsource,
                             char*  geometrysource){
  GrfGlProgram*        program    = grf_gl_program_new();
  GrfGlProgramPrivate* priv       = grf_gl_program_get_instance_private(program);
  priv->id                        = glCreateProgram();
  GrfGlShader*         shader     = NULL;
  char*                sources[3] = {vertexsource, fragmentsource, geometrysource};
  GLenum               types[3]   = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER};
  uint8_t              i;
  for(i = 0; i < 3; i++){
    if(sources[i]){
      shader = grf_gl_shader_new_from_text(sources[i],types[i]);
      grf_gl_shader_compile(shader);
      grf_gl_program_attach_shader(program, shader);
    }
  }
  return program;
}

void
grf_gl_program_compile(GrfGlProgram* program){
  GrfGlProgramPrivate* priv = grf_gl_program_get_instance_private(program);
  glLinkProgram (priv->id);

  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(priv->id, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(priv->id, 512, NULL, infoLog);
    printf("Error compilation program: %s\n", infoLog);
  }
}

void
grf_gl_program_attach_shader(GrfGlProgram* program, GrfGlShader* shader){
  GrfGlProgramPrivate* priv = grf_gl_program_get_instance_private(program);
  switch(grf_gl_shader_get_shadertype(shader)){
    case GL_VERTEX_SHADER  : priv->vertexShader   = shader;break;
    case GL_FRAGMENT_SHADER: priv->fragmentShader = shader;break;
    case GL_GEOMETRY_SHADER: priv->geometryShader = shader;break;
  }

  glAttachShader(priv->id,grf_gl_shader_get_id(shader));
}

void
grf_gl_program_use(GrfGlProgram* program){
  GrfGlProgramPrivate* priv = grf_gl_program_get_instance_private(program);
  glUseProgram(priv->id);
}

GLint
grf_gl_program_get_uniform(GrfGlProgram* program, const char* name){
  GrfGlProgramPrivate* priv = grf_gl_program_get_instance_private(program);
  return glGetUniformLocation(priv->id, (const GLchar*)name);
}

GLint
grf_gl_program_get_attrib(GrfGlProgram* program, const char* name){
  GrfGlProgramPrivate* priv = grf_gl_program_get_instance_private(program);
  return glGetAttribLocation(priv->id, (const GLchar*) name);
}
