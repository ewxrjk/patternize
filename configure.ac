# patternize © 2011,13 Richard Kettlewell
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.61])
AC_INIT([patternize], [0.2], [rjk@greenend.org.uk])
AC_CONFIG_AUX_DIR([config.aux])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([patternize.cc])
AM_CONFIG_HEADER([config.h])
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_RANLIB
AC_SET_MAKE
AC_C_BIGENDIAN
AC_DEFINE([_GNU_SOURCE], [1], [use GNU extensions])
if test "x$GXX" = xyes; then
  CXXFLAGS="$CXXFLAGS -std=c++11 -Wall -W -Werror -Wpointer-arith -Wwrite-strings"
fi
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
