using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Names
    {
        private static readonly Regex NameRegex = new Regex(@"(\w+\.)+\w+");
        
        public static string RemoveDotsFromNames(this string code)
        {
            int matchCount;
            do
            {
                matchCount = 0;
                var handled = new HashSet<string>();
                foreach (var match in NameRegex.Matches(code).OfType<Match>())
                {
                    var name = match.Value;
                    if (handled.Contains(name))
                        continue;
                    handled.Add(name);
                    double doubleValue;
                    if (double.TryParse(name, out doubleValue))
                        continue;
                    matchCount++;
                    var nameWithoutDots = name.Replace(".", string.Empty);
                    code = code.Replace(name, nameWithoutDots);
                }
            } while (matchCount > 0);
            return code;
        }

        private static readonly Regex IncludeRegex = new Regex(@"#include.*");
        public static string RemoveIncludes(this string code)
        {
            return IncludeRegex.Replace(code, string.Empty);
        }
    }
}