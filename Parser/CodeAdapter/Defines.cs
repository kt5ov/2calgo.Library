using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Defines
    {
        private static readonly Regex DefineRegex = new Regex(@"\#define\s+(?<name>[^\s]+)\s+(?<value>[^\r\n]+)",
                                                              RegexOptions.Compiled);

        public static string ReplaceDefines(this string code)
        {
            while (true)
            {
                var match = DefineRegex.Match(code);
                if (!match.Success)
                    break;

                code = code.Remove(match.Index, match.Length);
                var name = match.Groups["name"].Value;
                var value = match.Groups["value"].Value;
                var replaceRegex = new Regex(@"(?<=[^\w])" + name + @"(?=[^\w])");
                code = replaceRegex.Replace(code, value);
            }
            return code;
        }
    }
}