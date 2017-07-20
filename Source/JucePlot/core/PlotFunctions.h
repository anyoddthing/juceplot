/*
  ==============================================================================

    PlotFunctions.h
    Created: 1 Jul 2017 7:34:50pm
    Author:  Daniel Doubleday

  ==============================================================================
*/

#pragma once

class Plotdata;

using namespace juce;

/**
 * Installs a user-defined unary function of x as f(x)
 * example: Plotdata x(-6, 6);
 *			f(x) = sinc;	// if sinc is a function defined by the user.
 *			plot(x, f(x));	// Plots the graph of sinc(x) from -6 to 6
 */
func_ref  f(Plotdata &x);

/**
 * Installs a user-defined binary function of x as f2(x)
 * example: Plotdata x(-270, 270);
 *			f2(x) = tanLessThan; // tanLessThan is a user-defined binary function.
 */
binfunc_ref f2(Plotdata &x);

/**
 * Calculates a user-defined binary function of x as y = f2(x, double_value)
 * example: Plotdata x(-270, 270);
 *			f2(x) = tanLessThan; //  tanLessThan -user-defined binary function.
 *			plot(x, f2(x, 10));	 // Plots the graph of tanLessThan(x, 10)
 */
Plotdata f2(const Plotdata &x, double op2);

/**
 * Adds next point to plotting data
 * -You add x and y coordinates in 2 separate Plotdatas then call plot(). 
 * plot() will draw the curve joining all the data points. 
 * 
 * @param pd  the Plotdata to add the point to
 * @param val value of the point to add
 */

void point(Plotdata & pd, double val);

/**
 * Adds next point to plotting data
 *  
 * Same as above, but will take both axes and coordinates in one call. 
 * 
 * @param xAxis the x Plotdata to add the point to
 * @param xCoord value of the x coordinate to add
 * @param yAxis the y Plotdata to add the point to
 * @param yCoord value of the y coordinate to add
 */
void point(Plotdata &xAxis, Plotdata &yAxis,
           double xCoord, double yCoord);

/**
 * Insert array of data points into plotting data
 *  
 * @param axis the  Plotdata to add the points to
 * @param array The array of points to insert
 * @param numToInsert the number of points to add from the array.
 */
void insert(Plotdata & axis, const double array[], int numToInsert);

/** Sets the bottom left corner of the graph axes 
  * xmin and ymin should be less than or equal to any coordinate
  * on the graph.
  */ 
void axesBotLeft(Plotdata &x, Plotdata &y, 
                 double xmin, double ymin);

/** Sets the top right corner of the graph axes
  * xmax and ymax should be larger than or equal to any coordinate
  * on the graph.
  */ 
void axesTopRight(Plotdata &x, Plotdata &y, double xmax, double ymax);

/** Adds visible mark to plot, centered on coordinates xpos, ypos */
void addMark(Plotdata &x, Plotdata &y, double xpos, double ypos);

/** Adds visible mark at  coordinates xpos, ypos, specify colour */
void addMark(Plotdata &x, Plotdata &y, double xpos, double ypos, Colour colour);

/** Set colour of graph to requested colour */
void setColor(Plotdata &x, Plotdata &y, Colour colour);

/** Reset colour of graph to last colour before change */
void resetColor(Plotdata &x, Plotdata &y);

/**
 * Clear previous data from an axis.
 * @param the axis to clear data from
 */
void clear(Plotdata & pd);

/**
 * Breaks the current plot data at this point.
 *
 * Later, when plotting, the last point of the data previously 
 * entered will not be joined to the first point of the next data.
 * This allows plotting more than one function on a single graph
 * using the "point" method of data entry.
 * @param xAxis the x data to break
 * @param yAxis the y data to break
 */
 void breakplot(Plotdata &x, Plotdata &y);

/**
 * Print plotting data on axis "anAxis" to standard output (for debugging)
 */
void printPlotdata(const Plotdata & anAxis);


/**********************************************************************/
/*________ Maths functions that may be used to define functions ______*/

/*
 * Return new data, the sine of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata sin(const Plotdata & pd);

/**
 * Return new data, the cosine of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata cos(const Plotdata & pd);

/**
 * Return new data, the tan of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata tan(const Plotdata & pd);

/**
 * Return new data, the asin of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata asin(const Plotdata & pd);

/**
 * Return new data, the acos of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata acos(const Plotdata & pd);


/**
 * Return new data, the atan of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata atan(const Plotdata & pd);

/**
 * Return new data, the hyperbolic sine of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata sinh(const Plotdata & pd);

/**
 * Return new data, the hyperbolic cosine of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata cosh(const Plotdata & pd);

/**
 * Return new data, the hyperbolic tan of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata tanh(const Plotdata & pd);

/**
 * Return new data, the square root of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata sqrt(const Plotdata & pd);

/**
 * Return new data, the absolute value of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata fabs(const Plotdata & pd);

/**
 * Return new data, the natural logarithm of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata log(const Plotdata & pd);

/**
 * Return new data, the log (base 10)l of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata log10(const Plotdata & pd);

/**
 * Return new data, the exponential of the original data
 *
 * @param pd the original Plotdata
 */
Plotdata exp(const Plotdata & pd);

/**
 * Return new data, the power "exp" of the original data
 *
 * @param pd  the original Plotdata
 * @param exp value of the exponent to raise the data to.
 */
Plotdata pow(const Plotdata & pd, double exp);
