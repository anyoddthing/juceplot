/* File: Plotstream.cxx
 *
 * Implementation class Plotstream
 * A plotstream opens a window displays a data plot, then closes
 * the window.
 *
 * left-clicking the mouse in the window displays the current plot coordinates of
 * the cursor. 
 *
 * This file hides all the fiddly bits about plotting a function.
 *
 * Author: 	jlk
 * Version:	1.2
 * Date:	January 2006
 *
 * This file is in the public domain and can be used without
 * any restriction.
 */

using namespace juce;
using namespace std;

// Constants used exclusively in this file
static const int border_height	= 20;
static const int border_width	= 20;
static const int left_border	= 70;
static const int mark_length	= 4;

static const Colour graph_color = Colours::green; // Default only, can be changed
static const Colour point_color = graph_color; // will be XORed
static const Colour marker_color	= Colours::cyan; // Will give red in XOR mode over white
static const float chouia		= 0.0001; // an invisible difference (in drawing terms)

// Local function hidden at file scope
// dtoa safely converts a double to the equivalent char *
// It is the responsibility of the calling program to delete the returned string
static String dtoa(double val, int precision)
{
    ostringstream ostr;
	ostr.precision(precision);
	ostr << val;
	return String(ostr.str().c_str());
}

// Local function hidden at file scope
// pow10l() does not exist in mingw (redefine as inline wrapper here)
static inline double pow10l(double x)
{
	return powl(10.0, x);
}

/* Local function hidden at file scope
 * Rounds double value by discarding very small decimals.
 * (necessary to deal with previous small floating point errors)
 * @param val 		value to round
 * @param intrep    all significant digits as a long int
 * @return			truncated value as a double (may be inexact)
 */
static double floatRound(double val, long int &intrep)
{ 
	bool neg = false;
	string::size_type i, j;
	int leading0s = 0;
	ostringstream ostr;
	double res;
	
	ostr.setf(ios::floatfield);
	ostr.unsetf(ios::showpoint);
	ostr << std::fixed << val;
	
	string str = ostr.str();

	// Remove negative sign if any
	if (str[0] == '-')
	{
		neg = true;
		str = str.substr(1);
	}
	   
	// find position of dot if any
	auto decPos = str.find('.');
	if (decPos == string::npos) // then it's an integer, return now
	{
		intrep = static_cast<long int>(val);
		return val;
	}
    
    
	// find the 1st non 0 digit
	for(i = 0; i < str.size() && (str[i] == '0' || str[i] == '.'); i++);
	// cut the string there
	if (i > 0)
	{
		str = str.substr(i);
		leading0s = i - 1;
	}
		
	// remove decimal point if still there
	i = str.find('.');
	if (i != string::npos)
	{
		string head = str.substr(0, i);
		str = head.append(str.substr(i + 1));
	}
	
	// find first incidence of "000" or "999" if any in string
	i = str.find("000");
	j = str.find("999");
	// if i < j, then truncate at that point
	if (i != string::npos)
	{
		if (j == string::npos || i < j)
			str = str.substr(0, i);
	}
	// else round the number: Insert '.' before the 1st 9
	// then round up to an integer
	else if (j != string::npos)
	{
		double roundnum;
		
		str = str.insert(j, 1, '.');
		istringstream istr(str);
		istr >> roundnum;
		roundnum = round(roundnum);
		
		ostr.str(""); // Empty the ostringstream
		str = ostr.str();
		ostr << int(roundnum); // insert number as integer
		str = ostr.str();
		
		if (j == 0) // the decimal point has moved bcos of rounding up
			decPos++;
	}

	while((unsigned)decPos > str.size()) // The rounding up occurred to the
		str += '0'; 		   // left of the decimal point. Add 0s
			
	{
		
		istringstream istr(str);
		istr >> intrep;
	}
	
	// re-insert the leading 0s, decimal point & sign
	if(leading0s > 0)
		str.insert(0, leading0s, '0');
	   str.insert(decPos, 1, '.');
	if(neg)
		str.insert(0, 1, '-');
	istringstream istr(str);
	
	istr >> res;

	return res;
}	 

/*
 * Local function hidden at file scope
 * Attempt to guess a convenient divisor for grid along the X axis 
 * If 0 is in the range, it should in many cases be on the grid.
 * Otherwise, divide the x range by 10, or 6, or 7, or 5, or 8, or 3,
 * or 4, in this order if divisible, by 10 otherwise.
 */
