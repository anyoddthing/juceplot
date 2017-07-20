/*
  ==============================================================================

    PlotComponent.h
    Created: 1 Jul 2017 7:19:45pm
    Author:  Daniel Doubleday

  ==============================================================================
*/

#pragma once

using namespace juce;

//==============================================================================
/*
*/
class PlotComponent : public Component
{
public:
    PlotComponent(Plotdata x, Plotdata y) : _x(std::move(x)), _y(std::move(y))
    {
    }

    ~PlotComponent()
    {
    }

    void paint(Graphics& g) override
    {
        DBG("paint");
        Plotstream pl(getWidth(), getHeight());
        pl.plot(g, _x, _y);
    }

    void resized() override
    {
    }

private:
    const Plotdata _x, _y;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};
