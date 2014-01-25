using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Arrays
    {
        private static readonly Regex ArrayDeclarationRegex = new Regex(
            @"(?<type>\w+)\s+(?<name>\w+)\s*\[(?<size1>[^\]]*)\](\[(?<size2>[^\]]*)\]){0,1}(?<initialization>\s*\=[\s\n\r]*(?<values>\{[^\}]*\})){0,1}", RegexOptions.Compiled);

        public static string ReplaceArraysToMq4Arrays(this string code)
        {
            bool changed;
            do
            {
                changed = false;
                foreach (var match in ArrayDeclarationRegex.Matches(code).OfType<Match>())
                {
                    var type = match.Groups["type"].Value;
                    if (!type.IsSupported())
                        continue;

                    type = type.ReplaceSimpleTypesToMq4Types();

                    var name = match.Groups["name"].Value;
                    var size1 = match.Groups["size1"].Value;
                    var size2 = match.Groups["size2"].Value;
                    if (size1.Contains(","))
                    {
                        var splitByComma = size1.SplitByComma();
                        size1 = splitByComma[0];
                        size2 = splitByComma[1];
                    }

                    var values = match.Groups["values"].Value;
                    
                    string replacement;
                    if (string.IsNullOrEmpty(size2))
                        replacement = string.Format("{0}Array {1} = new {0}Array({2}{3}){4}", type, name, size1, size2, values);
                    else
                        replacement = string.Format("Mq4DoubleTwoDimensionalArray {0} = new Mq4DoubleTwoDimensionalArray({1}){2}", name, size2, values);

                    code = code
                        .Remove(match.Index, match.Value.Length)
                        .Insert(match.Index, replacement);
                    changed = true;
                    break;
                }
            } while (changed);

            return code;
        }

        public static string ReplaceArraysToIMq4Arrays(this string code)
        {
            bool changed;
            do
            {
                changed = false;
                foreach (var match in ArrayDeclarationRegex.Matches(code).OfType<Match>())
                {
                    var type = match.Groups["type"].Value;
                    if (!type.IsSupported())
                        continue;
                    type = type.ReplaceSimpleTypesToMq4Types();                    
                    var name = match.Groups["name"].Value;

                    string replacement;
                    if (type == "Mq4Double")
                        replacement = string.Format("IMq4DoubleArray {1}", type, name);
                    else
                        replacement = string.Format("{0}Array {1}", type, name);
                    code = code
                        .Remove(match.Index, match.Value.Length)
                        .Insert(match.Index, replacement);
                    changed = true;
                    break;
                }
            } while (changed);

            return code;
        }
}
}