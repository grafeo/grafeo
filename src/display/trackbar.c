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

  int*  variable;
  GrfTrackbarCallback trackbar_changed_event;


} GrfTrackbarPrivate;

// ImageWidget definition
G_DEFINE_TYPE_WITH_PRIVATE(GrfTrackbar, grf_trackbar, GTK_TYPE_BOX)

static void
grf_trackbar_class_init(GrfTrackbarClass *klass){
  klass->set_max  = grf_trackbar_set_max;
  klass->set_min  = grf_trackbar_set_min;
  klass->set_name = grf_trackbar_set_name;
  klass->set_pos  = grf_trackbar_set_pos;
}
static void
grf_trackbar_init(GrfTrackbar *self){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(self);
  priv->hbox     = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  priv->lbl_name = gtk_label_new("variable");
  priv->scale    = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL,NULL);
  gtk_scale_set_digits(GTK_SCALE(priv->scale),0);

  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->lbl_name,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->scale, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(self),priv->hbox,TRUE,TRUE,0);
}

static gboolean
grf_trackbar_changed_event(GtkRange* range, GtkScrollType scroll, gdouble value, gpointer user_data){
  (void) scroll;(void) range;

  GrfTrackbar* trackbar = GRF_TRACKBAR(user_data);
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  *(priv->variable) = (int) value;
  priv->trackbar_changed_event((int) value);
  return FALSE;
}

/*=================================
 * PUBLIC API
 *=================================*/

GrfTrackbar*
grf_trackbar_new(){
  return GRF_TRACKBAR(gtk_widget_new(GRF_TYPE_TRACKBAR,NULL));
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
  gtk_range_set_range(GTK_RANGE(priv->scale), priv->min_value, priv->max_value);
}

void
grf_trackbar_set_min (GrfTrackbar* trackbar, int min_value){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->min_value = min_value;
  gtk_range_set_range(GTK_RANGE(priv->scale), priv->min_value, priv->max_value);
}

void
grf_trackbar_set_pos (GrfTrackbar* trackbar, int pos){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->pos = pos;
  gtk_scale_set_draw_value(GTK_SCALE(priv->scale), (double)pos);
}

void
grf_trackbar_set_name(GrfTrackbar* trackbar, const char* name){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  gtk_label_set_label(GTK_LABEL(priv->lbl_name),(const gchar*)name);
  priv->name = (char*)name;
}

int
grf_trackbar_get_max(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  return priv->max_value;
}

int
grf_trackbar_get_min(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  return priv->min_value;
}

int
grf_trackbar_get_pos(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  return priv->pos;
}

char*
grf_trackbar_get_name(GrfTrackbar* trackbar){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  return priv->name;
}

void
grf_trackbar_connect_change_callback(GrfTrackbar* trackbar, int* variable, GrfTrackbarCallback trackbar_changed_event){
  GrfTrackbarPrivate* priv = grf_trackbar_get_instance_private(trackbar);
  priv->trackbar_changed_event = trackbar_changed_event;
  priv->variable = variable;
  g_signal_connect(priv->scale, "change-value", G_CALLBACK(grf_trackbar_changed_event), trackbar);
}
