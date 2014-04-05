/*!

\mainpage

  Spdr library and tools: readme

\brief This file contains a basic description of what Spdr is and how it can be
used.

\section links Other important documents

 - \ref changelog "changelog"
 - \ref license "license"

\section readme Readme

Copyright (C) 2014  Tomasz Siekierda
Project’s GIT repo: https://github.com/sierdzio/spdr

Spdr is a tool for merging (intelligent syncing) of folders, for example when
copying large collection of files between computers. It detects what changes
have been made and tries to bring the output directory up to date.

Core functionality is located in a standalone library: libspdr. On top of that
library, Spdr provides a Command Line application, as well as a GUI (using Qt
Widgets).

Spdr offers two major features:
 - importing a directory (\ref importOverview "detailed description")
 - synchronizing two directories (\ref synchronizationOverview "detailed description")

Both operations share a host of common properties: input and output files need
to be specified, there is an option of logging, etc.

Import operation works by copying all files from input directory to the output,
and sorting them according to specified formatting options (based on date and
time). See \ref importOverview "documentation" for SpdrImport for more information.

Synchronize operation is the core functionality of Spdr. The aim is to merge the
folders so that the output folder and file structure is exactly the
same as in the input folder, and to do it will as little effort as possible. If
you have ever tried to manually copy your movie or foto collection into an
archive, you probably know how hard and error-prone that can be. Spdr will do it
for you automatically and fast. The algorithm optimizes the whole operation by
skipping updates for unchanged files and minimizing the file operations that
need to be performed on the hard drive. See \ref synchronizationOverview "documentation"
for SpdrSynchronize for more information.

Directory renames are part of standard file copying logic, so you do not need to
worry about that.

Spdr comes with serious logging capabilities, it supports various output levels.
Logs are sent to stdout by default, but you can choose to save them in a file
(errors and critical errors will be sent to both stdout and the log file).

The GUI supports Drag & Drop for getting input and output folders (and the
log file, too). Just drag the folder from your file explorer and drop in onto
input, output or log file paths control.

\section cmdln Command Line Tool

Spdr command line tool is fully-functional (implements all features of Spdr
library). Below is the list of command line flags it recognizes.

\verbatim
Options:
  -h, --help            Displays this help.
  -v, --version         Displays version information.
  -i, --import          Spdr will perform import operation: it will copy all fi
                        les from input according to formatting tags specified i
                        n output directory path. See the documentation for more
                         information
  -s, --synchronize     (default) Spdr will perform file synchronization. See t
                        he documentation for more information
  -l, --log <path>      Log file location. If empty/ not specified, Spdr will p
                        rint to stdout
  --log-level <number>  Specifies how many log messages will be shown. Accepted
                         values are 0 (no logging) to 6 (log everything, includ
                        ing debug output). See the documentation for more infor
                        mation.
  -m, --move            Files will be moved from input to output (input will ch
                        ange!). By default, Spdr copies the files (leavign inpu
                        t intact)
  --simulate            Spdr will simulate all the actions, without actually do
                        ing any changes to the file system

Arguments:
  input                 Input folder, usually containing updated data
  output                Output folder, usually containing old data. When Import
                         operation is performed, the path can contain QDateTime
                        -style time and date tags enclosed in < and >, as well
                        as star * wildcard matching. See the documentation for
                        more information
\endverbatim

\section requirements System requirements
any working PC/ Mac or other device that Qt runs on, Qt 5.2 installed (minimum).

The Spdr library and CLI both depend only on QtCore module. Spdr GUI requires
QtCore, QtGui and QtWidgets to work.

\section authors Authors

\include AUTHORS.txt

\section todolist Project's TODO
 - add "quick" search: skip calculating Md5 to improve performance at the expense
   of synchronization accuracy
 - more deep tests for SpdrSynchronize
 - implement Spdr::Ask
 - use EXIF comparison
 - use DIFF comparison
 - combine recursive reading of directories with recursive reading of files:
   it can be done in a single function by detecting directories on the fly
 - add configure script
 - add automated localisation deployment script
 - add automated deployment script
 - add support for importing/ synchronizing hidden files. Test it!
*/
