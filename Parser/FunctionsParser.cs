using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser
{
    public static class FunctionsParser
    {
        private static readonly Regex DeclarationRegex = new Regex(@"(?<type>\w{1,})\s{1,}(?<name>\w{1,})\s*\((?<parameters>[^\)]*)\)", RegexOptions.Compiled);
        private static readonly Regex ParameterRegex = new Regex(@"(?<type>[^\s]+) (?<name>[^=]+)(=(?<defaultValue>\s*.+)){0,1}", RegexOptions.Compiled);

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

                var type = match.Groups["type"].Value;
                var parameters = match.Groups["parameters"].Value
                    .RemoveRefModifiers()
                    .ReplaceArraysToIMq4Arrays()
                    .SplitByComma()
                    .Where(s => s != string.Empty)
                    .Select(CreateParameter)
                    .ToArray();
                
                
                previousFunctionEndIndex = match.Index + declaration.Length + bodyWithAroundingBrackets.Length;
                var function = new Function(type, name, parameters, adaptedBody)
                    .FixReturnValue()
                    .RenameStandardFunctions();

                yield return function;
            }
        }

        private static string RemoveRefModifiers(this string parameters)
        {
            return parameters.Replace("ref ", string.Empty);
        }

        private static FunctionParameter CreateParameter(string parameterAsString, int index)
        {
            var parameterWithoutAmpersand = parameterAsString.Replace("&", string.Empty);
            var match = ParameterRegex.Match(parameterWithoutAmpersand);
            return new FunctionParameter
                {
                    ByReference = parameterAsString.Contains("&"),
                    DefaultValue = match.Groups["defaultValue"].Value,
                    Name = match.Groups["name"].Value,
                    Type = match.Groups["type"].Value,
                    Index = index
                };
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