using namespace juce;

// Constants used exclusively in this file
static const int BORDER_HEIGHT	= 20;
static const int BORDER_WIDTH	= 20;
static const int LEFT_BORDER	= 70;
static const int MARK_LENGTH	= 4;

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
        for (auto& data : _plotData)
        {
            drawFunc(graphics, data);
        }
    }

    void setSize(int width, int height)
    {
        _winWidth = width;
        _winHeight = height;
        
        updatePlotRange();
    }
    
    void setPlotRange(PlotRange plotRange)
    {
        _plotRange = plotRange;
        updatePlotRange();
    }
    
    PlotRange getPlotRange()
    {
        return _plotRange;
    }
    
    void updatePlotRange()
    {
        _plotWidth = _winWidth - BORDER_WIDTH - LEFT_BORDER;
        _plotHeight = _winHeight - 2 * BORDER_WIDTH;
        _xPlot2Screen = _plotWidth / _plotRange.getXRange();
        _yPlot2Screen = _plotHeight / _plotRange.getYRange();
    }
    
    void addPlotData(Expression expr, juce::Colour colour = juce::Colours::transparentBlack, juce::String name = juce::String::empty)
    {
        _plotData.emplace_back(expr, name, colour);
    }
    
    /* Convert graph x value to screen coordinate */
    float screenX(double x) const
    {
        return (x - _plotRange.loX) * _xPlot2Screen + LEFT_BORDER;
    }
    
    /* Convert graph y value to screen coordinate */
    float screenY(double y) const
    {
        return _winHeight - BORDER_HEIGHT - (y - _plotRange.loY) * _yPlot2Screen;
    }
    
    /* Convert screen coordinate to graph x value */
    double plotX(float screenX) const
    {
        return (screenX - LEFT_BORDER) / _xPlot2Screen + _plotRange.loX;
    }
    
    /* Convert screen coordinate to graph y value */
    double plotY(float screenY) const
    {
        return (_winHeight - BORDER_HEIGHT - screenY) / _yPlot2Screen + _plotRange.loY;
    }
    
