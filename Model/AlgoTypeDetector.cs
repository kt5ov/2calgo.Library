using System.Linq;

namespace _2calgo.Model
{
    public static class AlgoTypeDetector
    {
        private static readonly string[] RobotKeyWords = new[]
            {
                "OrderSend",
                "OrderClose(",
                "OrderCloseBy",
                "OrderDelete",
                "OrderModify",
            };

        public static AlgoType? GetAlgoType(string code, AlgoType defaultValue)
        {
            if (!code.Contains("start"))
                return null;
            if (RobotKeyWords.Any(code.Contains))
                return AlgoType.Robot;

            return defaultValue;
        }
    }
}