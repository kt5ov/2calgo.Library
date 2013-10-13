using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    class InvokesNotSupportedMethodsDetector : ErrorDetector
    {
        private static readonly List<MethodError> MethodErrors = new List<MethodError>
            {
                new MethodError("IndicatorDigits", ErrorType.NotSupportedWarning),
                new MethodError("IndicatorShortName", ErrorType.NotSupportedWarning),
                new MethodError("SetIndexLabel", ErrorType.NotSupportedWarning),
            };
        
        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            var methodCalls = MethodCallsParser.Parse(code).ToArray();
            foreach (var methodError in MethodErrors)
            {
                if (methodCalls.Any(call => call.MethodName == methodError.MethodName))
                    yield return new ParsingError(methodError.ErrorType, methodError.MethodName + "()", null);
            }
        }
    }
}