namespace pjsip
{
    public static class PjsipInfo
    {
        [System.Runtime.InteropServices.DllImport("pjsua2", EntryPoint="pj_get_version")]
        private static extern string pj_get_version();

        public static string GetVersionInfo()
        {
            return pj_get_version();
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