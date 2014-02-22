using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using _2calgo.Parser.CodeAdapter;

namespace _2calgo.Library
{
    public static class IncludeScaner
    {
        private static readonly Regex IncludeRegex = new Regex(@"#include\s+(\<|\"")(?<fileName>[^\>\""]+)", RegexOptions.Compiled);

        public static IEnumerable<string> GetIncludes(string code)
        {
            code = code.RemoveComments();

            return IncludeRegex.Matches(code)
                .OfType<Match>()
                .Select(m => m.Groups["fileName"].Value)
                .Distinct()
                .ToArray();
        }
    }
}