Readme for test data files of SpdrImport class.

This directory contains files used by libspdr's test suite.

Details:
 - files 1 to 5 are identical (same data), which can be used to test how Spdr
   behaves when presented with copied data
 - file 6 has same data, but different creation time
 - file 7 and 8 have different suffix cases, making them good candidates for
   testing file suffix case sensitivity
 - files in subfolder can be used to test how duplicates are being handled
 - *only* files 1-4 are already in the output