static int getXDivisor(double lo, double hi, int plotWidth)
{
	int pquo, div = 0;
	long int intVal;
    double delta = (hi - lo) / plotWidth; // Ignore 1 pixel inaccuracies
    double range = hi - lo;

	floatRound(range, intVal);
    
	// If 0 is on the axis attempt to place it on the grid
	if(lo < 0 && hi > 0) // 0 is part of the range
	{
		if(fabsl(lo) < hi)
			div = int(round(hi / fabsl(lo)));
		else
			div = int(round(fabs(lo) / hi));
			
		if (div > 0 && div <= 9)
        {
			div += 1; 
            
            // If too small number of divisions, try multiples
            if(div == 2) // See if divisible by 12, 10, 6, 8, or 4
            {            
        		if (remquol(intVal, 12, & pquo) < delta)
        			div = 12;
        	   	else if (remquol(intVal, 10, & pquo) < delta)
        			div = 10;
        		else if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
                else if (remquol(intVal, 8, & pquo) < delta)
        			div = 8;
        		else if (remquol(intVal, 4, & pquo) < delta)
        			div = 4;
            }
            else if (div == 3) // See if divisible by 12, 9 or 6
            {
        		if (remquol(intVal, 12, & pquo) < delta)
        			div = 12;
        	   	else if (remquol(intVal, 9, & pquo) < delta)
        			div = 9;
        		else if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
            }
            return div;
        }             	          	     	    	
	}
	
	// If not, then attempt a reasonable division of the range.
	// Try dividing by  12, or 10, or 5, or 6, or 8,  or 3, or 4	
	else
	{
		if (remquol(intVal, 12, & pquo) < delta)
			div = 12;
	   	else if (remquol(intVal, 10, & pquo) < delta)
			div = 10;
		else if (remquol(intVal, 7, & pquo) < delta)
			div = 7;
    	else if (remquol(intVal, 6, & pquo) < delta)
			div = 6;
	    else if (remquol(intVal, 5, & pquo) < delta)
			div = 5;
		else if (remquol(intVal, 8, & pquo) < delta)
			div = 8;
		else if (remquol(intVal, 3, & pquo) < delta)
			div = 3;
		else if (remquol(intVal, 4, & pquo) < delta)
			div = 4;
		else // We are running out of common divisors
			div = 10;
	}
	return div;
}

/*
 * Local function hidden at file scope
 * Attempt to guess a convenient divisor for grid along the Y axis 
 * If 0 is in the range, it should in many cases be on the grid.
 * Otherwise, divide the y range by 5 or 3 or 4 in this order if
 * divisible, by 5 otherwise.
 */
static int getYDivisor(double lo, double hi, int plotHeight)
{
	int pquo, div = 0;
	long int intVal = 0;
    double delta = (hi - lo) / plotHeight; // Ignore 1 pixel inaccuracies

	// If 0 is on the axis attempt to place it on the grid
	if(lo < 0 && hi > 0) // 0 is part of the range
	{
		if(fabsl(lo) < hi)
			div = int(round(hi / fabsl(lo)));
		else
			div = int(round(fabs(lo) / hi));
			
		if (div > 0 && div < 6)
        {
			div += 1; 
            
            // If too small number of divisions, try multiples
            if(div == 2) // See if divisible by 6, 4
            {            
        		if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
        		else if (remquol(intVal, 4, & pquo) < delta)
        			div = 4;
             }
            else if (div == 3) // See if divisible 6
            {
        		if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
            }
            return div;
        }             	          	     	    	
	}

	
	// if not, then attempt a reasonable division of the range 
	double range = hi - lo;
	floatRound(range, intVal);
	   
	// If 0 is in the middle try dividing by 4	  
	if (div == 1)
	{
		if (remquol(intVal, 4, & pquo) < delta)
			div = 4;
		else // We are running out of common divisors
			div = 2;
	}
	// else try dividing by  3, or 4, or 5, or 2	
	else
	{
		if (remquol(intVal, 5, & pquo) < delta)
			div = 5;
	   	if (remquol(intVal, 3, & pquo) < delta)
			div = 3;
		else if (remquol(intVal, 4, & pquo) < delta)
			div = 4;
		else if (remquol(intVal, 2, & pquo) < delta)
			div = 2;
	   	else // We are running out of common divisors
			div = 5;
	}
	return div;
}

