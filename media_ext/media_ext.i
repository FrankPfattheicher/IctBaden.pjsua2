
%include "arrays_csharp.i"
%include "typemaps.i"

%{
#include "pjsua2/media.hpp"
%}

%feature("director") AudioMediaCapture;

