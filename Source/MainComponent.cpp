/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

namespace plot = aot::plot;

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize(800, 600);
    
    auto expression = plot::sin(plot::x);
    _plotComponent = new plot::PlotComponent();
    _plotComponent->addPlotData(expression, Colours::blue, "");

    plot::PlotSamples samples;
    samples.pushBack(-2, 0.8);
    samples.pushBack(-1, -0.5);
    samples.pushBack(-0, 0.4);
    samples.pushBack(0.5, 0);
    _plotComponent->addPlotData(samples, Colours::red, "");
    
    _plotComponent->setSize(getWidth(), getHeight());
    _plotComponent->setPlotRange(-3, 3, -1, 1);
//    plotComponent_->setPlotRange(-0.00005, 0.00005, -1, 1);
    addAndMakeVisible(_plotComponent.get());
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white);
}

void MainContentComponent::resized()
{
    if (_plotComponent)
    {
        _plotComponent->setSize(getWidth(), getHeight());
    }

}
