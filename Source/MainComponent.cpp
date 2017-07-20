/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "JucePlot/JucePlot.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize(800, 600);
    Plotdata x(-4.0, 4.0), y = sin(x) - 0.5*x;
    plotComponent_ = new PlotComponent(std::move(x), std::move(y));
    plotComponent_->setSize(800, 600);
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
