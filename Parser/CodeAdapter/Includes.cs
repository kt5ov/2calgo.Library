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
                .Distinct()
                .ToArray();
        }

        public static string IncludeFiles(this string codeWithoutComments, File[] includeFiles)
        {
            var code = codeWithoutComments;
            foreach (var match in IncludeRegex.Matches(code).OfType<Match>())
            {
                var fileName = match.Groups["fileName"].Value;
                if (fileName == "stdlib.mqh" || fileName == "stderror.mqh" || fileName == "include/stdlib.mqh")
                {
                    code = code.Replace(match.Value, string.Empty);
                }
                else
                {
                    var file = includeFiles.First(f => f.Name == fileName);
                    code = code.Replace(match.Value, file.Mq4Code);
                }
            }
            return code;
        }
    }
}