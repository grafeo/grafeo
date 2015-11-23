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
#ifndef GRF_GL_SHADER_H
#define GRF_GL_SHADER_H
#include <epoxy/gl.h>
#include <grafeo/core.h>
#include <grafeo/display.h>
#include <glib-object.h>
BEGIN_DECLS
#define GRF_TYPE_GL_SHADER grf_gl_shader_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGlShader, grf_gl_shader, GRF, GL_SHADER, GObject)

typedef struct _GrfGlShaderClass{
  GObjectClass parent_instance;
}GrfGlShaderClass;
/**
 * @brief grf_gl_shader_new
 * @return
 */
GrfGlShader*
grf_gl_shader_new();

/**
 * @brief grf_gl_shader_new_from_file
 * @param shaderfile
 * @param shadertype
 * @return
 */
GrfGlShader*
grf_gl_shader_new_from_file(const char* shaderfile, GLenum shadertype);
/**
 * @brief grf_gl_shader_new_from_text
 * @param shadersource
 * @param shadertype
 * @return
 */
GrfGlShader*
grf_gl_shader_new_from_text(char* shadersource, GLenum shadertype);
/**
 * @brief grf_gl_shader_compile
 * @param shader
 */
void
grf_gl_shader_compile(GrfGlShader* shader);

/**
 * @brief grf_gl_shader_get_id
 * @param shader
 * @return
 */
GLuint
grf_gl_shader_get_id(GrfGlShader* shader);
/**
 * @brief grf_gl_shader_get_shadertype
 * @param shader
 * @return
 */
GLenum
grf_gl_shader_get_shadertype(GrfGlShader* shader);

END_DECLS
#endif
