using _2calgo.Parser.CodeAdapter.Switch;

namespace _2calgo.Parser.CodeAdapter
{
    public static class CAlgoCodeAdapter
    {
        public static string Adapt(this string code)
        {
            return code
                .FixReturnStatements()
                .SplitDeclarationsAndAssignments()
                .FixArrayDeclarations()
                .ReplaceSymbolToEmptyString()
                .FixSwitches();
        }
    }
}