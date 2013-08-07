using System.Text;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Comments
    {
        public static string RemoveComments(this string code)
        {
            return code
                .RemoveMultiLineComments()
                .RemoveSingleLineComments();
        }

        private static string RemoveSingleLineComments(this string code)
        {
            var result = new StringBuilder();
            var lines = code.SplitByLines();
            foreach (var line in lines)
            {
                var commentIndex = StartCommentIndex(line);

                if (commentIndex == -1)
                {
                    result.AppendLine(line);
                }
                else
                {
                    if (commentIndex != 0)
                        result.AppendLine(line.Substring(0, commentIndex));
                }
            }

            return result.ToString();
        }

        private static int StartCommentIndex(string line)
        {
            var quoteOpen = false;
            for (var i = 0; i < line.Length - 1; i++)
            {
                if (line[i] == '"')
                    quoteOpen = !quoteOpen;
                if (!quoteOpen && line.Substring(i, 2) == @"//")
                    return i;
            }
            return -1;
        }

        private static string RemoveMultiLineComments(this string code)
        {
            var result = new StringBuilder();
            var isComment = false;
            var i = 0;
            var isInsideSingleLineComment = false;
            while (i < code.Length)
            {
                if (code.SafeSubstring(i, 2) == "//")
                    isInsideSingleLineComment = true;
                if (code[i] == '\n')
                    isInsideSingleLineComment = false;
                if (code.SafeSubstring(i, 2) == "/*" && !isInsideSingleLineComment)
                {
                    isComment = true;
                    i += 2;
                    continue;
                }
                if (code.SafeSubstring(i, 2) == "*/")
                {
                    isComment = false;
                    i += 2;
                    continue;
                }
                if (isComment)
                {
                    i += 1;
                    continue;
                }
                result.Append(code[i]);
                i += 1;
            }
            return result.ToString();
        }
    }
}