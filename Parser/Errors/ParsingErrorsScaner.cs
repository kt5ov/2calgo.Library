using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace _2calgo.Parser.Errors
{
    public static class ParsingErrorsScaner
    {
         public static string HandleParsingErrors(this string code, ParsingErrors parsingErrors)
         {
             var twoDimentialArrayRegex = new Regex(@"\[[^\]]*\]\s*\[[^\]]*\]", RegexOptions.Compiled);
             if (twoDimentialArrayRegex.IsMatch(code))
                 parsingErrors.Add(ErrorType.NotSupportedError, "Two dimential array");

             foreach (var errorDetector in ErrorDetectors)
             {
                 foreach (var error in errorDetector.FindErrors(code))
                     parsingErrors.Add(error);
             }

             return code;
         }

        private static readonly List<ErrorDetector> ErrorDetectors = new List<ErrorDetector>
            {
                new InvokesNotSupportedMethodsDetector(),

                new ContainsStringErrorDetector("#import", ErrorType.NotSupportedError),
                new ContainsStringErrorDetector("double&", ErrorType.NotSupportedError),
                new ContainsStringErrorDetector("AccountEquity", ErrorType.NotSupportedError),

                new ContainsStringErrorDetector("OBJPROP_ANGLE", ErrorType.NotSupportedWarning),
                new ContainsStringErrorDetector("DRAW_ZIGZAG", ErrorType.NotSupportedWarning),
                new ContainsStringErrorDetector("OBJPROP_BACK", ErrorType.NotSupportedWarning),
                new ContainsStringErrorDetector("OBJPROP_RAY", ErrorType.NotSupportedWarning),

                new MultiTimeframeDetector()
            };
    }
}