namespace _2calgo.Model
{
    public class Property
    {
        public string Name { get; private set; }
        public string Value { get; private set; }

        public Property(string name, string value)
        {
            Name = name;
            Value = value;
        }
    }
}