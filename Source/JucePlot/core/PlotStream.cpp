using namespace juce;

// Constants used exclusively in this file
static const int border_height	= 20;
static const int border_width	= 20;
static const int left_border	= 70;
static const int mark_length	= 4;

static const float chouia		= 0.0001; // an invisible difference (in drawing terms)

double frexp10(double arg, int& exp)
{
    if (arg == 0)
    {
        exp = 0;
        return 0;
    }
    
    auto fexp = std::log10(std::abs(arg));
    
    exp = (fexp > 0) ? (int)fexp : -std::ceil(-fexp);
    return arg * pow(10 , -exp);
}

static String dtoa(double val, int precision, double rounding = 0)
{
    std::ostringstream ostr;
    ostr.precision(precision);
    if (rounding > 0)
    {
        int exp;
        frexp10(rounding, exp);
        auto norm = std::pow(10, std::abs(exp));
        val = std::lround(val * norm) / norm;
    }
    ostr << val;
    return String(ostr.str().c_str());
}

/************************* CLASS FUNCTIONS ***************************/

struct PlotStream::Impl
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

    void setSize(int width, int height)
    {
        winWidth = width;
        winHeight = height;
        
        updatePlotRange();
    }
    
    void setPlotRange(double loX, double hiX, double loY, double hiY)
    {
        plotRange_ = PlotRange(loX, hiX, loY, hiY);
        updatePlotRange();
    }
    
    void updatePlotRange()
    {
        plotWidth = winWidth - border_width - left_border;
        plotHeight = winHeight - 2 * border_width;
    }
    
    void addPlotData(Expression expr, juce::Colour colour = juce::Colours::transparentBlack, juce::String name = juce::String::empty)
    {
        plotData_.emplace_back(expr, name, colour);
    }
    
private:
    
    bool almostEqual(double x, double y)
    {
        return std::abs(x-y) < std::numeric_limits<float>::epsilon() * std::abs(x+y)
               || std::abs(x-y) < std::numeric_limits<float>::min();
    }
    
    /* Convert graph x value to screen coordinate */
    float X(double x, double x_scale) const
    {
        return (x - plotRange_.loX) / x_scale + left_border;
    }
    
    /* Convert graph y value to screen coordinate */
    float Y(double y, double y_scale) const
    {
        return winHeight - border_height - (y - plotRange_.loY) / y_scale;
    }
    
    /* Convert screen coordinate to graph x value */
//    double plotX(int screenX) const
//    {
//        return (screenX - left_border) * x_scale + loX;
//    }
//    
//    /* Convert screen coordinate to graph y value */
//    double plotY(int screenY) const
//    {
//        return (winHeight - border_height - screenY) * y_scale + loY;
//    }
    
    /* @return  true if given location is within x and y axes ranges */
