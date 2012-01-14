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
#include "Parse.hh"

void Parse::parse(const std::string &input) {
  size_t pos = 0;
  while(pos < input.size()) {
    size_t d = input.find('$', pos);
    if(d != pos) {
      if(d == std::string::npos)
        d = input.size();
      text(input, pos, d - pos);
      pos = d;
    } else if(pos + 1 < input.size() && input[pos + 1] == '$') {
      text("$", 0, 1);
      pos += 2;
    } else if(pos + 1 < input.size() && input[pos + 1] == '{') {
      pos += 2;
      d = input.find('}', pos);
      if(d != std::string::npos) {
        substitution(input, pos, d - pos);
        pos = d + 1;
      } else {
        // TODO report error?
        pos = input.size();
      }
    } else {
      // TODO report error?
      ++pos;
    }
  }
}

void Parse::text(const std::string &,
                 std::string::size_type, std::string::size_type) {
}

void Parse::substitution(const std::string &,
                         std::string::size_type, std::string::size_type) {
}
