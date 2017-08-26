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
    plotComponent_->setSize(800, 600);
    plotComponent_->setPlotRange(-3, 3, -1, 1);
    addAndMakeVisible(plotComponent_);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    DBG("paint main");
    g.fillAll(Colours::white);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
