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
        _plotstream.setPlotRange({ loX, hiX, loY, hiY });
    }

    void addPlotData(Expression expr, juce::Colour colour, juce::String name)
    {
        _plotstream.addPlotData(std::move(expr), colour, name);
    }
    
    void paint(juce::Graphics& g) override
    {
        _plotstream.plot(g);
    }
    
    void resized() override
    {
        _plotstream.setWindow(getWidth(), getHeight());
    }

    void zoom(float splitX, float splitY, float zoomX, float zoomY)
    {
        jassert(zoomX > 0);
        jassert(zoomY > 0);
        
        auto plotRange = _plotstream.getPlotRange();
        
        auto xRange = plotRange.getXRange();
        auto midX = plotRange.loX + xRange * splitX;
        
        auto yRange = plotRange.getYRange();
        auto midY = plotRange.loY + yRange * splitY;

        _plotstream.setPlotRange({
            midX - (midX - plotRange.loX) * zoomX,
            midX + (plotRange.hiX - midX) * zoomX,
            midY - (midY - plotRange.loY) * zoomY,
            midY + (plotRange.hiY - midY) * zoomY });
    }
    
    void move(float deltaX, float deltaY)
    {
        auto plotRange = _plotstream.getPlotRange();
        _plotstream.setPlotRange(plotRange.move(deltaX, deltaY));
    }
    
    void mouseWheelMove(const juce::MouseEvent& event,
                        const juce::MouseWheelDetails& wheel) override
    {
        if (wheel.deltaX == 0 && wheel.deltaY == 0)
            return;
        
        auto x = static_cast<float>(event.x) / getWidth();
        auto y = static_cast<float>(event.y) / getHeight();
        zoom(x, y, 1 + wheel.deltaX, 1 + wheel.deltaY);
        repaint();
    }
    
    void mouseDrag(const juce::MouseEvent& event) override
    {
        auto deltaX = _plotstream.plotX(_lastDragPoint.x) - _plotstream.plotX(event.position.x);
        auto deltaY = _plotstream.plotY(_lastDragPoint.y) - _plotstream.plotY(event.position.y);
        _lastDragPoint = event.position;
        
        move(deltaX, deltaY);
        repaint();
    }
    
    void mouseDown(const juce::MouseEvent& event) override
    {
        _lastDragPoint = event.position;
    }
    
private:
    PlotStream _plotstream;
    juce::Point<float> _lastDragPoint;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlotComponent)
};
