using System.Text.RegularExpressions;

namespace _2calgo.Parser.Extensions
{
    public static class MatchExtensions
    {
        public static int? GetIntValue(this Match match, string groupName)
        {
            int result;
            if (!int.TryParse(match.Groups[groupName].Value, out result))
                return null;
            return result;
        }
    }
}