# Making a new release of Tesseroids

1. `git checkout master`, just to be sure
2. Update the changelog with the version number
3. `make check`, `make test`, `cd cookbook; make all`, `make doc`, and
   `make package` to make sure everything works
4. `git tag v1.2.3` to mark a new release from the current commit
5. `git push --tags` to tell Github to make a new release
6. `make package` and upload the binary builds to Github (the sources are
   created automatically)
7. Grab a new DOI from Zenodo or figshare
8. Update the website
