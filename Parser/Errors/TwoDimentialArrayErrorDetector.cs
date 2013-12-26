using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.Errors
{
    class TwoDimentialArrayErrorDetector : ErrorDetector
    {
        private static readonly Regex First = new Regex(@"\w\s*\[(?<body>[^\]""]+,[^\]""]+)\]");
        private static readonly Regex Second = new Regex(@"double\s+[^\[]*\[[^\]\r]*\]\s*\[[^\]\r]*\]");

        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            foreach (var match in First.Matches(code).OfType<Match>())
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
                                             match.Value);
                        yield break;
                    }
                }
            }
            foreach (var match in Second.Matches(code).OfType<Match>())
            {
                yield return new ParsingError(ErrorType.NotSupportedCriticalError, "Two dimential array",
                                             match.Value);
            }
        }
    }
}