private:
    
    bool almostEqual(double x, double y)
    {
        return std::abs(x-y) < std::numeric_limits<float>::epsilon() * std::abs(x+y)
               || std::abs(x-y) < std::numeric_limits<float>::min();
    }
    
    void drawFunc(juce::Graphics& graphics, const PlotData& data)
    {
        graphics.setColour(data.colour);
        
        auto incr = _plotRange.getIncrStep();
        auto& expr = data.expr;
        
        auto loX = _plotRange.loX;
        Point<float> start(screenX(loX), screenY(expr[loX]));
        
        for (auto x = loX + incr; x <= _plotRange.hiX; x += incr)
        {
            auto y = expr[x];
            auto nextPoint = Point<float>(screenX(x), screenY(y));
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
    
    void getAxisSteps(double min, double max, int maxDivs, double& start, double& step)
    {
        auto range = (max - min);
        
        step = range / maxDivs;
        auto p = std::ceil(-std::log10(step) - 1);
        auto normalisedStep = step * std::pow(10, p);
        
        step = (normalisedStep > 0.5 ? 1 : 0.5) * std::pow(10, -p);
        auto remainder = std::fmod(min, step);
        start = min - remainder;
    }
    
    /* Draws the axes */
    void drawAxes(juce::Graphics& graphics)
    {
        // draw the rectangle
        graphics.setColour(Colours::darkgrey);
        graphics.drawRect(
            LEFT_BORDER - 1, BORDER_HEIGHT - 1,
            _plotWidth + 2, _plotHeight + 2);

        Font font;
        font.setTypefaceName(Font::getDefaultSansSerifFontName());
        graphics.setFont(font);
        auto fontHeight = graphics.getCurrentFont().getAscent();

        // draw the grid
        float dashPattern[] = {4, 4};
        
        const int minDivWidth = 50;
        auto maxXDivs = _plotWidth / minDivWidth;
        
        double xStart, xStep;
        getAxisSteps(_plotRange.loX, _plotRange.hiX, maxXDivs, xStart, xStep);

        for (auto x = xStart; x <= _plotRange.hiX; x += xStep)
        {
            auto xOffset = screenX(x);
            if (! (almostEqual(x, _plotRange.loX) || almostEqual(x, _plotRange.hiX)))
            {
                graphics.setColour(Colours::lightgrey);
                graphics.drawDashedLine(Line<float>(xOffset, BORDER_HEIGHT,
                                                    xOffset, _winHeight - BORDER_HEIGHT),
                                        dashPattern, 2);
                
                graphics.setColour(Colours::darkgrey);
                graphics.drawLine(xOffset, _winHeight - BORDER_HEIGHT,
                                  xOffset, _winHeight - BORDER_HEIGHT - MARK_LENGTH);
                graphics.drawLine(xOffset, BORDER_HEIGHT,
                                  xOffset, BORDER_HEIGHT + MARK_LENGTH);
            }
            auto asciival = dtoa(x, 2, xStep);
            int ypos = _winHeight - BORDER_HEIGHT / 2 + 5;
            graphics.drawSingleLineText(asciival, xOffset, ypos, Justification::horizontallyCentred);
        }

        double yStart, yStep;
        auto maxYDivs = _plotHeight / minDivWidth;
        getAxisSteps(_plotRange.loY, _plotRange.hiY, maxYDivs, yStart, yStep);
        for (auto y = yStart; y <= _plotRange.hiY; y += yStep)
        {
            auto yOffset = screenY(y);
            if (! (almostEqual(y, _plotRange.loY) || almostEqual(y, _plotRange.hiY)))
            {
                graphics.setColour(Colours::lightgrey);
                graphics.drawDashedLine(Line<float>(LEFT_BORDER, yOffset,
                                                    _winWidth - BORDER_WIDTH, yOffset),
                                        dashPattern, 2);
                
                graphics.setColour(Colours::darkgrey);
                graphics.drawLine(LEFT_BORDER, yOffset,
                                  LEFT_BORDER + MARK_LENGTH, yOffset);
                graphics.drawLine(_winWidth - BORDER_WIDTH, yOffset,
                                  _winWidth - BORDER_WIDTH - MARK_LENGTH, yOffset);
            }
            
            int xpos = LEFT_BORDER - 3;
            auto asciival = dtoa(y, 2, yStep);
            graphics.drawSingleLineText(asciival, xpos, yOffset + fontHeight / 2 - 1, Justification::right);
        }
    }

private:
    
    int _winWidth, _winHeight;
    int _plotWidth, _plotHeight;
    double _xPlot2Screen, _yPlot2Screen;
    
    std::vector<PlotData> _plotData;
    PlotRange _plotRange;
    
    juce::Colour _colour;
};

PlotStream::PlotStream() : _impl(new Impl{}, [](Impl* impl) { delete impl; })
{
    
}

void PlotStream::setWindow(int width, int height)
{
    _impl->setSize(width, height);
}

void PlotStream::setPlotRange(PlotRange plotRange)
{
    _impl->setPlotRange(plotRange);
}

PlotRange PlotStream::getPlotRange()
{
    return _impl->getPlotRange();
}

void PlotStream::addPlotData(Expression expr, juce::Colour colour, juce::String name)
{
    _impl->addPlotData(expr, colour, name);
}

void PlotStream::plot(juce::Graphics& graphics)
{
    _impl->plot(graphics);
}

/* Convert graph x value to screen coordinate */
float PlotStream::screenX(double x) const
{
    return _impl->screenX(x);
}

/* Convert graph y value to screen coordinate */
float PlotStream::screenY(double y) const
{
    return _impl->screenY(y);
}

/* Convert screen coordinate to graph x value */
double PlotStream::plotX(float screenX) const
{
    return _impl->plotX(screenX);
}

/* Convert screen coordinate to graph y value */
double PlotStream::plotY(float screenY) const
{
    return _impl->plotY(screenY);
}



