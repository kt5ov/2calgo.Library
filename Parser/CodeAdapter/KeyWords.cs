using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class KeyWords
    {
        private static readonly List<string> KeyWordsToBeReplaced = new List<string>
            {
                "long", "short",  "abstract",  "event",  "new",  "struct",  "base",  "object",  "this",  "operator", 
                "throw", "finally",  "out",  "byte",  "fixed",  "override",  "try",  "float",  "params",  "typeof", 
                "catch", "private",  "uint",  "char",  "foreach",  "protected",  "ulong",  "checked",  "goto",  "public", 
                "unchecked", "class",  "readonly",  "unsafe",  "const",  "implicit",  "ref",  "ushort",  "in",  "using", 
                "decimal", "sbyte",  "virtual",  "interface",  "sealed",  "volatile",  "delegate",  "internal",  "short", 
                "do", "is",  "sizeof",  "lock",  "stackalloc",  "long",  "enum",  "namespace"

            };

         public static string ReplaceCSharpKeyWords(this string code)
         {
             foreach (var keyWord in KeyWordsToBeReplaced)
             {
                 var regex = new Regex(@"(?<=[^\w])" + keyWord + @"(?=[^\w])");
                 code = regex.Replace(code, keyWord + "_");
             }
             return code;
         }
    }
}