//    bool withinRange( double x, double y) const
//    {
//        return x + x_scale >= loX && x - x_scale <= hiX
//            && y + y_scale >= loY && y - y_scale <= hiY;
//    }
    
    void drawFunc(juce::Graphics& graphics, const PlotData& data)
    {
        graphics.setColour(data.colour);
        
        auto incr = plotRange_.getIncrStep();
        auto& expr = data.expr;
        
        auto x_range = plotRange_.getXRange();
        auto y_range = plotRange_.getYRange();

        auto x_scale = x_range / plotWidth;
        auto y_scale = y_range / plotHeight;

        auto loX = plotRange_.loX;
        
        Point<float> start(X(loX, x_scale), Y(expr[loX], y_scale));
        
        for (auto x = loX + incr; x <= plotRange_.hiX; x += incr)
        {
            auto y = expr[x];
            auto nextPoint = Point<float>(X(x, x_scale), Y(y, y_scale));
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
//    void drawSinglePoint(juce::Graphics& graphics, double xCoord, double yCoord)
//    {
//        drawPointShape(graphics, X(xCoord) + 1, Y(yCoord));
//    }
    
    
    void getAxisSteps(double min, double max, int maxDivs, double& start, double& step)
    {
        auto range = (max - min);
        
        step = range / maxDivs;
        auto p = std::ceil(-std::log10(step) - 1);
        auto normalisedStep = step * std::pow(10, p);
        
        step = (normalisedStep > 0.5 ? 1 : 0.5) * std::pow(10, -p);
        start = min - std::fmod(min, step);
    }
    
    /* Draws the axes */
    void drawAxes(juce::Graphics& graphics)
    {
        // draw the rectangle
        graphics.setColour(Colours::darkgrey);
        graphics.drawRect(
            left_border - 1, border_height - 1,
            plotWidth + 2, plotHeight + 2);

        Font font;
        font.setTypefaceName(Font::getDefaultSansSerifFontName());
        graphics.setFont(font);
        auto fontHeight = graphics.getCurrentFont().getAscent();

        // draw the grid
        float dashPattern[] = {4, 4};
        
        const int minDivWidth = 50;
        auto maxXDivs = plotWidth / minDivWidth;
        
        double xStart, xStep;
        getAxisSteps(plotRange_.loX, plotRange_.hiX, maxXDivs, xStart, xStep);
        auto xScale = plotWidth / plotRange_.getXRange();

        for (auto x = xStart; x <= plotRange_.hiX; x += xStep)
        {
            auto xOffset = x * xScale + left_border - xStart * xScale + (xStart - plotRange_.loX) * xScale;
            if (! (almostEqual(x, plotRange_.loX) || almostEqual(x, plotRange_.hiX)))
            {
                graphics.setColour(Colours::lightgrey);
                graphics.drawDashedLine(Line<float>(xOffset, border_height,
                                                    xOffset, winHeight - border_height),
                                        dashPattern, 2);
                
                graphics.setColour(Colours::darkgrey);
                graphics.drawLine(xOffset, winHeight - border_height,
                                  xOffset, winHeight - border_height - mark_length);
                graphics.drawLine(xOffset, border_height,
                                  xOffset, border_height + mark_length);
            }
            auto asciival = dtoa(x, 2, xStep);
            int ypos = winHeight - border_height / 2 + 5;
            graphics.drawSingleLineText(asciival, xOffset, ypos, Justification::horizontallyCentred);
        }
        

        double yStart, yStep;
        auto maxYDivs = plotHeight / minDivWidth;
        getAxisSteps(plotRange_.loY, plotRange_.hiY, maxYDivs, yStart, yStep);
        auto yScale = plotHeight / plotRange_.getYRange();
        
        for (auto y = yStart; y <= plotRange_.hiY; y += yStep)
        {
            auto yOffset = -y * yScale + border_height + plotHeight + yStart * yScale;
            if (! (almostEqual(y, plotRange_.loY) || almostEqual(y, plotRange_.hiY)))
            {
                graphics.setColour(Colours::lightgrey);
                graphics.drawDashedLine(Line<float>(left_border, yOffset,
                                                    winWidth - border_width, yOffset),
                                        dashPattern, 2);
                
                graphics.setColour(Colours::darkgrey);
                graphics.drawLine(left_border, yOffset,
                                  left_border + mark_length, yOffset);
                graphics.drawLine(winWidth - border_width, yOffset,
                                  winWidth - border_width - mark_length, yOffset);
            }
            
            int xpos = left_border - 3;
            auto asciival = dtoa(y, 2, yStep);
            graphics.drawSingleLineText(asciival, xpos, yOffset + fontHeight / 2 - 1, Justification::right);
        }
    }

private:
    
    int winWidth, winHeight;
    int plotWidth, plotHeight;
    
    //	char * winTitle;
    std::vector<PlotData> plotData_;
    PlotRange plotRange_;
    
    juce::Colour colour;                     // Current drawing colour
    juce::Colour lastColour;                 // Previous drawing colour
};

PlotStream::PlotStream() : impl_(new Impl{}, [](Impl* impl) { delete impl; })
{
    
}

void PlotStream::setWindow(int width, int height)
{
    
    impl_->setSize(width, height);
}

void PlotStream::setPlotRange(double loX, double hiX, double loY, double hiY)
{
    impl_->setPlotRange(loX, hiX, loY, hiY);
}

void PlotStream::addPlotData(Expression expr, juce::Colour colour, juce::String name)
{
    impl_->addPlotData(expr, colour, name);
}

void PlotStream::plot(juce::Graphics& graphics)
{
    impl_->plot(graphics);
}




