namespace _2calgo.Model
{
    public class Parameter
    {
        public string Type { get; private set; }
        public string BackfieldType { get; private set; }
        public string Name { get; private set; }
        public string DefaultValue { get; private set; }

        public Parameter(string type, string name, string defaultValue, string backfieldType)
        {
            Type = type;
            Name = name;
            DefaultValue = defaultValue;
            BackfieldType = backfieldType;
        }
    }
}