namespace _2calgo.Model
{
    public class Function
    {
        public string Name { get; private set; }
        public string Declaration { get; private set; }
        public string Body { get; private set; }

        public Function(string name, string declaration, string body)
        {
            Name = name;
            Declaration = declaration;
            Body = body;
        }
    }
}