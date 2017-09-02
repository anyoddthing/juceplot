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

    double loX;	double hiX; 	// Highest and lowest data values in x
    double loY;	double hiY;	// Highest and lowest points on y axis

};
