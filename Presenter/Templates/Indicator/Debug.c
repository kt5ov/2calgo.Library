static class Debug
{
    private static Action<string> _printAction;
    private static bool _isActive;

    public static void Initialize(Action<string> printAction)
    {
        _printAction = printAction;
    }

    public static void WriteLine(string message)
    {
        if (_isActive)
            _printAction(message);
    }

    public static void WriteLine(string format, params object[] args)
    {
        if (_isActive)
            _printAction(string.Format(format, args));
    }

    public static void Activate()
    {
        _isActive = true;
    }

    public static void Deactivate()
    {
        _isActive = false;
    }
}