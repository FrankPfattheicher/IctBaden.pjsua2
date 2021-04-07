using System.Runtime.InteropServices;

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
        [System.Runtime.InteropServices.DllImport("pjsua2", EntryPoint="pj_enable_media_negotiation_module")]
        private static extern int pj_enable_media_negotiation_module();

        public static void EnableTelephoneEventNegotiationExtension()
        {
            pj_enable_media_negotiation_module();
        }
    }
    
}