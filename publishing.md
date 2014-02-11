# Publishing a new version

This is a checklist of things to do before marking a new version:

## Bump version number

Do this in (desperately need a better way):

* `src/lib/version.c`
* `doc/conf.py`
* `dist/Makefile`

## Build packages

Run the commands in the `Makefile` in `dist` to produce the packages.

## Create a release on Github

This will make a tag with the version number. You can the upload the built
packages for this version.

## Update the website

On the `gh-pages` branch:

* Bump the version number
* Update links to built packages
