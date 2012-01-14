#ifndef EXPANDER_HH
#define EXPANDER_HH

#include "Parse.hh"
#include <map>

class Stream;

class HTMLExpander: public Parse {
  Stream &output;
  std::map<std::string,std::string> vars;
public:
  HTMLExpander(Stream &output);
  void set(const std::string &name, const std::string &value);
  void text(const std::string &text,
            std::string::size_type pos,
            std::string::size_type n);
  void substitution(const std::string &text,
                    std::string::size_type pos,
                    std::string::size_type n);

};

#endif /* EXPANDER_HH */

