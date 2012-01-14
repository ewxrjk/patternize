#include <config.h>
#include "HTMLExpander.hh"
#include "IO.hh"

HTMLExpander::HTMLExpander(Stream &o): output(o) {
}

void HTMLExpander::set(const std::string &name, const std::string &value) {
  vars[name] = value;
}

void HTMLExpander::text(const std::string &text,
                        std::string::size_type pos,
                        std::string::size_type n) {
  output.write(text, pos, n);
}

void HTMLExpander::substitution(const std::string &text,
                                std::string::size_type pos,
                                std::string::size_type n) {
  const std::string name(text, pos, n);
  std::map<std::string,std::string>::const_iterator it = vars.find(name);
  if(it != vars.end()) {
    const std::string &value = it->second;
    for(size_t i = 0; i < value.size(); ++i) {
      switch(value[i]) {
      case '<':
      case '&':
      case '"':
        output.printf("&#%d;", value[i]);
        break;
      default:
        output.write(value[i]);
        break;
      }
    }
  }
}
