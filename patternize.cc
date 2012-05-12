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
#include "IO.hh"
#include <getopt.h>
#include <cstdlib>
#include <sys/time.h>
#include <clocale>

static const struct option options[] = {
  { "help", no_argument, 0, 'h' },
  { "version", no_argument, 0, 'V' },
  { "number", required_argument, 0, 'n' },
};

static void help(), version();

int main(int argc, char **argv) {
  int n;
  int count = 1;

  try {
    if(!setlocale(LC_CTYPE, ""))
      throw std::runtime_error(std::string("setlocale: ") + strerror(errno));
    while((n = getopt_long(argc, argv, "+hVn:", options, 0)) >= 0) {
      switch(n) {
      case 'h': help();
      case 'V': version();
      case 'n': count = atoi(optarg); break;
      default: return 1;
      }
    }
    const int args_remaining = argc - optind;
    if(args_remaining < 1)
      throw std::runtime_error("must specify a configuration file");
    if(args_remaining > 1)
      throw std::runtime_error("must only specify one configuration file");
    Conf c(argv[optind]);
    if(c.getErrors())
      return 1;
    Stream s(stdout, "stdout");
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
      throw std::runtime_error(std::string("gettimeofday: ") + strerror(errno));
    srand48(tv.tv_sec ^ tv.tv_usec);
    while(count-- > 0)
      s.printf("%s\n", pick(c).c_str());
    s.close();
  } catch(std::runtime_error &e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
  return 0;
}

static void help() {
  printf("Usage:\n"
         "  patternize [OPTIONS] PATH\n"
         "Options:\n"
         "  --number, -n COUNT     Number of strings to generate\n"
         "  --help, -h             Display usage message\n"
         "  --version, -V          Display version string\n");
  exit(0);
}

static void version() {
  puts(VERSION);
  exit(0);
}
