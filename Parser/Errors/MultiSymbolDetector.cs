using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    internal class MultiSymbolDetector : ErrorDetector
    {
        private static readonly List<string> PossibleMultiTimeFrameMethods = new List<string>
            {
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
                "iFractals",
                "iStochastic",
                "iBars",
            };

        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            var methodCalls = MethodCallsParser.Parse(code).ToArray();
            foreach (var methodCall in methodCalls)
            {
                if (PossibleMultiTimeFrameMethods.Any(m => m == methodCall.MethodName))
                {
                    var symbol = methodCall.Parameters[0];
                    if (symbol != "Symbol()" && symbol != "NULL" && symbol != "0")
                    {
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiSymbol", symbol);
                    }
                }
            }
        }
    }
}