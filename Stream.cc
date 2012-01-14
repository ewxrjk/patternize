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
#include <cerrno>
#include <cstring>
#include <cstdarg>
#include <stdexcept>

Stream::Stream(const std::string &path, const std::string &mode):
  fp(fopen(path.c_str(), mode.c_str())),
  name(path) {
  if(!fp)
    throw IOError("opening", name, errno);
}

Stream::~Stream() {
  if(fp)
    fclose(fp);
}

bool Stream::readline(std::string &line) {
  int c;
  line.clear();
  while((c = getc(fp)) != EOF && c != '\n')
    line += c;
  if(ferror(fp))
    throw IOError("reading", name, errno);
  return line.size() || c == '\n';
}

void Stream::readall(std::string &s) {
  int c;
  s.clear();
  while((c = getc(fp)) != EOF)
    s += c;
  if(ferror(fp))
    throw IOError("reading", name, errno);
}
  
int Stream::printf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int rc = vfprintf(fp, format, ap);
  va_end(ap);
  if(rc < 0)
    throw IOError("writing", name, errno);
  return rc;
}

void Stream::write(int c) {
  if(putc(c, fp) < 0)
    throw IOError("writing", name, errno);
}    

void Stream::write(const char *ptr, size_t n) {
  fwrite(ptr, 1, n, fp);
  if(ferror(fp))
    throw IOError("writing", name, errno);
}

void Stream::write(const std::string &s,
                   std::string::size_type pos,
                   std::string::size_type n) {
  if(pos >= s.size())
    return;
  if(n > s.size() - pos)
    n = s.size() - pos;
  write(s.data() + pos, n);
}

void Stream::close() {
  if(fclose(fp) < 0)
    throw IOError("closing", name, errno);
  fp = NULL;
}
