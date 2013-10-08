using System.Linq;

namespace _2calgo.Model
{
    public static class AlgoTypeDetector
    {
        private static readonly string[] IndicatorKeyWords = new[]
            {
                "IndicatorBuffers",
                "IndicatorCounted",
                "IndicatorDigits",
                "IndicatorShortName",
                "SetIndexArrow",
                "SetIndexBuffer",
                "SetIndexDrawBegin",
                "SetIndexEmptyValue",
                "SetIndexLabel",
                "SetIndexShift",
                "SetIndexStyle",
                "SetLevelStyle",
                "SetLevelValue",
                "indicator_chart_window",
                "indicator_buffers",
                "indicator_color",
                "indicator_width",
                "indicator_level",
            };

        private static readonly string[] RobotKeyWords = new[]
            {
                "OrderSend",
                "OrderClose",
                "OrderCloseBy",
                "OrderDelete",
                "OrderModify",
            };

        public static AlgoType? GetAlgoType(string code)
        {
            if (!code.Contains("start"))
                return null;
            if (RobotKeyWords.Any(code.Contains))
                return AlgoType.Robot;
            if (IndicatorKeyWords.Any(code.Contains))
                return AlgoType.Indicator;
            return AlgoType.Robot;
        }
    }
}