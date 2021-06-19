using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
// ReSharper disable UnusedType.Global
// ReSharper disable UnusedMember.Global

namespace pjsip
{
    public static class PjsipInfo
    {
        static PjsipInfo()
        {
            if (Environment.OSVersion.Platform == PlatformID.Unix)
            {
                NativeLibrary.SetDllImportResolver(typeof(PjsipInfo).Assembly, ImportResolver);
            }
        }
        
        private static IntPtr ImportResolver(string libraryName, Assembly assembly, DllImportSearchPath? searchPath)
        {
            var libHandle = IntPtr.Zero;
            if (libraryName == "pjsua2")
            {
                var path = AppDomain.CurrentDomain.BaseDirectory ?? ".";
                var libPath = Path.Combine(path, "linux-x64/native/pjsua2.so");
                NativeLibrary.TryLoad(libPath, assembly, DllImportSearchPath.System32, out libHandle);
            }
            return libHandle;
        }

        
        [DllImport("pjsua2", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint="PjGetVersion")]
        [return: MarshalAs(UnmanagedType.LPStr)]        
        private static extern string PjGetVersion();

        public static string GetVersionInfo()
        {
            var version = PjGetVersion();
            return $"PJSIP V{version}";
        }
    }
    
    public static class PjsipExt
    {
        [DllImport("pjsua2", EntryPoint="pj_enable_media_negotiation_module")]
        private static extern int pj_enable_media_negotiation_module();

        public static int EnableTelephoneEventNegotiationExtension()
        {
            return pj_enable_media_negotiation_module();
        }
    }
    
}