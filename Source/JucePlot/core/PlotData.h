#pragma once

#include <cmath>
#include <vector>
#include <iostream>

// Unary function pointer type accepted by Plotdata (double parameter)
typedef double (*Func)(double operand);
#define func_ref Func &

// Binary function pointer type accepted by Plotdata
typedef double (*BinFunc)(double operand1, double operand2);
#define binfunc_ref BinFunc &

/** Define data iterator */
typedef std::vector<double>::const_iterator  dataIterator;

/** Number of points in a plot */
enum Grain
{
	EXTRA_FINE = 901,
	FINE = 501,
	MEDIUM = 301,
	COARSE = 201,
	GROSS = 101
};

struct PlotSampler
{
    
};

struct PlotData
{
    PlotData(Expression expr, juce::String name, juce::Colour colour) : expr(expr), name(name), colour(colour)
    {}
    
    Expression expr;
    juce::String name;
    juce::Colour colour;
    
};

