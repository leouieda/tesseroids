################################################################################
#
#    Copyright (C) 2009  Leonardo Uieda
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    Doubts and/or suggestions please mail the author:
#         Leonardo Uieda - leouieda@gmail.com
#
################################################################################

################################################################################
# Created on 15-Jul-2009 08:49 PM
# Last modified by $Author:  $
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision: $'
__date__ = '$Date: $'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

"""
The setup script for the Tesseroids software package.
"""


from distutils.core import setup

setup( \
  name='tesseroids', \
  fullname='Tesseroids', \
  description="Direct modeling of gravity fields in spherical coordinates", \
  version='0.3', \
  author="Leonardo Uieda", \
  author_email='leouieda@gmail.com', \
  license='GPL', \
  url='http://code.google.com/p/tesseroids/', \
  scripts=['tesspot', \
           'tessgx', 'tessgy', 'tessgz', \
           'tessgxx', 'tessgxy', 'tessgxz', \
                      'tessgyy', 'tessgyz', \
                                 'tessgzz'], \
  py_modules=['glq'], \
  packages=['tesseroids_lib'])