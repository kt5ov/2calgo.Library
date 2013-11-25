class FolderPaths
{
    public static string _2calgoAppDataFolder
    {
        get
        {
            var result = Path.Combine(SystemAppData, "2calgo");
            if (!Directory.Exists(result))
                Directory.CreateDirectory(result);
            return result;
        }
    }

    public static string _2calgoDesktopFolder
    {
        get
        {
            var result = Path.Combine(Desktop, "2calgo");
            if (!Directory.Exists(result))
                Directory.CreateDirectory(result);
            return result;
        }
    }

    static string SystemAppData
    {
        get { return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData); }
    }

    static string Desktop
    {
        get { return Environment.GetFolderPath(Environment.SpecialFolder.Desktop); }
    }
}