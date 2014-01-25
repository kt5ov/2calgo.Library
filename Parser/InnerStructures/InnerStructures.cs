using System;

namespace _2calgo.Parser.InnerStructures
{
    public class InnerStructures
    {
        public bool IsInsideSomeStructure { get; private set; }

        private int _openedBrackets;
        private int _openedFigureBrackets;
        private int _openedSquareBrackets;
        private readonly StringStructure _stringStructure = new StringStructure();

        public void Handle(char @char)
        {
            _stringStructure.Handle(@char);
            if (_stringStructure.IsInsideString)
            {
                IsInsideSomeStructure = true;
                return;
            }

            if (@char == '(')
                _openedBrackets++;
            if (@char == ')')
                _openedBrackets--;
            if (@char == '{')
                _openedFigureBrackets++;
            if (@char == '}')
                _openedFigureBrackets--;
            if (@char == '[')
                _openedSquareBrackets++;
            if (@char == ']')
                _openedSquareBrackets--;

            if (_openedBrackets < 0 || _openedFigureBrackets < 0 || _openedSquareBrackets < 0)
                throw new Exception("Incorrect sequence of brackets");

            IsInsideSomeStructure = _openedBrackets > 0 || _stringStructure.IsInsideString || _openedFigureBrackets > 0 || _openedSquareBrackets > 0;
        }
    }
}