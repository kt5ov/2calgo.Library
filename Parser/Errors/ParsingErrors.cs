using System.Collections.Generic;

namespace _2calgo.Parser.Errors
{
    public class ParsingErrors
    {
        private readonly List<ParsingError> _errors;

        public ParsingErrors()
        {
            _errors = new List<ParsingError>();
        }

        public IEnumerable<ParsingError> Errors
        {
            get { return _errors; }
        }

        public void Add(ErrorType errorType, string message, string details = "")
        {
            _errors.Add(new ParsingError(errorType, message, details));
        }

        public void Add(ParsingError error)
        {
            _errors.Add(error);
        }
    }
}