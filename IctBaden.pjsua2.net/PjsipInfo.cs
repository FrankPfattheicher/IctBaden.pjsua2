namespace pjsip
{
    public static class PjsipInfo
    {
        public static string GetVersionInfo()
        {
            var ep = new Endpoint();
            ep.libCreate();
            var cfg = new EpConfig();
            ep.libInit(cfg);
            var ver = ep.libVersion();
            ep.libDestroy();
            ep.Dispose();
            return $"PJSIP V{ver.full}";
        }
    }
}