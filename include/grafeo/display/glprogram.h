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
#ifndef GRF_GL_PROGRAM_H
#define GRF_GL_PROGRAM_H
#include <glib-object.h>
#include <grafeo/display.h>

#define GRF_TYPE_GL_PROGRAM grf_gl_program_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGlProgram, grf_gl_program, GRF, GL_PROGRAM, GObject)

typedef struct _GrfGlProgramClass{
  GObjectClass parent_class;
}GrfGlProgramClass;
/**
 * @brief grf_gl_program_new
 * @return
 */
GrfGlProgram*
grf_gl_program_new();
/**
 * @brief grf_gl_program_new_from_files
 * @param vertexfile
 * @param fragmentfile
 * @param geometryfile
 * @return
 */
GrfGlProgram*
grf_gl_program_new_from_files(const char* vertexfile,
                              const char* fragmentfile,
                              const char* geometryfile);
/**
 * @brief grf_gl_program_new_from_text
 * @param vertexsource
 * @param fragmentsource
 * @param geometrysource
 * @return
 */
GrfGlProgram*
grf_gl_program_new_from_text(char* vertexsource,
                             char* fragmentsource,
                             char* geometrysource);

/**
 * @brief grf_gl_program_compile
 * @param program
 */
void
grf_gl_program_compile(GrfGlProgram* program);
/**
 * @brief grf_gl_program_attach_shader
 * @param program
 * @param shader
 */
void
grf_gl_program_attach_shader(GrfGlProgram* program, GrfGlShader* shader);
/**
 * @brief grf_gl_program_use
 * @param program
 */
void
grf_gl_program_use(GrfGlProgram* program);
/**
 * @brief grf_gl_program_get_uniform
 * @param program
 * @param name
 * @return
 */
GLint
grf_gl_program_get_uniform(GrfGlProgram* program, const char* name);
/**
 * @brief grf_gl_program_get_attrib
 * @param program
 * @param name
 * @return
 */
GLint
grf_gl_program_get_attrib(GrfGlProgram* program, const char* name);
#endif
