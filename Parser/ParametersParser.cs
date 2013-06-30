using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;
using _2calgo.Parser.Extensions;
using _2calgo.Parser.InnerStructures;

namespace _2calgo.Parser
{
    public static class ParametersParser
    {
        public static readonly Regex ParameterRegex =
            new Regex(@"extern[\s\n]{1,}(?<type>\w{1,})\s+", RegexOptions.Compiled);
        
        public static IEnumerable<Parameter> Parse(string code)
        {
            foreach (var match in ParameterRegex.Matches(code).OfType<Match>())
            {
                var type = match.Groups["type"].Value.ReplaceSimpleTypesToMq4Double();
                
                var parametersPart = GetParametersPart(code, match.Index + match.Length);
                var parameters = parametersPart.SplitByComma();

                foreach (var parameter in parameters)
                {
                    var equalsPosition = parameter.IndexOf("=");
                    if (equalsPosition == -1)
                    {
                        yield return new Parameter(type.ToPublicType(), parameter.Trim(), null, type);
                        continue;
                    }
                    var name = parameter.Substring(0, equalsPosition).Trim();
                    var value = parameter.Substring(equalsPosition + 1).Trim();
                    yield return new Parameter(type.ToPublicType(), name, value, type);
                }
            }
        }

        private static string GetParametersPart(string code, int startIndex)
        {
            var result = new StringBuilder();

            var innerStructuresHandler = new InnerStructuresHandler();
            for (var i = startIndex; i < code.Length; i++)
            {
                innerStructuresHandler.Handle(code[i]);

                if (!innerStructuresHandler.IsInsideSomeStructure && code[i] == ';')
                    return result.ToString();

                result.Append(code[i]);
            }

            return result.ToString();
        }

        public static string RemoveParameters(this string code)
        {
            while (true)
            {
                var match = ParameterRegex.Match(code);
                if (!match.Success)
                    return code;

                var parametersPart = GetParametersPart(code, match.Index + match.Length);
                var headerPlusParameters = match.Value + parametersPart + ";";

                code = code.Replace(headerPlusParameters, string.Empty);
            } 
        }
    }
}