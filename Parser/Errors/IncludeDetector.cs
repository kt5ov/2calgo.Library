using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    class IncludeDetector : ErrorDetector
    {
        private static readonly Regex IncludeRegex = new Regex(@"#include\s+[<""](?<name>[^\>""]*)[>""]");
        
        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            foreach (var match in IncludeRegex.Matches(code).OfType<Match>())
            {
                if (match.Groups["name"].Value != "stdlib.mqh" && match.Groups["name"].Value != "stderror.mqh")
                {
                    yield return new ParsingError(ErrorType.NotSupportedCriticalError, "#include", match.Groups["name"].Value);
                }
            }
        }
    }
}