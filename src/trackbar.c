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
#include <grafeo/trackbar.h>
/*=================================
 * PRIVATE API
 *=================================*/
// Private members
typedef struct _GrfTrackbarPrivate{
  int   min_value;
  int   max_value;
  int   pos;
  char* name;

  GtkWidget* scale;
  GtkWidget* hbox;
  GtkWidget* lbl_min;
  GtkWidget* lbl_max;
  GtkWidget* lbl_name;


} GrfTrackbarPrivate;

// ImageWidget definition
G_DEFINE_TYPE_WITH_PRIVATE(GrfTrackbar, grf_trackbar, GTK_TYPE_CONTAINER)

static grf_trackbar_class_init(GrfTrackbarClass *klass){
  klass->set_max  = grf_trackbar_set_max;
  klass->set_min  = grf_trackbar_set_min;
  klass->set_name = grf_trackbar_set_name;
  klass->set_pos  = grf_trackbar_set_pos;
}
static grf_trackbar_init(GrfTrackbar *self){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(self);
  priv->hbox     = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  priv->lbl_name = gtk_label_new("variable");
  priv->scale    = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL,NULL);

  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->lbl_name,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->scale, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(self),box);
}

/*=================================
 * PUBLIC API
 *=================================*/

GrfTrackbar*
grf_trackbar_new(){
  return gtk_widget_new(GRF_TYPE_TRACKBAR,NULL);
}

GrfTrackbar*
grf_trackbar_new_with_name(char* name){
  GrfTrackbar* trackbar = grf_trackbar_new();
  grf_trackbar_set_name(trackbar,name);
  return trackbar;
}

void
grf_trackbar_set_max (GrfTrackbar* trackbar, int max_value){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

void
grf_trackbar_set_min (GrfTrackbar* trackbar, int min_value){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

void
grf_trackbar_set_pos (GrfTrackbar* trackbar, int pos){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

void
grf_trackbar_set_name(GrfTrackbar* trackbar, char* name){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

int
grf_trackbar_get_max(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

int
grf_trackbar_get_min(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

int
grf_trackbar_get_pos(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}

char*
grf_trackbar_get_name(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->max_value = max_value;
}
