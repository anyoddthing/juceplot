/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "koolplot/koolplot.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize(800, 600);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white);

    Plotdata x(-4.0, 4.0), y = sin(x) - 0.5*x;
    auto image = plot(x, y);
    g.drawImageTransformed(image, AffineTransform::scale(0.5f));
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
