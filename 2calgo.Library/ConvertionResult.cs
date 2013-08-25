using System.CodeDom.Compiler;
using System.Collections.Generic;
using _2calgo.Parser.Errors;
using System.Linq;

namespace _2calgo.Library
{
    public class ConvertionResult
    {
        public IEnumerable<ParsingError> ParsingErrors { get; private set; }
        public IEnumerable<CompilerError> CompilerErrors { get; private set; }
        public string CAlgoCode { get; private set; }
        
        public ConvertionResult(string cAlgoCode, IEnumerable<ParsingError> parsingErrors, IEnumerable<CompilerError> compilerErrors)
        {
            ParsingErrors = parsingErrors;
            CompilerErrors = compilerErrors;
            CAlgoCode = cAlgoCode;
        }

        public bool IsSuccessfull
        {
            get { return !(CompilerErrors.Any(e => !e.IsWarning) || ParsingErrors.Any(e => e.ErrorType >= ErrorType.Error)); }
        }
    }
}