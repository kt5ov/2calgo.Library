using System.Collections.Generic;

namespace _2calgo.Parser.Errors
{
    public static class ParsingErrorsScaner
    {
         public static string HandleParsingErrors(this string code, ParsingErrors parsingErrors)
         {
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
                new FileSeekDetector(),

                new ContainsStringErrorDetector("#import", ErrorType.NotSupportedCriticalError),
                new ContainsStringErrorDetector("OBJPROP_ANGLE", ErrorType.NotSupportedWarning),
                new ContainsStringErrorDetector("DRAW_ZIGZAG", ErrorType.NotSupportedWarning),
                new ContainsStringErrorDetector("OBJPROP_BACK", ErrorType.NotSupportedWarning),
            };
    }
}