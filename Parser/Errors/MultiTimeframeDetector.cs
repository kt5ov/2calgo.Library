using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    internal class MultiTimeframeDetector : ErrorDetector
    {
        private static readonly List<string> _multiTimeframeMethods = new List<string>
            {
                "iOpen",
                "iHigh",
                "iLow",
                "iClose",
                "iVolume",
                "iTime",
                "iBarShift",
                "iHighest",
                "iLowest",
                "Highest",
                "Lowest",
                "iMA",
                "iRSI",
                "iBands",
                "iADX",
                "iATR",
                "iMACD",
                "iCCI",
                "iStdDev",
                "iWPR",
                "iSAR",
            }; 

        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            var methodCalls = MethodCallsParser.Parse(code).ToArray();
            foreach (var methodCall in methodCalls)
            {
                if (_multiTimeframeMethods.Any(m => m == methodCall.MethodName))
                {
                    var symbol = methodCall.Parameters[0];
                    if (symbol != "Symbol()" && symbol != "NULL")
                    {
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiSymbol", symbol);
                    }
                    var timeframe = methodCall.Parameters[1];
                    if (timeframe != "0" && timeframe != "Period()")
                    {
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiTimeFrame", timeframe);
                    }
                }
            }
        }
    }
}