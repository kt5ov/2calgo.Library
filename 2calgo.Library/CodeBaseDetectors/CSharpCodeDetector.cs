using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;

namespace _2calgo.Library.CodeBaseDetectors
{
    public static class CSharpCodeDetector
    {
        public static bool IsCSharpCode(string code)
        {
            code = code.RemoveComments();
            foreach (var cSharpWord in NotSupportedNames.CSharp)
            {
                if (code.Contains(cSharpWord))
                {
                    return true;
                }
            }
            return false;
        }
    }
}