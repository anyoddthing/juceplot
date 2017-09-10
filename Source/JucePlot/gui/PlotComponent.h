#pragma once

class PlotComponent : public juce::Component
{
public:
    PlotComponent()
    {
    }
    
    ~PlotComponent()
    {
        DBG("PlotComponent dtor");
    }
    
    void setPlotRange(double loX, double hiX, double loY, double hiY)
    {
        DBG("PlotComponent setPlotRange");
        plotstream_.setPlotRange(loX, hiX, loY, hiY);
    }

    void addPlotData(Expression expr, juce::Colour colour, juce::String name)
    {
        DBG("PlotComponent addPlotData");
        plotstream_.addPlotData(std::move(expr), colour, name);
    }
    
    void zoom(double widthZoom, double heightZoom)
    {
        
    }
    
    void paint(juce::Graphics& g) override
    {
        plotstream_.plot(g);
    }
    
    void resized() override
    {
        plotstream_.setWindow(getWidth(), getHeight());
    }

private:
    PlotStream plotstream_;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};
