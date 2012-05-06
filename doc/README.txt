sierdzio's Photo Downloader and Redistributor

README

This is an on-line README for sPDaR (https://sourceforge.net/projects/spdar/).
Project’s GIT repo: https://gitorious.org/spdar/

This a very early version of this file. It will be updated in the future.
To download photos:
 - connect media containing photos (if you have them somewhere remote),
 - click "Select folder to import from" and browse
   to the folder containing photos,
 - click "Select folder to export to" and browse target folder
   (all photos will be copied there),
 - select folder structure (currently only one is available),
 - click the “Transfer!” button.

To redistribute photos:
 - click “Select folder containing photos” and browse to your archive location
   (where developed/ processed photos are),
 - click “Select the destination folder” and browse to your destination,
 - click “Transfer!” button to start copying developed photos.

Note that sPDaR copies files that have no other version (no RAW file).
Other files are being MOVED. This means that those files are deleted
in the source archive, and are transferred to the destination archive.

This way, you will have two archives: a “developer” one, containing original
RAW, JPG and TIFF files, and another “user” one, where only
the developed/ processed files are stored (not RAWs).

System requirements:
any working PC/ Mac or other device that Qt runs on (Symbian, Maemo, Win CE...),
Linux dependencies for binaries (does NOT apply to the source code and Windows versions): Qt4.8.1.
