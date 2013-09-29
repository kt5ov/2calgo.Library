using System.Collections.Generic;
using _2calgo.Model;
using _2calgo.Parser.Errors;

namespace _2calgo.Parser
{
    public class ParsingResult
    {
        public Algo Algo { get; private set; }
        public IEnumerable<ParsingError> ParsingErrors { get; private set; }

        public ParsingResult(Algo algo, IEnumerable<ParsingError> parsingErrors)
        {
            Algo = algo;
            ParsingErrors = parsingErrors;
        }
    }
}