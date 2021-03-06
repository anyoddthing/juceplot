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

  ID:               aot_juceplot
  vendor:           daniel@anyoddthing.com
  version:          0.0.1
  name:             aot_juceplot
  description:      Simple plotting lib derived from koolplot.
  website:          http://www.anyoddthing.com
  license:          BSD

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

/* -------------------------------------------------------- */

#include <sstream>

namespace aot { namespace plot {

    #include "core/PlotExpression.h"
    #include "core/PlotData.h"
    #include "core/PlotRange.h"
    #include "core/PlotStream.h"
    #include "gui/PlotComponent.h"

} }
