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
#include "HTMLExpander.hh"
#include "Conf.hh"
#include "IO.hh"
#include "patternize.hh"
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <langinfo.h>

static std::string locate(const std::string &name);
static std::string myself();

static const char *const dirs[] = {
  PKGCONFDIR "/cgi",
  PKGDATADIR "/cgi",
  NULL
};

int main() {
  try {
    if(setlocale(LC_CTYPE, "") < 0)
      throw std::runtime_error(std::string("setlocale: ") + strerror(errno));
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
      throw std::runtime_error(std::string("gettimeofday: ") + strerror(errno));
    srand48(tv.tv_sec ^ tv.tv_usec);
    // Callers can choose between different configurations...
    const char *config = getenv("QUERY_STRING");
    if(!config || !*config)
      config = "default";
    // Configurations must be strictly alphanumeric and not unreasonably long
    // (so the naming length limits can't be overflowed)
    if(strlen(config) > 16)
      throw std::runtime_error("invalid configuration name");
    for(size_t i = 0; config[i]; ++i)
      if(!isalnum(config[i]))
        throw std::runtime_error("invalid configuration name");
    // Find the configuration and read it
    const std::string fullConfig = locate(std::string(config) + ".patterns");
    Conf c(fullConfig);
    if(c.getErrors())
      throw std::runtime_error("parse of " + fullConfig + " failed");
    // Find the template and read it
    std::string t;
    Stream(locate(std::string(config) + ".html"), "r").readall(t);
    // Set up HTML output
    Stream s(stdout, "stdout");
    HTMLExpander h(s);
    h.set("result", pick(c));
    h.set("config", config);
    h.set("package", PACKAGE);
    h.set("version", VERSION);
    if(!strcmp(config, "default"))
      h.set("uri", myself());
    else
      h.set("uri", myself() + "?" + config);
    s.printf("Content-Type: text/html; charset=%s\n"
             "\n",
             nl_langinfo(CODESET));
    h.parse(t);
    s.close();
  } catch(std::runtime_error &e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
  return 0;
}

static std::string locate(const std::string &name) {
  for(size_t i = 0; dirs[i]; ++i) {
    std::string path = std::string(dirs[i]) + "/" + name;
    if(access(path.c_str(), F_OK) == 0)
      return path;
  }
  throw std::runtime_error(name + " not found in any search directory");
}

static std::string myself() {
  const char *script_name_env = getenv("SCRIPT_NAME");
  std::string script_name = script_name_env ? script_name_env : "/cgi-bin/patternize";
  const char *server_name_env = getenv("SERVER_NAME");
  std::string server_name = server_name_env ? server_name_env : "localhost";
  const char *server_port_env = getenv("SERVER_PORT");
  std::string server_port = server_port_env ? server_port_env : "80";
  if(server_port == "80")
    return "http://" + server_name + script_name;
  else
    return "http://" + server_name + ":" + server_port + script_name;
}
