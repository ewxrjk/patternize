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
#include "patternize.hh"
#include "Conf.hh"
#include "Parse.hh"
#include <cstdlib>
#include <stdexcept>
#include <cstdio>

typedef unsigned long long choice_type;
#define CF "llu"

static choice_type getChoices(const Conf &c, const std::string &pattern);
static std::string substitute(const Conf &c,
                              const std::string &pattern,
                              choice_type choice);

std::string pick(const Conf &c) {
  // Figure out the total number of possible choices
  choice_type choices = 0;
  std::vector<std::string> subs;
  for(size_t n = 0; n < c.patterns.size(); ++n)
    choices += getChoices(c, c.patterns[n]);
  // Pick one of them
  double r = drand48();
  choice_type choice = r * choices;
  // Find the right pattern and expand it
  for(size_t n = 0; n < c.patterns.size(); ++n) {
    choice_type choices_here = getChoices(c, c.patterns[n]);
    if(choice < choices_here)
      return substitute(c, c.patterns[n], choice);
    else
      choice -= choices_here;
  }
  throw std::logic_error("pick() fell off the end");
}

// ----------------------------------------------------------------------------

class CountSubstitutions: public Parse {
  const Conf &c;
public:
  CountSubstitutions(const Conf &conf): c(conf), choices(1) {}
  choice_type choices;
  void substitution(const std::string &text,
                    std::string::size_type pos,
                    std::string::size_type n) {
    const std::string category(text, pos, n);
    Conf::categories_type::const_iterator it = c.categories.find(category);
    if(it != c.categories.end())
      choices *= it->second.size();
  }
};

static choice_type getChoices(const Conf &c, const std::string &pattern) {
  CountSubstitutions cs(c);
  cs.parse(pattern);
  return cs.choices;
}

// ----------------------------------------------------------------------------

class Substitute: public Parse {
  const Conf &c;
  choice_type choice;
public:
  Substitute(const Conf &conf, choice_type ch): c(conf), choice(ch) {}
  std::string result;
  void text(const std::string &text,
            std::string::size_type pos,
            std::string::size_type n) {
    result.append(text, pos, n);
  }
  void substitution(const std::string &text,
                    std::string::size_type pos,
                    std::string::size_type n) {
    const std::string category(text, pos, n);
    Conf::categories_type::const_iterator it = c.categories.find(category);
    if(it != c.categories.end()) {
      const std::vector<std::string> &subs = it->second;
      size_t nsubs = subs.size();
      result.append(subs[choice % nsubs]);
      choice /= nsubs;
    }
  }
};

static std::string substitute(const Conf &c,
                              const std::string &pattern,
                              choice_type choice) {
  Substitute s(c, choice);
  s.parse(pattern);
  return s.result;
}
