using System.Text.RegularExpressions;
using _2calgo.Model;
using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
    public static class StaticVariables
    {
        private static readonly Regex DeclarationRegex = new Regex(@"static\s+(?<declaration>[^;]+;)");

        public static void ExtractStaticVariablesToFields(IndicatorCode code)
        {
            foreach (var function in code.Functions)
            {
                foreach (var match in DeclarationRegex.Matches(function.Body).OfType<Match>())
                {
                    
                }
            }
        }
    }
}