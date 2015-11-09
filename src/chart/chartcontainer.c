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
#include <grafeo/chart.h>
/*=========================
 *      PRIVATE API
 *=======================*/
/**
 * Composite of charts
 * To get the
 */
typedef struct _GrfChartContainerPrivate{
  grfdim_t           dim;          /**< Layout of container: 1D (row or column) or 2D (grid) */
  grfsize_t        * size;         /**< Number of components in each dimension */
  GrfQueue           children;     /**< List of components */
  //GrfChartComponent* children;     /**< List of components */
}GrfChartContainerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfChartContainer, grf_chart_container, GRF_TYPE_CHART_COMPONENT)

static void
grf_chart_container_init(GrfChartContainer *self){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(self);
  priv->dim            = 0;
  priv->size           = NULL;
  priv->children.begin = NULL;
  priv->children.end   = NULL;
  priv->children.length= 0;
}
static void
grf_chart_container_class_init(GrfChartContainerClass *klass){
  klass->add_component      = grf_chart_container_add_component;
  klass->remove_component   = grf_chart_container_remove_component;
  klass->get_dim            = grf_chart_container_get_dim;
  klass->get_num_components = grf_chart_container_get_num_components;
  klass->get_num_leafs      = grf_chart_container_get_num_leafs;
  klass->get_size           = grf_chart_container_get_size;
  klass->get_title          = grf_chart_container_get_title;
  klass->set_size           = grf_chart_container_set_size;
  klass->set_title          = grf_chart_container_set_title;
}

/*=========================
 * PUBLIC API: Creation
 *=======================*/
GrfChartContainer*
grf_chart_container_new(){
  return g_object_new(GRF_TYPE_CHART_CONTAINER, NULL);
}

/*=========================
 * PUBLIC API: Mutators
 *=======================*/
void
grf_chart_container_set_title(GrfChartContainer *chart_container, char *title){
  grf_chart_component_set_title(GRF_CHART_COMPONENT(chart_container), title);
}

void
grf_chart_container_set_size(GrfChartContainer *chart_container, grfdim_t dim, grfsize_t *size){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  priv->dim = dim;
  if(priv->size) free(priv->size);
  priv->size = malloc(dim*sizeof(grfsize_t));
  memcpy(priv->size,size,dim*sizeof(grfsize_t));
}

/*=========================
 * PUBLIC API: Accessors
 *=======================*/
char*
grf_chart_container_get_title(GrfChartContainer* chart_container){
  return grf_chart_component_get_title(GRF_CHART_COMPONENT(chart_container));
}

grfsize_t*
grf_chart_container_get_size(GrfChartContainer *chart_container){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  return priv->size;
}

grfdim_t
grf_chart_container_get_dim(GrfChartContainer* chart_container){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  return priv->dim;
}

grfsize_t
grf_chart_container_get_num_components(GrfChartContainer *chart_container){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  return priv->children.length;
}

grfsize_t
grf_chart_container_get_num_leafs(GrfChartContainer *chart_container, grfbool_t recursive){
  grfsize_t sum = 0;
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  GrfList* list;
  for(list = priv->children.begin; list; list = grf_list_next(list)){
    GrfChartComponent* component = GRF_CHART_COMPONENT(grf_list_value(list));
    if(!GRF_IS_CHART_CONTAINER(component))
      sum++;
    else if(recursive)
      sum += grf_chart_container_get_num_leafs(GRF_CHART_CONTAINER(component), recursive);
  }
  return sum;
}
grfbool_t
grf_chart_container_is_empty(GrfChartContainer* chart_container){
  return grf_chart_container_get_num_components(chart_container) == 0;
}

GrfChartComponent*
grf_chart_container_get_first_leaf(GrfChartContainer* chart_container){
  // First
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  GrfList* list;
  for(list = priv->children.begin; list; list = grf_list_next(list)){
    GrfChartComponent* component = GRF_CHART_COMPONENT(grf_list_value(list));
  }
}
GrfChartComponent*
grf_chart_container_get_last_leaf(GrfChartContainer* chart_container){

}

/*=========================
 * PUBLIC API: Operations
 *=======================*/
void
grf_chart_container_add_component(GrfChartContainer *chart_container,
                                  GrfChartComponent *chart_component){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  grf_queue_append(&priv->children,chart_component);
}

void
grf_chart_container_remove_component(GrfChartContainer *chart_container,
                                     GrfChartComponent *chart_component){
  GrfChartContainerPrivate* priv = grf_chart_container_get_instance_private(chart_container);
  grf_queue_remove(&priv->children,chart_component);
}
