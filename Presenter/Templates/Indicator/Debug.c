    static class Debug
    {
        private static Action<string> _printAction;
        public static void Initialize(Action<string> printAction)
        {
            _printAction = printAction;
        }

        public static void WriteLine(string message)
        {
            _printAction(message);
        }

        public static void WriteLine(string format, params object[] args)
        {
            _printAction(string.Format(format, args));
        }
    }