using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Buffers
    {
        private static readonly Regex BufferRegex = new Regex(@"Mq4Double\s{1,}\w{1,}\s*\[\s*\]\s*\;", RegexOptions.Compiled);

        public static string RemoveMq4Buffers(this string code)
        {
            return BufferRegex.Replace(code, string.Empty);
        }
    }
}