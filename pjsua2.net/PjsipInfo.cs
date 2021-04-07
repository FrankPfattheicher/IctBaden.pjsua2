using System.Runtime.InteropServices;
// ReSharper disable UnusedType.Global
// ReSharper disable UnusedMember.Global

namespace pjsip
{
    public static class PjsipInfo
    {
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