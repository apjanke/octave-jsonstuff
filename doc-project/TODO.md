JsonStuff TODO
==============

## Big things

* Get building and running on Windows
* Fix build on Ubuntu
  * Need to include `<jsoncpp/json/json.h>` instead of just `<json/json.h>`
  * Get compiling on older Ubuntus: Trusty and Xenial
* Unify `make local` target and `src/Makefile`

## Code

* Switch to "amalgamated source" jsoncpp build so there's no external library dependencies and it works on Windows
* Tests
* jsondecode
  * conformant-struct condensation
  * Extension: field-filling for arrays of objects -> struct array instead of cell array of structs
* jsonencode
  * Maybe support Java objects in inputs?
* Other TODOs scattered around the code
* Maybe OOP-ify the code (keeping `jsonencode`/`jsondecode`) as front-end function wrappers
  * So I can support various encoding/decoding options
* Maybe [UBJSON](https://en.wikipedia.org/wiki/UBJSON) support
* Maybe octfile-ify jsonencode, for speed
* Documentation
  * __octave_link_register_doc__ doesn't work on Octave 4.2. Make PKG_ADD version-sensitive

## Project

* Get TravisBuddy working
* Travis build
  * run octave: run tests on installed package

