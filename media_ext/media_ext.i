
%include "arrays_csharp.i"
%include "typemaps.i"

%{
#include "pjsua2/media.hpp"
%}

%feature("director") LogWriter;

%feature("director") AudioMediaCapture;
%feature("director") AudioMediaPlayback;
