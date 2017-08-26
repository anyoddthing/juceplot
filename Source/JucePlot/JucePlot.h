#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               JucePlot
  vendor:           daniel@anyoddthing.com
  version:          0.0.1
  name:             JucePlot
  description:      Simple plotting lib derived from koolplot.
  website:          http://www.anyoddthing.com
  license:          BSD

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

/* -------------------------------------------------------- */

namespace aot { namespace plot {

    #include "core/PlotExpression.h"
    #include "core/Plotdata.h"
    #include "core/Plotstream.h"
    #include "gui/PlotComponent.h"

} }
