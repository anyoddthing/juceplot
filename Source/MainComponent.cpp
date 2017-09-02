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
    plotComponent_->setSize(getWidth(), getHeight());
    plotComponent_->setPlotRange(-3, 3, -1, 1);

    addAndMakeVisible(plotComponent_.get());
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
    if (plotComponent_)
    {
        DBG("resized: w:" << getWidth() << " height:" << getHeight());
        auto widthZoom = (double) plotComponent_->getWidth() / getWidth();
        auto heightZoom = (double) plotComponent_->getHeight() / getHeight();
        
        plotComponent_->zoom(widthZoom, heightZoom);
        plotComponent_->setSize(getWidth(), getHeight());
    }

}
