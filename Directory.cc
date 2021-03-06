// patternize © 2011 Richard Kettlewell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>
#include "IO.hh"

Directory::Directory(const std::string &path): dp(opendir(path.c_str())),
                                               name(path) {
  if(!dp)
    throw IOError("opening", name, errno);
}

Directory::~Directory() {
  if(dp)
    closedir(dp);
}

bool Directory::get(std::string &name) {
  errno = 0;
  struct dirent *de = readdir(dp);
  if(de) {
    name = de->d_name;
    return true;
  }
  if(errno)
    throw IOError("reading", name, errno);
  return false;
}
