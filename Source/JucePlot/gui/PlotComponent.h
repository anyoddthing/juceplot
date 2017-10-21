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
        plotstream_.setPlotRange({ loX, hiX, loY, hiY });
    }

    void addPlotData(Expression expr, juce::Colour colour, juce::String name)
    {
        plotstream_.addPlotData(std::move(expr), colour, name);
    }
    
    void paint(juce::Graphics& g) override
    {
        plotstream_.plot(g);
    }
    
    void resized() override
    {
        plotstream_.setWindow(getWidth(), getHeight());
    }

    void zoom(float splitX, float splitY, float zoomX, float zoomY)
    {
        jassert(zoomX > 0);
        jassert(zoomY > 0);
        
        auto plotRange = plotstream_.getPlotRange();
        
        auto xRange = plotRange.getXRange();
        auto midX = plotRange.loX + xRange * splitX;
        
        auto yRange = plotRange.getYRange();
        auto midY = plotRange.loY + yRange * splitY;

        
        plotstream_.setPlotRange({
            midX - (midX - plotRange.loX) * zoomX,
            midX + (plotRange.hiX - midX) * zoomX,
            midY - (midY - plotRange.loY) * zoomY,
            midY + (plotRange.hiY - midY) * zoomY });
        
//        DBG(plotstream_.getPlotRange());
    }
    
    void move(float deltaX, float deltaY)
    {
        auto plotRange = plotstream_.getPlotRange();
        plotstream_.setPlotRange(plotRange.move(deltaX, deltaY));
    }
    
    void mouseWheelMove (const juce::MouseEvent& event,
                         const juce::MouseWheelDetails& wheel) override
    {
        if (wheel.deltaX == 0 && wheel.deltaY == 0)
            return;
        
//        DBG("moue wheel: " << wheel.deltaX << " " << wheel.deltaY);
        auto x = static_cast<float>(event.x) / getWidth();
        auto y = static_cast<float>(event.y) / getHeight();
        zoom(x, y, 1 + wheel.deltaX, 1 + wheel.deltaY);
        repaint();
    }
    
    void mouseDrag(const juce::MouseEvent& event) override
    {
        DBG("mouse drag: " << event.getDistanceFromDragStartX() << " " << event.getDistanceFromDragStartY());
        auto deltaX = plotstream_.plotX(lastDragPoint_.x) - plotstream_.plotX(event.position.x);
        auto deltaY = plotstream_.plotX(lastDragPoint_.y) - plotstream_.plotX(event.position.y);
        lastDragPoint_ = event.position;
        
        move(deltaX, deltaY);
        repaint();
    }
    
    void mouseDown(const juce::MouseEvent& event) override
    {
        DBG("mouse down");
        lastDragPoint_ = event.position;
    }
    
private:
    PlotStream plotstream_;
    juce::Point<float> lastDragPoint_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};
