using System.Runtime.InteropServices;

namespace pjsip;

public static class PjsipExt
{
    /// ====== media ====== 
    [DllImport("pjsua2", EntryPoint="pj_enable_media_negotiation_module")]
    private static extern int pj_enable_media_negotiation_module();

    public static int EnableTelephoneEventNegotiationExtension() => pj_enable_media_negotiation_module();

    
    /// ====== log level ====== 
    [DllImport("pjsua2", EntryPoint="pj_log_get_level")]
    private static extern int pj_log_get_level();

    public static int GetLogLevel() => pj_log_get_level();

    
    [DllImport("pjsua2", EntryPoint="pj_log_set_level")]
    private static extern void pj_log_set_level(int level);

    public static void SetLogLevel(int level) => pj_log_set_level(level);

}