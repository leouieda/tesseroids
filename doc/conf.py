# -*- coding: utf-8 -*-
from __future__ import print_function
import sys
import os
import datetime

# Needed to get the version string from versioneer
sys.path.append('..')
import print_version

project = u'Tesseroids'
copyright = u'2009-{:d}, Leonardo Uieda'.format(datetime.date.today().year)
version = 'v{}'.format(print_version.version)
release = version.split('-')[0]
print(version, release)
doi = "10.5281/zenodo.16033"
rst_epilog = """
.. |doi| replace:: {}
.. |doilink| replace:: doi:`{} <http://dx.doi.org/{}>`__
""".format(doi, doi, doi)

extensions = ['sphinx.ext.mathjax']
templates_path = ['_templates']
source_suffix = '.rst'
#source_encoding = 'utf-8-sig'
master_doc = 'index'

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
#today = ''
# Else, today_fmt is used as the format for a strftime call.
today_fmt = '%Y'
exclude_patterns = ['_build']
pygments_style = 'sphinx'
try:
    import sphinx_rtd_theme
    html_theme = "sphinx_rtd_theme"
    html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
except ImportError:
    html_theme = 'default'
#html_theme_options = {}
#html_theme_path = []
html_title = "{} v{}".format(project, version)
# A shorter title for the navigation bar.  Default is the same as html_title.
#html_short_title = None
html_logo = '_static/logo.png'
html_favicon = '_static/logo.ico'
html_static_path = ['_static']
# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
html_use_smartypants = True
# Output file base name for HTML help builder.
htmlhelp_basename = 'Tesseroidsdoc'
