using _2calgo.Parser.CodeAdapter.Switch;

namespace _2calgo.Parser.CodeAdapter
{
    public static class CAlgoCodeAdapter
    {
        public static string AdaptFunctionBody(this string functionBody)
        {
            return functionBody
                .FixReturnStatements()
                .SplitDeclarationsAndAssignments()
                .FixArrayDeclarations()
                .ReplaceSymbolToEmptyString()
                .FixSwitches()
                .FixArrayResize();
        }
    }
}