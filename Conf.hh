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

#ifndef CONF_HH
#define CONF_HH

#include <map>
#include <vector>
#include <set>
#include <string>

class Conf {
public:
  Conf();
  Conf(const std::string &path);

  void readconf(const std::string &path);

  typedef std::vector<std::string> patterns_type;
  patterns_type patterns;
  typedef std::map<std::string,std::vector<std::string> > categories_type;
  categories_type categories;

  int getErrors() const { return errors; }

  enum Distribution {
    Uniform,
    TwoLevel,
  };
  Distribution getDistribution() const { return distribution; }

private:
  void readfile(const std::string &path);
  int errors;
  Distribution distribution;

};

#endif /* CONF_HH */

