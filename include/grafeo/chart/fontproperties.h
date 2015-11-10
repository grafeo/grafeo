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
#ifndef GRF_FONT_PROPERTIES_H
#define GRF_FONT_PROPERTIES_H
#include <glib-object.h>
#include <grafeo/core.h>

BEGIN_DECLS

#ifndef G_DECLARE_DERIVABLE_TYPE
#define G_DECLARE_DERIVABLE_TYPE(ModuleObjName, module_obj_name, MODULE, OBJ_NAME, ParentName) \
  GType module_obj_name##_get_type (void);                                                               \
  G_GNUC_BEGIN_IGNORE_DEPRECATIONS                                                                       \
  typedef struct _##ModuleObjName ModuleObjName;                                                         \
  typedef struct _##ModuleObjName##Class ModuleObjName##Class;                                           \
  struct _##ModuleObjName { ParentName parent_instance; };                                               \
                                                                                                         \
  _GLIB_DEFINE_AUTOPTR_CHAINUP (ModuleObjName, ParentName)                                               \
                                                                                                         \
  static inline ModuleObjName * MODULE##_##OBJ_NAME (gconstpointer ptr) {                                     \
    return G_TYPE_CHECK_INSTANCE_CAST (ptr, module_obj_name##_get_type (), ModuleObjName); }             \
  static inline ModuleObjName##Class * MODULE##_##OBJ_NAME##_CLASS (gconstpointer ptr) {                      \
    return G_TYPE_CHECK_CLASS_CAST (ptr, module_obj_name##_get_type (), ModuleObjName##Class); }         \
  static inline gboolean MODULE##_IS_##OBJ_NAME (gconstpointer ptr) {                                         \
    return G_TYPE_CHECK_INSTANCE_TYPE (ptr, module_obj_name##_get_type ()); }                            \
  static inline gboolean MODULE##_IS_##OBJ_NAME##_CLASS (gconstpointer ptr) {                                 \
    return G_TYPE_CHECK_CLASS_TYPE (ptr, module_obj_name##_get_type ()); }                               \
  static inline ModuleObjName##Class * MODULE##_##OBJ_NAME##_GET_CLASS (gconstpointer ptr) {                  \
    return G_TYPE_INSTANCE_GET_CLASS (ptr, module_obj_name##_get_type (), ModuleObjName##Class); }       \
  G_GNUC_END_IGNORE_DEPRECATIONS
#endif

typedef enum{
  GRF_FONT_STYLE_NORMAL,
  GRF_FONT_STYLE_OBLIQUE,
  GRF_FONT_STYLE_ITALIC,
}GrfFontStyle;

typedef enum{
  GRF_FONT_VARIANT_NORMAL,
  GRF_FONT_VARIANT_SMALL_CAPS,
}GrfFontVariant;

typedef enum{
  GRF_FONT_STRETCH_ULTRA_CONDENSED = 1001,
  GRF_FONT_STRETCH_EXTRA_CONDENSED = 1002,
  GRF_FONT_STRETCH_CONDENSED       = 1003,
  GRF_FONT_STRETCH_SEMI_CONDENSED  = 1004,
  GRF_FONT_STRETCH_NORMAL          = 1005,
  GRF_FONT_STRETCH_SEMI_EXPANDED   = 1006,
  GRF_FONT_STRETCH_EXPANDED        = 1007,
  GRF_FONT_STRETCH_EXTRA_EXPANDED  = 1008,
  GRF_FONT_STRETCH_ULTRA_EXPANDED  = 1009,
}GrfFontStretch;

typedef enum{
  GRF_FONT_WEIGHT_ULTRALIGHT = 1001,
  GRF_FONT_WEIGHT_LIGHT      = 1002,
  GRF_FONT_WEIGHT_NORMAL     = 1003,
  GRF_FONT_WEIGHT_REGULAR    = 1004,
  GRF_FONT_WEIGHT_BOOK       = 1005,
  GRF_FONT_WEIGHT_MEDIUM     = 1006,
  GRF_FONT_WEIGHT_ROMAN      = 1007,
  GRF_FONT_WEIGHT_SEMIBOLD   = 1008,
  GRF_FONT_WEIGHT_DEMIBOLD   = 1009,
  GRF_FONT_WEIGHT_DEMI       = 1010,
  GRF_FONT_WEIGHT_BOLD       = 1011,
  GRF_FONT_WEIGHT_HEAVY      = 1012,
  GRF_FONT_WEIGHT_EXTRA_BOLD = 1013,
  GRF_FONT_WEIGHT_BLACK      = 1014,
}GrfFontWeight;

#define GRF_TYPE_FONT_PROPERTIES grf_font_properties_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfFontProperties, grf_font_properties, GRF, FONT_PROPERTIES, GObject)

typedef struct _GrfFontPropertiesClass{
  GObjectClass parent_class;
}GrfFontPropertiesClass;

/**
 * @brief grf_font_properties_new
 * @return
 */
GrfFontProperties*
grf_font_properties_new();

END_DECLS

#endif
