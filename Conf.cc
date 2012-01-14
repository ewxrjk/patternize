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
#include "Conf.hh"
#include "IO.hh"
#include <sys/stat.h>
#include <algorithm>

Conf::Conf(): errors(0) {
}

Conf::Conf(const std::string &path): errors(0) {
  readconf(path);
}

void Conf::readconf(const std::string &path) {
  struct stat s;
  if(stat(path.c_str(), &s) < 0)
    throw IOError("accessing", path, errno);
  if(S_ISDIR(s.st_mode)) {
    Directory d(path);
    std::string name;
    std::vector<std::string> names;
    while(d.get(name))
      if(name.size()
         && name[0] != '.'
         && name.find('~') == std::string::npos)
        names.push_back(name);
    std::sort(names.begin(), names.end());
    for(size_t i = 0; i < names.size(); ++i)
      readfile(path + "/" + names[i]);
  } else if(S_ISREG(s.st_mode)) {
    readfile(path);
  } else
    throw std::runtime_error(path + " must be a file or directory");
}

void Conf::readfile(const std::string &path) {
  Stream s(path, "r");
  std::string line;
  std::vector<std::string> *target = NULL;
  int lineno = 0;
  bool reported_missing_section = false;
  while(s.readline(line)) {
    ++lineno;
    if(line.size() == 0 || line[0] == '#' || line[0] == ';')
      continue;
    if(line[0] == '[') {
      // Skip initial whitespace
      size_t pos = 1;
      while(pos < line.size() && isspace(line[pos]))
        ++pos;
      // The section type
      size_t start = pos;
      while(pos < line.size() && isalnum(line[pos]))
        ++pos;
      if(pos < line.size() && !isspace(line[pos]) && line[pos] != ']') {
        fprintf(stderr, "%s:%d:%zu: invalid character in section type\n",
                path.c_str(), lineno, pos);
        ++errors;
        continue;
      }
      std::string type(line, start, pos - start);
      // Intervening whitespace
      while(pos < line.size() && isspace(line[pos]))
        ++pos;
      // The section name, optionally
      std::string name;
      if(pos < line.size() && isalnum(line[pos])) {
        start = pos;
        while(pos < line.size() && isalnum(line[pos]))
          ++pos;
        if(pos < line.size() && line[pos] != ']') {
          fprintf(stderr, "%s:%d:%zu: invalid character in section name\n",
                  path.c_str(), lineno, pos);
          ++errors;
          continue;
        }
        name.assign(line, start, pos - start);
        // Trailing whitespace
        while(pos < line.size() && isspace(line[pos]))
          ++pos;
      }
      // Expect a final ] and nothing else
      if(!(pos < line.size() && line[pos] == ']')) {
        fprintf(stderr, "%s:%d:%zu: missing close bracket\n",
                path.c_str(), lineno, pos);
        ++errors;
      } else if(pos + 1 != line.size()) {
        fprintf(stderr, "%s:%d:%zu: junk at end of line\n",
                path.c_str(), lineno, pos);
        ++errors;
      }
      if(type == "patterns")
        target = &patterns;
      else if(type == "category")
        target = &categories[name];
      else {
        fprintf(stderr, "%s:%d: invalid section type '%s'\n",
                path.c_str(), lineno, type.c_str());
        ++errors;
      }
    } else {
      if(target)
        target->push_back(line);
      else if(!reported_missing_section) {
        reported_missing_section = true;
        fprintf(stderr, "%s:%d: missing section declaration\n",
                path.c_str(), lineno);
        ++errors;
      }
    }
  }
}
