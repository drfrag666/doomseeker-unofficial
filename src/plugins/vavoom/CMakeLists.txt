#------------------------------------------------------------------------------
# CMakeLists.txt
#------------------------------------------------------------------------------
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301  USA
#------------------------------------------------------------------------------
# Copyright (C) 2010 Braden "Blzut3" Obrzut <admin@maniacsvault.net>
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.12)

set(PLUGIN_NAME vavoom)
set(PLUGIN_VERSION 17)
include(../PluginHeader.cmake)

set(VAVOOM_FILES
	vavoomengineplugin.cpp
	vavoomgameinfo.cpp
	vavoomgamerunner.cpp
	vavoommasterclient.cpp
	vavoomserver.cpp
)

set(HEADER_FILES
	vavoomengineplugin.h
	vavoomgameinfo.h
	vavoomgamerunner.h
	vavoommasterclient.h
	vavoomserver.h
)

add_library(${PLUGIN_NAME} MODULE ${VAVOOM_FILES} ${HEADER_FILES})

include(../PluginFooter.cmake)
