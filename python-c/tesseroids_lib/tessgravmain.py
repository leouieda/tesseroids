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
"""
    This module contains the class Main that is used as the user interface for
    the tessgrav programs.
"""

################################################################################
# Created on 5-May-2009 7:39 PM
# Last modified by $Author: leouieda $
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision: 56 $'
__date__ = '$Date: 2010-07-20 23:09:12 +0200 (Tue, 20 Jul 2010) $'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################


import sys
import logging
import optparse
import time

import tesseroid as ts
import glq

class Main:
    """
    The main class that handles the creation of an optparser and logger, does
    all the input and output.
    """

    def __init__(self, name, description, args, grav_calculator, version='0.3'):
        """
        name            - String with the name of the program;
        description     - String with a short description of the functionality
                          of the program;
        args            - The CMD arguments (with the name of the program in
                          [0]) This should be the sys.argv list;
        grav_calculator - The class that calculated the gravity field in
                          question. Preferably a subclass of TesseroidGravity;
        """
        self.name = name
        self.description = description
        self.args = args
        self.grav_calculator = grav_calculator
        self.version = version
        

    def start(self):
        """
        Run the program with the parameters specified in the contructor.
        """

        # Print the name and description and exit if no arguments where given
        if len(self.args) == 1:
            print self.name + ':'
            print self.description
            print "\nUse '%s -h' or '%s --help' for instructions." % \
                    (self.args[0], self.args[0])
            sys.exit()

        # DEFINE THE CMD ARGS AND PARSER
        ########################################################################
        # The usage instructions
        usage = """%prog MODEL_FILE [options]

        MODEL_FILE is an ASCII file containing the model elements (tesseroids).
        Each line of MODEL_FILE contains the information on a single tesseroid
        and should be formated as:

          W E S N Top Bottom Density

        all given in degrees and SI units (m for Top and Bottom; kg/m^3 for
        Density).
        OUTPUT is printed to stdout in the form:

          lon lat field

        lon, lat are in degrees and field in: mGal for g; Eotvos for the
        gradient tensor; m^2/s^2 for the potential."""

        # Create a parser
        parser = optparse.OptionParser(usage=usage, version=self.version)

        # Set the default of the parameters
        parser.set_defaults(verbose=True, region=False, spacing=False, \
                            height=False, point=False, order=[2,2,2], \
                            log=False)

        # Add the options
        parser.add_option('--quiet', '-q', action='store_false', \
            dest="verbose", help='Run program quietly. Disable printing ' + \
                                 'verbose messages to stderr;')

        parser.add_option('--region', '-r', nargs=4, action='store', \
            type='float', \
            help="Boundaries of the grid where the gravitational field " + \
            "will be calculated [in degrees];", metavar='W E S N')

        parser.add_option('--spacing', '-b', nargs=2, action='store', \
            type='float', metavar='DLON DLAT', \
            help="Spacing between the grid points [in degrees];")

        parser.add_option('--point', '-p', nargs=3, action='store', \
            type='float', \
            help="Calculate the gravitational field only at point " + \
            "(LON, LAT, HEIGHT) instead of a whole grid. " + \
            "Mutualy exclusive with options -r, -b and -e;", \
            metavar='LON LAT HEIGHT')

        parser.add_option('--order', '-o', nargs=3, type='int', \
            action='store', \
            help="Order of the Gauss-Legendre Quadrature in the lon, " + \
            "lat and radial directions. Defaults to -o2 2 2;", \
            metavar='OLON OLAT OR')

        parser.add_option('--height', '-e', nargs=1, type='float', \
            action='store', help="Height at which to position the " + \
            "calculation grid [in meters].", metavar="HEIGHT"   )

        parser.add_option('--log', '-l', nargs=1, type='string', \
            action='store', \
            help="Create a log file named FILE with debug information;", \
            metavar='FILE')
        ########################################################################

        # PARSE THE CMD AND CHECK THE OPTIONS GIVEN
        ########################################################################

        # Parse the options
        options, arguments = parser.parse_args()

        # Set the log file, if wanted
        if options.log:
            logging.basicConfig(level=logging.DEBUG,
                format='\n%(asctime)s %(name)-20s - %(levelname)-8s: %(message)s',
                datefmt='%m-%d-%y %H:%M:%S',
                filename=options.log,
                filemode='w')

        # Create a logger for the prog
        logger = logging.getLogger(self.args[0])
        logger.setLevel(logging.DEBUG)
        # Create console handler and set level to debug
        ch = logging.StreamHandler(strm=sys.stderr)
        # Create formatter
        formatter = logging.Formatter("%(message)s")
        # Add formatter to ch
        ch.setFormatter(formatter)
        # Set verbose level
        if options.verbose:
            ch.setLevel(logging.INFO)
        else:
            ch.setLevel(logging.WARNING)
        # Add ch to logger
        logger.addHandler(ch)
        
        # Be polite and greet the user
        logger.info("Welcome to %s version %s!" % (self.name, self.version))
        logger.info("Parcing cmd options...")

        # Check if any calculation point has been given
        if not options.region and not options.spacing and not options.height \
           and not options.point:
            logger.error("ERROR! Calculation point not given!")
            sys.exit()
        if options.point and \
           (options.region or options.spacing or options.height):
            logger.error("ERROR! Option -p is mutualy exclusive with -r, " + \
                    "-b and -e.")
            sys.exit()
        if (options.region and not options.spacing and not options.height) or \
           (options.spacing and not options.region and not options.height) or \
           (options.height and not options.region and not options.spacing):
            logger.error("ERROR! Can't have -r without -b or -e \n" + \
                         "or vice-versa.")
            sys.exit()

        # Check isf an input file was given
        if len(arguments) == 0:
            logger.error("ERROR! No input model file given.")
            sys.exit()
        if len(arguments) > 1:
            logger.error("ERROR! Too many input model files!")
            sys.exit()
        ########################################################################

        # READ THE MODEL FILE
        ########################################################################
        # Open input file
        logger.info("Openning model input file '%s'..." % (arguments[0]))
        try:
            modelfile = open(arguments[0], 'r')
        except IOError, e:
            logger.error("ERROR! " + str(e))
            sys.exit()

        # Read the model from the file
        logger.info("Reading model from file...")
        model = []
        try:
            # Line counter
            l = 1
            for line in modelfile:

                # Get the values from the line string
                values = line.split()

                # Skip blank lines
                if len(values) == 0:
                    l += 1
                    continue

                # Check if there are enough values in the line (7)
                if len(values) != 7:
                    logger.error("ERROR! Strange data in line %d of " % (l) + \
                                 "model file!")
                    return ''

                # Convert the values to floats and put them on Tesseroid classes
                try:
                    W = float(values[0])
                    E = float(values[1])
                    S = float(values[2])
                    N = float(values[3])
                    T = float(values[4])
                    B = float(values[5])
                    D = float(values[6])
                    tess = ts.Tesseroid(W, E, S, N, T, B, D, \
                                        '%d' % (len(model)+1) )
                    model.append(tess)

                except ValueError:
                    logger.error("ERROR! Strange data in line %d of " % (l) + \
                                 "model file!")
                    return ''

                except ts.TesseroidError, e:
                    logger.error(str(e))
                    return ''

                l += 1
        except:
            logger.error("Oops! Something went wrong while reading the " + \
                         "model file!")
            logger.error("Sorry I don't have more information.")
            logger.error("Please check your input file and the command line.")
            sys.exit()

        # Finally close the file
        finally:
            modelfile.close()

        logger.info("%d model elements read successfully!" % (len(model)))
        ########################################################################

        # CREATE THE CALCULATION GRID
        ########################################################################
        logger.info("Creating computation grid...")

        lons = []
        lats = []
        heights = []

        # If calculating on a grid
        if not options.point:
            # Store the height and spacing to avoid repeated lookups
            height = options.height
            dlon = options.spacing[0]
            dlat = options.spacing[1]
            # Iterate over the lats
            lat = options.region[2]
            while lat <= options.region[3]:
                # Iterate over the lons
                lon = options.region[0]
                while lon <= options.region[1]:
                    lons.append(lon)
                    lats.append(lat)
                    heights.append(height)
                    # Update lon
                    lon += dlon
                # Update lat
                lat += dlat
        # If calculating on a point, add only it to the grid
        else:
            lons.append(options.point[0])
            lats.append(options.point[1])
            heights.append(options.point[2])
        ########################################################################

        # CREATE THE ABSCISSAS, WEIGHTS AND TESSEROIDGRAVITY CLASS
        ########################################################################
        logger.info("Creating auxiliary objects...")

        # Create the Abscissas
        abslon = glq.Abscissas(options.order[0])
        abslat = glq.Abscissas(options.order[1])
        absr = glq.Abscissas(options.order[2])

        # Create the Weights
        wlon = glq.Weights(abslon)
        wlat = glq.Weights(abslat)
        wr = glq.Weights(absr)

        # Create the TesseroidGravity class
        tessgrav = self.grav_calculator(abslon, wlon, abslat, wlat, absr, wr)
        ########################################################################

        # CALCULATE THE GRAVITY FIELD
        ########################################################################
        logger.info("Calculating gravitational field...")
        start = time.clock()
        field = tessgrav.calculate(model, lons, lats, heights)
        end = time.clock()
        deltat = end - start
        logger.info("Time it took (s): %f" % deltat)
        ########################################################################

        # PRINT THE FIELD TO STDOUT
        # Output is in the following format:
        #    lon1 lat1 result11
        #    lon1 lat2 result12
        #    lon1 lat3 result13
        #    ...
        #    lon1 latn result1n
        #                               <empty line>
        #    lon2 lat1 result21
        #    ...
        ########################################################################
        logger.info("Printing result to stdout...")
        current_lon = lons[0]
        for lon, lat, f in zip(*[lons, lats, field]):
            if lon != current_lon:
                print ""
                current_lon = lon
            print "% 3.8f % 2.8f % .8f" % (lon, lat, f)    
        ########################################################################

        logger.info("Done!")