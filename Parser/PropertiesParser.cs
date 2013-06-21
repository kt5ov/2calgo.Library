using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;

namespace _2calgo.Parser
{
    public static class PropertiesParser
    {
        public static readonly Regex PropertyRegex = new Regex(@"#property\s{1,}(?<name>[\w]{1,})\s*(?<value>[^\n\r\#]*)");

        public static IEnumerable<Property> Parse(string code)
        {
            var matchCollection = PropertyRegex.Matches(code);
            foreach (var match in matchCollection.OfType<Match>())
            {
                var name = match.Groups["name"].Value;
                var value = match.Groups["value"].Value.Trim();
                
                yield return new Property(name, value);
            }
        }
    }
}