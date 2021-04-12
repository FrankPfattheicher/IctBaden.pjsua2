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


### My plans are:

* automate build to support latest PJSIP release &nbsp; &nbsp; ✓
* build nuget package &nbsp; &nbsp; ⌛

### Further plans:

* support Linux (Ubuntu x64)
* support RaspberryPi (Linux ARM) 

