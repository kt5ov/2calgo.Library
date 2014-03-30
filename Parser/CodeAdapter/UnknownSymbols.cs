using System.Linq;
using System.Text;
using _2calgo.Parser.InnerStructures;

namespace _2calgo.Parser.CodeAdapter
{
    public static class UnknownSymbols
    {
        static readonly char[] NotReplaceChars =
        {
            ' ',
            '\t',
            '\n',
            '\r',

            ',',
            ':',
            ';',
            '_',
            '.',
            '#',
            '%',
            '^',
            '`',
            '\'',
            '"',

            '=',
            '<',
            '>',
                
            '&',
            '|',
            '*',
            '/',
            '\\',
            '+',
            '-',
            '!',

            '(',
            ')',
            '[',
            ']',
            '{',
            '}',
        };


        public static string ReplaceUnknownSymbols(this string code)
        {
            var result = new StringBuilder();

            var stringStructure = new StringStructure();
            for (var i = 0; i < code.Length; i++)
            {
                stringStructure.Handle(code[i]);

                if (char.IsLetter(code[i]) || char.IsDigit(code[i]) || stringStructure.IsInsideString || NotReplaceChars.Contains(code[i]))
                {
                    result.Append(code[i]);
                }
                else
                {
                    var bytes = Encoding.Unicode.GetBytes(code[i].ToString());
                    var replacement = "u" + string.Join("_", bytes.Select(b => b.ToString()));
                    result.Append(replacement);
                }
            }

            return result.ToString();
        }
    }
}