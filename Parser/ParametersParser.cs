using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser
{
    public static class ParametersParser
    {
        public static readonly Regex ParameterRegex =
            new Regex(@"extern[\s\n]{1,}(?<type>\w{1,})\s+(?<parameters>[^;]*);", RegexOptions.Compiled);
        
        public static IEnumerable<Parameter> Parse(string code)
        {
            foreach (var match in ParameterRegex.Matches(code).OfType<Match>())
            {
                var type = match.Groups["type"].Value.ReplaceSimpleTypesToMq4Double();
                var parameters = match.Groups["parameters"].Value.SplitByComma();

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
    }
}