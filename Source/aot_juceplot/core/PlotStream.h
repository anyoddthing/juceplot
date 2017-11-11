#pragma once

enum Rounding
{
	DOWN,
	ANY,
	UP
};

class PlotStream
{
public:
    PlotStream();

    void setWindow(int width, int height);
    
    void setPlotRange(PlotRange plotRange);
    PlotRange getPlotRange();
	
    /* Convert graph x value to screen coordinate */
    float screenX(double x) const;
    
    /* Convert graph y value to screen coordinate */
    float screenY(double y) const;
    
    /* Convert screen coordinate to graph x value */
    double plotX(float screenX) const;
    
    /* Convert screen coordinate to graph y value */
    double plotY(float screenY) const;

    void addPlotData(Expression expr, juce::Colour colour = juce::Colours::transparentBlack, juce::String name = juce::String::empty);
    
    void plot(juce::Graphics& graphics);

private:
    
    class Impl;
    std::unique_ptr<Impl, void (*)(Impl *)> _impl;
};

