using System.Collections.Generic;

namespace _2calgo.Parser.Errors
{
    class ContainsStringErrorDetector : ErrorDetector
    {
        private readonly string _stringToFind;
        private readonly ErrorType _errorType;

        public ContainsStringErrorDetector(string stringToFind, ErrorType errorType)
        {
            _stringToFind = stringToFind;
            _errorType = errorType;
        }
        
        public override IEnumerable<ParsingError> FindErrors(string code)
        {
            if (code.Contains(_stringToFind))
                yield return new ParsingError(_errorType, _stringToFind, null);
        }
    }
}