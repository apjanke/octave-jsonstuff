JsonStuff Developer Notes
=========================

# Build and CI

### Environments

| OS | Environment | OS Version           | Octave | jsoncpp | Texinfo | Qt | Comments |
| -- | | ----------- | ------------------ | ------ | ----- |------- | ----- | -------- |
| Ubuntu | Bionic     | 18 Bionic         | 4.2.2  | 1.7   | 6.5    | 5.9   | No Travis env yet |
| Ubuntu | Xenial     | 16 Xenial         | 4.0    | 1.7   | 6.1    | 5.5   |       |
| Ubuntu | Trusty     | 14 Trusty         | 3.8    | 0.6   | 5.2    | 5.2   |       |
| macOS | xcode10.1   | 10.13 High Sierra |        |       | 4.8    |       |       |
| macOS | xcode10     | 10.13 High Sierra |        |       | 4.8    |       |       |
| macOS | xcode9.4    | 10.13 High Sierra |        |       | 4.8    |       |       |
| macOS | xcode9.2    | 10.12 Sierra      |        |       | 4.8    |       |       |
| macOS | Homebrew (1/2019) | 10.13+      | 4.4    | 1.8   | 6.5    | 5.12  |       |

Texinfo version is the system version on Mac, except in the "Homebrew" environment. Homebrewed versions are all up to date on macOS, so I'm not bothering listing them.

I'm assuming Ubuntu stays on the same major/minor version for its packages within a release.

I think we currently require:

| Package | Version | Why                     |
| ------- | ------- | ----------------------- |
| Texinfo | 5.x     | Supports auto-numbering |
| Octave  | 4.4     | |
| jsoncpp | ???     | |
