using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using _2calgo.Model;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Includes
    {
        private static readonly Regex IncludeRegex = new Regex(@"#include\s+(\<|\"")(?<fileName>[^\>\""]+)(\>|\"")", RegexOptions.Compiled);

        public static IEnumerable<string> GetIncludes(string code)
        {
            code = code.RemoveComments();

            return IncludeRegex.Matches(code)
                .OfType<Match>()
                .Select(m => m.Groups["fileName"].Value)
                .Where(NotStandard)
                .Distinct()
                .ToArray();
        }

        private static readonly string[] StandardIncludes = new[]
        {
            "stdlib.mqh",
            "stderror.mqh",
            "include/stdlib.mqh"
        };

        private static bool NotStandard(string name)
        {
            return !StandardIncludes.Contains(name);
        }

        public static string IncludeFiles(this string codeWithoutComments, File[] includeFiles)
        {
            var code = codeWithoutComments;
            foreach (var match in IncludeRegex.Matches(code).OfType<Match>())
            {
                var fileName = match.Groups["fileName"].Value;
                if (NotStandard(fileName))
                {
                    var file = includeFiles.First(f => f.Name == fileName);
                    code = code.Replace(match.Value, file.Mq4Code);
                }
                else
                {
                    code = code.Replace(match.Value, string.Empty);
                }
            }
            return code;
        }
    }
}