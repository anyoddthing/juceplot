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
    plotComponent_ = new plot::PlotComponent();
    plotComponent_->addPlotData(expression, Colours::blue, "");

    plot::PlotSamples samples;
    samples.pushBack(-2, 0.8);
    samples.pushBack(-1, -0.5);
    samples.pushBack(-0, 0.4);
    samples.pushBack(0.5, 0);
    plotComponent_->addPlotData(samples, Colours::red, "");
    
    plotComponent_->setSize(getWidth(), getHeight());
    plotComponent_->setPlotRange(-3, 3, -1, 1);
//    plotComponent_->setPlotRange(-0.00005, 0.00005, -1, 1);
    addAndMakeVisible(plotComponent_.get());
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
    if (plotComponent_)
    {
        plotComponent_->setSize(getWidth(), getHeight());
    }

}
