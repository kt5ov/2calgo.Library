namespace _2calgo.Parser.InnerStructures
{
    public class InnerStructuresHandler
    {
        public bool IsInsideSomeStructure { get; private set; }

        private int _openedBrackets;
        private int _openedFigureBrackets;
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

            IsInsideSomeStructure = _openedBrackets > 0 || _stringStructure.IsInsideString || _openedFigureBrackets > 0;
        }
    }
}