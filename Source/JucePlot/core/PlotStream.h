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
    void setPlotRange(double loX, double hiX, double loY, double hiY);    
	
    void addPlotData(Expression expr, juce::Colour colour = juce::Colours::transparentBlack, juce::String name = juce::String::empty);
    
    void plot(juce::Graphics& graphics);

private:
    
    class Impl;
    std::unique_ptr<Impl, void (*)(Impl *)> impl_;
};

