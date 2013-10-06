using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    class PartialCloseDetector : ErrorDetector
    {
        private static readonly Regex OrderCloseRegex = new Regex(@"OrderClose\s*\([^,]*,(?<lots>[^,]*),");

        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            foreach (var match in OrderCloseRegex.Matches(code).OfType<Match>())
            {
                var lots = match.Groups["lots"].Value.Replace(" ", string.Empty).Trim();
                
                if (lots != "OrderLots()")
                    yield return new ParsingError(ErrorType.NotSupportedError, "Partial close", lots);
            }
        }
    }
}