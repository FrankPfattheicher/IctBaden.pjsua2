# IctBaden.pjsua2
.NET wrapper for PJSUA2 (not the first one).

There are several others on nuget:

* [pjsip-pjsua2-csharp, PJSIP 2.8](https://www.nuget.org/packages/pjsip-pjsua2-csharp/)
* [pjsip-pjsua2-cs, PJSIP 2.10](https://www.nuget.org/packages/pjsip-pjsua2-cs/)
* more on [nuget.org...](https://www.nuget.org/packages?q=pjsip)

For more information about the PJSIP project see https://www.pjsip.org/

### Motivation
The problem was, only **pjsip-pjsua2-cs** uses PJSIP version 2.10.1, that is also not the latest. There is no public repository to build that and mailing the owner has not answered.

**pjsip-pjsua2-csharp** is two years inactive.

So i decided to do it my own, using the help of **pjsip-pjsua2-csharp's** build-it-yourself documentation (thanks a lot).

### What is in the Package

pjsua2.net.dll - .NET Core (netcoreapp3.1) wrapper library    
pjsua2.dll - Native Windows x64 PJSUA2 library including SWIG generated C# wrappers    

### Extensions
The pjsua2.net.dll includes two additional static classes:

``` c#
    public static class PjsipInfo
    {
        // Get included PJSIP version
        public static string GetVersionInfo();
    }
    
    public static class PjsipExt
    {
        // Enable the pj-media-neg extension
        public static int EnableTelephoneEventNegotiationExtension();
    }
```

Mor information for [pj-media-neg...](https://github.com/FrankPfattheicher/pj-media-neg)

## Roadmap

April 2021
* automate build to support latest PJSIP release &nbsp; &nbsp; ✓
* build nuget package &nbsp; &nbsp; ✓
* support Windows (>= 7, x64) &nbsp; &nbsp; ✓

June 2021
* support Linux (Ubuntu >= 18.04, x64) &nbsp; &nbsp; ✓

October 2023
* NET 6.0
* Updated to PJSIP version 2.13-dev

March 2024
* C#12, NET 8.0
* Updated to PJSIP version 2.14-dev

October 2024
* Audio stream capture added 

### Further plans:

* support RaspberryPi (Linux ARM) &nbsp; &nbsp; ⌛

## Enable Logging
Set PJ_LOG_MAX_LEVEL to desired value in site_config.h

For example:

    #   define PJ_LOG_MAX_LEVEL			5

## License
The code in this repository is under MIT license.

The code in PJSIP (not included here) is under General Public License (GPL) version 2.    
For more information see: https://www.pjsip.org/licensing.htm

