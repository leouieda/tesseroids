"""
Get the version string from versioneer and print it to stdout
"""
import sys
import os

# Make sure versioneer is imported from here
here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, here)
import versioneer

versioneer.VCS = 'git'
versioneer.tag_prefix = 'v'
versioneer.versionfile_source = 'version.py' # This line is useless
versioneer.parentdir_prefix = 'tesseroids-'
version = versioneer.get_version()
if version == 'master':
    # When downloading a zip from Github, versioneer gets the version from the
    # directory name in the zip file. That would endup being 'master'. More
    # useful would be the commit hash. The pattern below will be replaced by
    # git when 'git archive' is called
    version = '$Format:%H$'

if __name__ == '__main__':
    print(version)
