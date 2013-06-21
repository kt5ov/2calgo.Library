namespace _2calgo.Parser.InnerStructures
{
    public class StringStructure
    {
        public bool IsInsideString { get; private set; }
        
        public void Handle(char @char)
        {
            if (@char == '"')
                IsInsideString = !IsInsideString;
        }
    }
}