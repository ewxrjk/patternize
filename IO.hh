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

#ifndef IO_HH
#define IO_HH

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <dirent.h>

class Stream {
public:
  Stream(): fp(NULL) {}
  Stream(FILE *fp_, const std::string &path): fp(fp_), name(path) {}
  Stream(const std::string &path, const std::string &mode);
  ~Stream();

  bool readline(std::string &line);
  void readall(std::string &all);
  int printf(const char *format, ...);
  void write(int c);
  void write(const char *ptr, size_t n);
  void write(const std::string &s,
             std::string::size_type pos = 0,
             std::string::size_type n = std::string::npos);
  void close();

private:
  FILE *fp;
  std::string name;
};

class Directory {
public:
  Directory(): dp(NULL) {}
  Directory(const std::string &path);
  ~Directory();
  bool get(std::string &name);
private:
  DIR *dp;
  std::string name;
};

class IOError: public std::runtime_error {
public:
  IOError(const std::string &doing, const std::string &path, int errno_value):
    std::runtime_error(doing + " " + path + ": " + strerror(errno_value)) {
  }
};


#endif /* IO_HH */

