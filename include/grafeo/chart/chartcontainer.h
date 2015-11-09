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
#ifndef GRF_CHART_CONTAINER_H
#define GRF_CHART_CONTAINER_H

#include <glib-object.h>
#include <grafeo/chart.h>
#include <grafeo/core.h>

G_BEGIN_DECLS

#define GRF_TYPE_CHART_CONTAINER grf_chart_container_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfChartContainer, grf_chart_container, GRF, CHART_CONTAINER, GrfChartComponent)

typedef struct _GrfChartContainerClass{
  GrfChartComponentClass parent_instance;
  void       (*set_title)         (GrfChartContainer* self, char* title);
  void       (*set_size)          (GrfChartContainer* self, grfdim_t dim,
                                   grfsize_t* size);
  char*      (*get_title)         (GrfChartContainer *self);
  grfsize_t* (*get_size)          (GrfChartContainer* self);
  grfdim_t   (*get_dim)           (GrfChartContainer* self);
  grfsize_t  (*get_num_components)(GrfChartContainer* self);
  grfsize_t  (*get_num_leafs)    (GrfChartContainer* self,
                                   grfbool_t recursive);
  void       (*add_component)     (GrfChartContainer* self,
                                   GrfChartComponent* component,
                                   grfsize_t* position);
  void       (*remove_component)  (GrfChartContainer* self,
                                   GrfChartComponent* component);
}GrfChartContainerClass;

/**
 * @brief Creates a new instance of the container for a chart
 * @return
 */
GrfChartContainer*
grf_chart_container_new();

/*=========================
 *      MUTATORS
 *=======================*/

/**
 * @brief Set the title of the chart. If `NULL` is passed, the title won't be
 *        rendered
 * @param self
 * @param title
 */
void
grf_chart_container_set_title(GrfChartContainer* chart_container, char* title);
/**
 * @brief Set the size (number of components) for each dimension of the
 *        container.
 *
 * @param self
 * @param dim
 * @param size
 */
void
grf_chart_container_set_size(GrfChartContainer* chart_container, grfdim_t dim,
                             grfsize_t* size);

/*=========================
 *      ACCESSORS
 *=======================*/
/**
 * @brief grf_chart_container_get_title
 * @param chart_container
 * @return
 */
char*
grf_chart_container_get_title(GrfChartContainer* chart_container);

/**
 * @brief grf_chart_container_get_size
 * @param chart_container
 * @return
 */
grfsize_t*
grf_chart_container_get_size(GrfChartContainer* chart_container);

/**
 * @brief grf_chart_container_get_dim
 * @param chart_container
 * @return
 */
grfdim_t
grf_chart_container_get_dim(GrfChartContainer* chart_container);

/**
 * @brief grf_chart_container_get_num_components
 * @param chart_container
 * @return
 */
grfsize_t
grf_chart_container_get_num_components(GrfChartContainer* chart_container);

/**
 * @brief grf_chart_container_get_num_panels
 * @param chart_container
 * @return
 */
grfsize_t
grf_chart_container_get_num_leafs(GrfChartContainer* chart_container, grfbool_t recursive);
/**
 * @brief grf_chart_container_is_empty
 * @param chart_container
 * @return
 */
grfbool_t
grf_chart_container_is_empty(GrfChartContainer* chart_container);

/*=========================
 *      OPERATIONS
 *=======================*/
/**
 * @brief grf_chart_container_add_component
 * @param chart_container
 * @param chart_component
 * @param position
 */
void
grf_chart_container_add_component(GrfChartContainer* chart_container,
                                  GrfChartComponent* chart_component);
/**
 * @brief grf_chart_container_remove_component
 * @param chart_container
 * @param chart_component
 */
void
grf_chart_container_remove_component(GrfChartContainer* chart_container,
                                     GrfChartComponent* chart_component);
G_END_DECLS

#endif
