using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    class TwoDimentialArrayErrorDetector : ErrorDetector
    {
        private static readonly Regex Regex = new Regex(@"\w\s*\[(?<body>[^\]""]+,[^\]""]+)\]");

        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            foreach (var match in Regex.Matches(code).OfType<Match>())
            {
                var body = match.Groups["body"].Value;
                var innerStructures = new InnerStructures.InnerStructures();
                foreach (var @char in body)
                {
                    innerStructures.Handle(@char);
                    if (@char == ',' && !innerStructures.IsInsideSomeStructure)
                    {
                        yield return
                            new ParsingError(ErrorType.NotSupportedCriticalError, "Two dimential array",
                                             Regex.Match(code).Value);
                        yield break;
                    }
                }
            }
        }
    }
}