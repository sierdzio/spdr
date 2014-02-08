/*! \mainpage

  Spdr library and tools: readme

\section links Other important documents

 - \ref changelog "changelog"
 - \ref license "license"

\section readme Readme

spdr
Copyright (C) 2014  Tomasz Siekierda

This is an on-line README for spdr.
Project’s GIT repo: https://github.com/sierdzio/spdr

WARNING: spdr is in the process of being rewritten with different functionality
in mind!

As of  12.01.2014, the plan is to completely rewrite sPDaR. Project will change
it's name to spdr, license will from now on be WTFPL. Spdr will be a tool for
merging (intelligent syncing) of folders, for example when copying large
collection of files between computers. It will detect what changes have been
made and try to bring the output directory up to date.

Core functionality will be moved into a library: libspdr. On top of that library,
a Command Line application will be written, as well as a GUI (using Qt Widgets,
and possibly also another one using Qt Quick 2).

Current sPDaR functionality will be renamed to "Import directory" and be just
a small part of the whole project. The code will be analysed and reviewed, as
currently it's rather old and rusty.

All new code will be (hopefully) unit tested. Also, benchmarks will be implemented
to test how well spdr fares under certain conditions. The whole project must
have a comprehensive logging capabilities, be easy to install (configure file),
etc.

Support for Qt4 will be dropped, because the new spdr will use some of the new
features present in Qt 5.2 (QCommandLine, QRegularExpression,
QDir:removeRecursively() maybe qCDebug and friends). As an additional experiment,
spdr might also include QBS and CMake build configuration.

Overview of the algorithm:
 - scan output, put it into local cache (comprising of data hash and file path)
 - for every file in input, see if MD5/ SHA can be matched. If not, try matching
   by file path and file name. The last resort is to try and find the file by
   content (that would be very CPU-consuming, though!)

Directory renames will be part of standard file copying logic. Logging should
be extensive, support various output levels, and every message should be timed.

The GUI will support Drag & Drop for getting input and output folders.

\section cmdln Command Line Tool

Spdr command line tool is fully-functional (implements all features of Spdr
library). Below is the list of command line flags it recognizes.

\verbatim
Options:
  -h, --help            Displays this help.
  -v, --version         Displays version information.
  -i, --import          Spdr will perform import operation: it will copy all
                        files from input according to formatting tags specified
                        in output directory path. See the documentation for more
                        information
  -s, --synchronize     (default) Spdr will perform file synchronization. See
                        the documentation for more information
  -l, --log <path>      Log file location. If empty/ not specified, Spdr will
                        print to stdout
  --log-level <number>  Specifies how many log messages will be shown. Accepted
                        values are 0 (no logging) to 6 (log everything,
                        including debug output). See the documentation for more
                        information.
  -m, --move            Files will be moved from input to output (input will
                        change!). By default, Spdr copies the files (leavign
                        input intact)
  --simulate            Spdr will simulate all the actions, without actually
                        doing any changes to the file system

Arguments:
  input                 Input folder, usually containing updated data
  output                Output folder, usually containing old data. When Import
                        operation is performed, the path can contain
                        QDateTime-style time and date tags enclosed in < and >,
                        as well as star * wildcard matching. See the
                        documentation for more information
\endverbatim

\section requirements System requirements
any working PC/ Mac or other device that Qt runs on, Qt 5.2 installed (minimum).

\section authors Authors

\include AUTHORS.txt

\section todolist Project's TODO
 - a LOT :-)
 - use EXIF comparison
 - use DIFF comparison
 - move SpdrFileData to a separate (private) header: it can be useful for SpdrImport, too
 - combine recursive reading of directories with recursive reading of files: it can be done
   in a single function by detecting directories on the fly
*/
