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
typedef struct _GrfGlShaderPrivate{
  GLuint       id;
  char*        source;
  const char*  filename;
  GLenum       type;
}GrfGlShaderPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfGlShader, grf_gl_shader, G_TYPE_OBJECT)

static void
grf_gl_shader_init(GrfGlShader *self){

}
static void
grf_gl_shader_class_init(GrfGlShaderClass *klass){

}

/*=================================
 * PUBLIC API
 *=================================*/
GrfGlShader*
grf_gl_shader_new(){
  return g_object_new(GRF_TYPE_GL_SHADER, NULL);
}

GrfGlShader*
grf_gl_shader_new_from_file(const char* shaderfile, GLenum shadertype){
  char* text;
  g_file_get_contents(shaderfile,&text,NULL,NULL);
  GrfGlShader* shader      = grf_gl_shader_new_from_text(text, shadertype);
  GrfGlShaderPrivate* priv = grf_gl_shader_get_instance_private(shader);
  priv->filename           = shaderfile;
  return shader;
}

GrfGlShader*
grf_gl_shader_new_from_text(char* shadersource, GLenum shadertype){
  GrfGlShader* shader      = grf_gl_shader_new();
  GrfGlShaderPrivate* priv = grf_gl_shader_get_instance_private(shader);
  priv->source             = shadersource;
  priv->id                 = glCreateShader(shadertype);
  priv->type               = shadertype;
  glShaderSource(priv->id, 1, (const GLchar* const*)&priv->source, NULL);
  return shader;
}

void
grf_gl_shader_compile(GrfGlShader* shader){
  GrfGlShaderPrivate* priv = grf_gl_shader_get_instance_private(shader);

  GLint result;
  char* shader_error_message;
  int   info_log_length;

  glCompileShader(priv->id);

  glGetShaderiv(priv->id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
    glGetShaderiv(priv->id, GL_INFO_LOG_LENGTH, &info_log_length);
    shader_error_message = malloc(info_log_length+1);
    glGetShaderInfoLog(priv->id, info_log_length, NULL, shader_error_message);
    fprintf(stdout, "%s\n", shader_error_message);
    free(shader_error_message);
  }
}

GLuint
grf_gl_shader_get_id(GrfGlShader* shader){
  GrfGlShaderPrivate* priv = grf_gl_shader_get_instance_private(shader);
  return priv->id;
}
GLenum
grf_gl_shader_get_shadertype(GrfGlShader* shader){
  GrfGlShaderPrivate* priv = grf_gl_shader_get_instance_private(shader);
  return priv->type;
}