/************************* CLASS FUNCTIONS ***************************/

struct Plotstream::Impl
{
    void plot(Graphics& graphics)
    {
        drawAxes(graphics);
        
        /* Draw curve */
        for (auto& data : plotData_)
        {
            drawFunc(graphics, data);
        }
    }

    void setWindow(int width, int height)
    {
        winWidth = width;
        winHeight = height;
    }
    
    void setPlotRange(double loX, double hiX, double loY, double hiY)
    {
        this->loX = loX;
        this->hiX = hiX;
        this->loY = loY;
        this->hiY = hiY;
        
        x_range = hiX - loX;
        y_range = hiY - loY;
        
        plotWidth = winWidth - border_width - left_border;
        plotHeight = winHeight - 2 * border_width;
        
        x_scale = x_range / plotWidth;
        y_scale = y_range / plotHeight;
    }
    
    void addPlotData(Expression expr, juce::Colour colour = juce::Colours::transparentBlack, juce::String name = juce::String::empty)
    {
        plotData_.emplace_back(expr, name, colour);
    }
    
private:
    
    /* Convert graph x value to screen coordinate */
    float X(double x) const
    {
        return (x - loX) / x_scale + left_border;
    }
    
    /* Convert graph y value to screen coordinate */
    float Y(double y) const
    {
        return winHeight - border_height - (y - loY) / y_scale;
    }
    
    /* Convert screen coordinate to graph x value */
    double plotX(int screenX) const
    {
        return (screenX - left_border) * x_scale + loX;
    }
    
    /* Convert screen coordinate to graph y value */
    double plotY(int screenY) const
    {
        return (winHeight - border_height - screenY) * y_scale + loY;
    }
    
    /* @return  true if given location is within x and y axes ranges */
    bool withinRange( double x, double y) const
    {
        return	x + x_scale >= loX && x - x_scale <= hiX
        &&
        y + y_scale >= loY && y - y_scale <= hiY;
    }
    
    void drawFunc(juce::Graphics& graphics, const PlotData& data)
    {
        graphics.setColour(data.colour);
        
        auto incr = (hiX - loX) / Grain::MEDIUM;
        auto& expr = data.expr;
        
        Point<float> start(X(loX), Y(expr[loX]));
        
        for (auto x = loX + incr; x <= hiX; x += incr)
        {
            auto y = expr[x];
            auto nextPoint = Point<float>(X(x), Y(y));
            graphics.drawLine(start.getX(), start.getY(), nextPoint.getX(), nextPoint.getY());
            start = nextPoint;
        }
    }
    
    
    // Ancillary function used locally by drawSinglePoint()
    // draws the point shape in X and Y.
    void drawPointShape(juce::Graphics& graphics, int x, int y)
    {
        graphics.fillRect(x - 2, y - 2, 5, 5);
    }
    
    // Draw a single point at the given coordinates
    void drawSinglePoint(juce::Graphics& graphics, double xCoord, double yCoord)
    {
        drawPointShape(graphics, X(xCoord) + 1, Y(yCoord));
    }
    
