using System;
using System.Text.RegularExpressions;
using _2calgo.Model;
using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
    public static class StaticVariables
    {
        private static readonly Regex DeclarationRegex = new Regex(@"static\s+(?<declaration>[^;]+;)");
                                                                  
        public static void ExtractStaticVariablesToFields(this IndicatorCode code)
        {
            foreach (var function in code.Functions)
            {
                foreach (var match in DeclarationRegex.Matches(function.Body).OfType<Match>())
                {
                    var declaration = match.Groups["declaration"].Value;
                    function.Body = function.Body.Replace(match.Value, string.Empty);
                    code.FieldsDeclarations += Environment.NewLine + declaration;
                }
            }
        }
    }
}