#pragma once

#include "PlotData.h"

enum Rounding
{
	DOWN,
	ANY,
	UP
};

class Plotstream
{
public:
	Plotstream(int width = 640, int height = 320);
	void plot(juce::Graphics& graphics, const Plotdata& x, const Plotdata& y, juce::Colour colour = juce::Colours::green);

private:
	int winWidth, winHeight;
	int plotWidth, plotHeight;
	char * winTitle;
	double lo_x;	double hi_x; 	// Highest and lowest data values in x
	double lo_y;	double hi_y;	// Highest and lowest points on y axis
	double x_range; double y_range; // Ranges of x values and y values
	double x_scale;	double y_scale; // Scales of graph drawing to screen pixels
	bool plotStarted;				// True while plotting is going on
	bool marked; 					// True when a marker is visible
	int lastX;
	int lastY; 						// Location of last marker drawn
    juce::Colour colour;                     // Current drawing colour
    juce::Colour lastColour;                 // Previous drawing colour
    
	/* Convert graph x value to screen coordinate */
	float X(double x) const;
	/* Convert graph y value to screen coordinate */
	float Y(double y) const;
	/* Convert screen coordinate to graph x value */
	double plotX(int screenX) const;
	/* Convert screen coordinate to graph y value */
	double plotY(int screenY) const;
	/* Round(est) double within one pixel in X */
	Rounding nearRoundX(double & val, Rounding direction = ANY) const;
	/* Round(est) double within one pixel in Y */
	Rounding nearRoundY(double & val, Rounding direction = ANY) const;
	   /* True if values given are within x and y graph ranges */
	bool withinRange(double xVal, double yVal) const;
	/* Draws the axes */
	void drawAxes(juce::Graphics& graphics);
	/* Draw the data */
	void drawFunc(juce::Graphics& graphics, const Plotdata & x, const Plotdata & y);
	/* Watches the mouse and prints cursor coords if clicked */
	bool watchMouse();
	// Draw a marker at the current cursor position. Erase if erase is true
	void drawMarker(juce::Graphics& graphics, bool erase = false);
    // Draw a single point at the requested coordinates
    void drawSinglePoint(juce::Graphics& graphics, double xCoord, double yCoord);
    // Set new foreground colour, store last colour
    void setFgColor(juce::Graphics& graphics, juce::Colour fgColour);
    // Reset foreground colour to last colour
    void resetFgColor(juce::Graphics& graphics);
    // Check whether the stream holds a command at this point (colour change
    // or single point), and handle the command if it does.
    void handleCommand(juce::Graphics& graphics, dataIterator &x_it, dataIterator &y_it);
    // Draw a marker 
    void drawMarkShape(juce::Graphics& graphics, int x, int y);
    // Draw a poing
    void drawPointShape(juce::Graphics& graphics, int x, int y);


};

