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
  int   min;
  int   max;
  int   pos;
  char* name;

  GtkWidget* slider;
  GtkWidget* hbox;
  GtkWidget* lbl_min;
  GtkWidget* lbl_max;
  GtkWidget* lbl_name;


} GrfTrackbarPrivate;

// ImageWidget definition
G_DEFINE_TYPE_WITH_PRIVATE(GrfTrackbar, grf_trackbar, GTK_TYPE_CONTAINER)

GrfTrackbar*
grf_trackbar_new(){
  return gtk_widget_new(GRF_TYPE_TRACKBAR,NULL);
}

GrfTrackbar*
grf_trackbar_new_with_name(char* name){

}

void
grf_trackbar_set_max (GrfTrackbar* trackbar, int max_value){

}

void
grf_trackbar_set_min (GrfTrackbar* trackbar, int min_value){

}

void
grf_trackbar_set_pos (GrfTrackbar* trackbar, int pos){

}

void
grf_trackbar_set_name(GrfTrackbar* trackbar, char* name){

}

int
grf_trackbar_get_max(GrfTrackbar* trackbar){

}

int
grf_trackbar_get_min(GrfTrackbar* trackbar){

}

int
grf_trackbar_get_pos(GrfTrackbar* trackbar){

}

char*
grf_trackbar_get_name(GrfTrackbar* trackbar){

}
