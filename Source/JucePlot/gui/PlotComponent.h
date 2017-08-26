#pragma once


//==============================================================================
/*
*/
class PlotComponent : public juce::Component
{
public:
    PlotComponent()
    {
    }
    
    void setPlotRange(double loX, double hiX, double loY, double hiY)
    {
        plotstream_.setPlotRange(loX, hiX, loY, hiY);
    }

    void addPlotData(Expression expr, juce::Colour colour, juce::String name)
    {
        plotstream_.addPlotData(std::move(expr), colour, name);
    }
    
    void paint(juce::Graphics& g) override
    {
        DBG("paint");
        plotstream_.plot(g);
    }

    void resized() override
    {
        DBG("resizes");
        plotstream_.setWindow(getWidth(), getHeight());
    }

private:
    Plotstream plotstream_;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};