    /* Draws the axes */
    void drawAxes(juce::Graphics& graphics)
    {
        int xDivs;				// number of x divisions
        int yDivs;				// number of y divisions
        int divLength;	 	 	// length (in pixels) of a division
        int sigdigits;
        long int intVal;
        
        // draw the rectangle
        graphics.setColour(Colours::darkgrey);
        graphics.drawRect(
                          left_border - 1,
                          border_height - 1,
                          plotWidth + 2,
                          plotHeight + 2
                          );
        
        // Attempt to guess a reasonable number of grid divisions for x and y
        xDivs = getXDivisor(loX, hiX, plotWidth);
        // If y axis is large, divide in the same manner as x axis
        if (winHeight > winWidth * 3 / 5.0)
            yDivs = getXDivisor(loY, hiY, plotHeight);
        else
            yDivs = getYDivisor(loY, hiY, plotHeight);
        
        // draw the grid
        graphics.setColour(Colours::lightgrey);
        float dashPattern[] = {4, 4};
        // Horizontal grid
        for (int i = yDivs - 1; i > 0; i--)
        {
            graphics.drawDashedLine(Line<float>(
                                                left_border, border_height + plotHeight * i / yDivs,
                                                winWidth - border_width, border_height + plotHeight * i / yDivs),
                                    dashPattern, 2
                                    );
        }
        
        // Vertical grid
        for (int i = xDivs - 1; i > 0; i--)
        {
            graphics.drawDashedLine(Line<float>(
                                                left_border + 1 + plotWidth * i / xDivs, border_height,
                                                left_border + 1 + plotWidth * i / xDivs, winHeight - border_height),
                                    dashPattern, 2
                                    );
        }
        
        // Draw Axes markers
        graphics.setColour(Colours::darkgrey);
        
        // Y axis
        for (int i = yDivs - 1; i > 0; i--)
        {
            graphics.drawLine(
                              left_border, border_height + plotHeight * i / yDivs,
                              left_border + mark_length, border_height + plotHeight * i / yDivs
                              );
            graphics.drawLine(
                              winWidth - border_width, border_height + plotHeight * i / yDivs,
                              winWidth - border_width - mark_length, border_height + plotHeight * i / yDivs
                              );
        }
        
        // X axis
        for (int i = xDivs - 1; i > 0; i--)
        {
            graphics.drawLine(
                              left_border + 1 + plotWidth * i / xDivs, winHeight - border_height,
                              left_border + 1 + plotWidth * i / xDivs, winHeight - border_height - mark_length
                              );
            graphics.drawLine(
                              left_border + 1 + plotWidth * i / xDivs, border_height,
                              left_border + 1 + plotWidth * i / xDivs, border_height + mark_length
                              );
        }
        
        // Number the axes
        graphics.setColour(Colours::black);
        
        // Y axis
        Font font;
        font.setTypefaceName(Font::getDefaultSansSerifFontName());
        graphics.setFont(font);
        auto fontHeight = graphics.getCurrentFont().getHeight();
        
        divLength = int(plotHeight / yDivs);
        int xpos = left_border - 3;
        int ypos = border_height;
        double divVal = floatRound((loY - hiY) / yDivs, intVal);
        
        for (int i = 0;  i <= yDivs; i++)
        {
            double val = floatRound(hiY + divVal * i, intVal);
            if(fabs(val) < chouia * (hiY - loY))
                val = 0;
            auto asciival = dtoa(val, 3);
            graphics.drawSingleLineText(asciival, xpos, ypos + divLength * i + fontHeight / 3, Justification::right);
        }
        
        // X axis
        ypos = winHeight - border_height/2 + 5;
        xpos = left_border - 3;
        divVal = floatRound((hiX - loX) / xDivs, intVal);
        for (int i = 0;  i <= xDivs; i++)
        {
            double val = floatRound(loX + divVal * i, intVal);
            if(fabs(val) < chouia * (hiX - loX))
                val = 0;
            auto asciival = dtoa(val, 3);
            graphics.drawSingleLineText(asciival, xpos + plotWidth * i / xDivs, ypos, Justification::horizontallyCentred);
        }
    }
    
private:
    
    int winWidth, winHeight;
    int plotWidth, plotHeight;
    
    //	char * winTitle;
    std::vector<PlotData> plotData_;
    
    double loX;	double hiX; 	// Highest and lowest data values in x
    double loY;	double hiY;	// Highest and lowest points on y axis
    double x_range; double y_range; // Ranges of x values and y values
    double x_scale;	double y_scale; // Scales of graph drawing to screen pixels
    
    juce::Colour colour;                     // Current drawing colour
    juce::Colour lastColour;                 // Previous drawing colour
};

Plotstream::Plotstream() : impl_(new Impl{}, [](Impl* impl) { delete impl; })
{
    
}

void Plotstream::setWindow(int width, int height)
{
    impl_->setWindow(width, height);
}

void Plotstream::setPlotRange(double loX, double hiX, double loY, double hiY)
{
    impl_->setPlotRange(loX, hiX, loY, hiY);
}

void Plotstream::addPlotData(Expression expr, juce::Colour colour, juce::String name)
{
    impl_->addPlotData(expr, colour, name);
}

void Plotstream::plot(juce::Graphics& graphics)
{
    impl_->plot(graphics);
}




