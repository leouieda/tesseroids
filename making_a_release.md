# Making a new release of Tesseroids

* `git checkout master`, just to be sure
* Run `make check`, `make test`, `cd cookbook; make all`, `make doc`, and
   `make package` and make sure everything works
* Update the changelog with the version number (insert below the "(future)"
   version)
* Reserve a new DOI from Zenodo
    * Create a new upload (use some dummy files and **delete them later**)
    * Click on "Pre-reserve DOI"
    * Copy in the title "Tesseroids vX.X.X: Forward modeling of gravitational
      fields in spherical coordinates"
    * Copy the project description (below)
    * Copy the changelog of this release to the description
    * Use tags: tesseroid, geophysics, modeling, C, gravity
* Update the DOI in the README
* Update the DOI in the `conf.py` of the docs
* Update the DOI in the Previous releases
* Update year in LICENSE.txt
* Commit the recent changes (DOI updates)
* `git tag vX.X.X` to mark a new release from the current commit
* `git push --tags` to tell Github to make a new release
* `make package` to build Linux and Windows binaries
* Create the release on Github
    * Upload the **only** binary builds to Github (the sources are  created
      automatically)
    * Copy the project description and latest changelog from Zenodo
    * Put a mention of the DOI with a link
* Upload the latest packages for the release on Zenodo and publish

Use the following project description for releases and Zenodo:

A collection of command-line programs for modeling the gravitational potential, acceleration, and gradient tensor. Tesseroids supports models and computation grids in Cartesian and spherical coordinates.

Official website and documentation: http://tesseroids.leouieda.com/

