using System.Text.RegularExpressions;
using System.Linq;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Buffers
    {
        private static readonly Regex DoubleBufferRegex = new Regex(@"Mq4Double\s{1,}(?<name>\w{1,})\s*\[\s*\]\s*\;", RegexOptions.Compiled);

        public static string RemoveMq4Buffers(this string code, string[] buffers)
        {
            code = code.ReplaceSimpleTypesToMq4Double();
            foreach (var match in DoubleBufferRegex.Matches(code).OfType<Match>())
            {
                if (buffers.All(b => b != match.Groups["name"].Value))
                    continue;

                code = code.Replace(match.Value, string.Empty);
            }
            return code;
        }
    }
}