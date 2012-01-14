# patternize © 2011 Richard Kettlewell
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

bin_PROGRAMS=patternize
noinst_PROGRAMS=patternize.cgi
patternize_SOURCES=Conf.cc Directory.cc Parse.cc patternize.cc Pick.cc	\
Stream.cc Conf.hh IO.hh Parse.hh patternize.hh
patternize_cgi_SOURCES=Conf.cc Directory.cc HTMLExpander.cc Parse.cc	\
Pick.cc Stream.cc Conf.hh HTMLExpander.hh IO.hh Parse.hh patternize.hh	\
cgi.cc
AM_CPPFLAGS=-DPKGCONFDIR='"$(sysconfdir)/$(PACKAGE)"' -DPKGDATADIR='"$(pkgdatadir)"'
dist_man_MANS=patternize.1
EXTRA_DIST=tumblr.patterns debian/changelog debian/control	\
debian/copyright debian/rules scripts/dist README.cgi

