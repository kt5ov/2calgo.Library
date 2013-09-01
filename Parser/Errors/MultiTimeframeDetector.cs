using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    internal class MultiTimeframeDetector : ErrorDetector
    {
        private static readonly List<string> PossibleMultiTimeFrameMethods = new List<string>
            {
                "iCCI",
                "iStochastic",
            };
        
        private static readonly List<string> MultiTimeFrameMethods = new List<string>
            {
                "ArrayCopySeries",
            };

        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            var methodCalls = MethodCallsParser.Parse(code).ToArray();
            foreach (var methodCall in methodCalls)
            {
                if (PossibleMultiTimeFrameMethods.Any(m => m == methodCall.MethodName))
                {
                    var symbol = methodCall.Parameters[0];
                    if (symbol != "Symbol()" && symbol != "NULL")
                    {
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiSymbol", symbol);
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiTimeFrame or MultiSymbol", null);
                    }
                    var timeframe = methodCall.Parameters[1];
                    if (timeframe != "0" && timeframe != "Period()")
                    {
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiTimeFrame", timeframe);
                        yield return new ParsingError(ErrorType.NotSupportedError, "MultiTimeFrame or MultiSymbol", null);
                    }
                }
                if (MultiTimeFrameMethods.Any(m => m == methodCall.MethodName))
                    yield return new ParsingError(ErrorType.NotSupportedError, "MultiTimeFrame or MultiSymbol", null);
            }
        }
    }
}