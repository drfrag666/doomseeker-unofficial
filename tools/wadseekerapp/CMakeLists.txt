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
# Copyright (C) 2009 Braden "Blzut3" Obrzut <admin@maniacsvault.net>
#------------------------------------------------------------------------------
project(WadseekerApp)

cmake_minimum_required(VERSION 2.8.12)

find_package(Qt4 COMPONENTS QtCore REQUIRED)

find_library(Wadseeker_LIBRARY wadseeker)
if(Wadseeker_LIBRARY-NOTFOUND)
	message(SEND_ERROR "Wadseeker was not found.")
else(Wadseeker_LIBRARY-NOTFOUND)
	message(STATUS "Wadseeker located at ${Wadseeker_LIBRARY}")
endif()

include_directories(${QT_INCLUDES})

qt4_wrap_cpp(QtMocFiles main.h)
add_executable(wadseeker ${QtMocFiles} main.cpp)

target_link_libraries(wadseeker ${QT_QTCORE_LIBRARY} ${Wadseeker_LIBRARY})
