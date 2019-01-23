JsonStuff TODO
==============

## Big things

* Get building and running on Windows
* Fix build on Ubuntu
  * Need to include `<jsoncpp/json/json.h>` instead of just `<json/json.h>`
* Unify `make local` target and `src/Makefile`

## Code

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


## Project

* Get TravisBuddy working
* Travis build
  * run octave: run tests on installed package

