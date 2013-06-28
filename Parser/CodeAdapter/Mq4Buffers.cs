using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Buffers
    {
        private static readonly Regex DoubleBufferRegex = new Regex(@"Mq4Double\s{1,}\w{1,}\s*\[\s*\]\s*\;", RegexOptions.Compiled);

        public static string RemoveMq4Buffers(this string code)
        {
            code = code.ReplaceSimpleTypesToMq4Double();
            return DoubleBufferRegex.Replace(code, string.Empty);
        }
    }
}