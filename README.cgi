patternize CGI
==============

To run patternize as a web toy, use patternize.cgi.  Either install it
it your cgi-bin directory or use a web server that recognizes the .cgi
extension.

patternize.cgi decides which configuration file to use via the query
string (the part of the URL after the '?').  If this is missing or
empty then 'default' is used.

The name of the configuration file must consist only of letters and
digits and must not exceeed 16 bytes in length.

The CGI first looks for NAME.patterns in a search path.  This is a
patternize configuration file as described in 'man patternize'.  Next
it looks for NAME.html in the same search path.  It need not be found
in the same location as the .patterns file.

The search path is:

  ${sysconfdir}/patternize/cgi    (Debian: /etc/patternize/cgi)
  ${datadir}/patternize/cgi       (Debian: /usr/share/patternize/cgi)

The .html file is a template using the same syntax as patterns.  The
following substitutions are defined:

  ${result}       The randomly picked string.
  ${config}       The configuration name used.
  ${package}      The name of the package, i.e. 'patternize'.
  ${version}      The version of the packacge.
  ${uri}          The URL of this page.
