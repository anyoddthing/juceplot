#pragma once

struct PlotRange
{
    PlotRange() = default;
    PlotRange(double loX, double hiX, double loY, double hiY)
    {
        this->loX = loX;
        this->hiX = hiX;
        this->loY = loY;
        this->hiY = hiY;
    }
    
    PlotRange move(double deltaX, double deltaY)
    {
        return {
            loX + deltaX,
            hiX + deltaX,
            loY + deltaY,
            hiY + deltaY
        };
    }
    
    bool isXOriginVisible()
    {
        return loX < 0 && hiX > 0;
    }
    
    double getIncrStep(Grain grain = Grain::MEDIUM)
    {
        return (hiX - loX) / grain;
    }
    
    double getXRange()
    {
        return hiX - loX;
    }
    
    double getYRange()
    {
        return hiY - loY;
    }
    
    template <typename StreamT>
    friend StreamT& operator<< (StreamT& stream, const PlotRange& plotRange)
    {
        stream
            << "PlotRange: loX=" << plotRange.loX << " hiX=" << plotRange.hiX
            << " loY=" << plotRange.loY << " hiY=" << plotRange.hiY;
        
        return stream;
    }

    double loX;	double hiX; 	// Highest and lowest data values in x
    double loY;	double hiY;	// Highest and lowest points on y axis

};

