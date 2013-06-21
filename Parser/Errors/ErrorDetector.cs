using System.Collections.Generic;

namespace _2calgo.Parser.Errors
{
    internal abstract class ErrorDetector
    {
        public abstract IEnumerable<ParsingError> FindErrors(string code);
    }
}