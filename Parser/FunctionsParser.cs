using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;

namespace _2calgo.Parser
{
    public static class FunctionsParser
    {
        private static readonly Regex DeclarationRegex = new Regex(@"\w{1,}\s{1,}(?<name>\w{1,})\s*\([\w\,\s\=]*\)", RegexOptions.Compiled);

        public static IEnumerable<Function> Parse(string code)
        {
            var matches = DeclarationRegex.Matches(code).OfType<Match>().ToArray();

            var previousFunctionEndIndex = -1;
            foreach (var match in matches)
            {
                var name = match.Groups["name"].Value;
                if (match.Index < previousFunctionEndIndex)
                    continue;
                
                var declaration = match.Value;
                var bodyWithAroundingBrackets = code.GetBodyWithAroundingBrackets(match.Index);
                var body = bodyWithAroundingBrackets.Substring(1, bodyWithAroundingBrackets.Length - 2);
                var adaptedBody = body.AdaptFunctionBody();
                
                previousFunctionEndIndex = match.Index + declaration.Length + bodyWithAroundingBrackets.Length;
                var function = new Function(name, declaration, adaptedBody)
                    .FixReturnValue();

                yield return function;
            }
        }

        public static string RemoveFunctions(this string code)
        {
            do
            {
                var match = DeclarationRegex.Match(code);
                if (!match.Success)
                    break;

                var bodyWithAroundingBrackets = code.GetBodyWithAroundingBrackets(match.Index);
                code = code.Replace(bodyWithAroundingBrackets, string.Empty);
                code = code.Replace(match.Value, string.Empty);
            } while (true);

            return code;
        }
    }
}