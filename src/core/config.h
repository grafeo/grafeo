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
#ifndef GRF_CONFIG_H
#define GRF_CONFIG_H
#define GRF_VERSION_MAJOR 
#define GRF_VERSION_MINOR 
#define GRF_VERSION_RELEASE 
#define GRF_WEBSITE "http://grafeo.github.io"
#define GETTEXT_PACKAGE "grafeo"
#include <stdint.h>
#include <grafeo/i18n-lib.h>
const char* const* grf_config_get_authors_names();
const char* const* grf_config_get_authors_emails();
const char* grf_config_get_license();
const char* grf_config_get_website();
const char* grf_config_get_copyright();
const char* grf_config_get_description();
uint8_t grf_config_get_version_major();
uint8_t grf_config_get_version_minor();
uint8_t grf_config_get_version_release();
#endif
