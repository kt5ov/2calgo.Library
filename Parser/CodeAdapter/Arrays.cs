using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Arrays
    {
        private static readonly Regex ArrayDeclarationRegex = new Regex(
            @"(?<type>\w+)\s+(?<name>\w+)\s*\[(?<size>[^\]]*)\]\s*(\=\s*(?<init>\{[^\}]*\})\s*){0,1};", RegexOptions.Compiled);

        public static string FixArrayDeclarations(this string code)
        {
            foreach (var match in ArrayDeclarationRegex.Matches(code).OfType<Match>())
            {
                var variable = match.Groups["name"].Value;
                var type = match.Groups["type"].Value;
                var size = match.Groups["size"].Value.Trim();
                var init = match.Groups["init"].Value.Trim();
                
                string replacement;
                if (size == string.Empty)
                {
                    var initReplacement = init == string.Empty ? string.Empty : string.Format(" = new {0}[] {1}", type, init);
                    replacement = string.Format("{0}[] {1}{2};", type, variable, initReplacement);
                }
                else
                {
                    replacement = string.Format("{0}[] {1} = new {0}[{2}]{3};", type, variable, size, init);
                }

                code = code.Replace(match.Value, replacement);
            }
            return code;
        }
    }
}