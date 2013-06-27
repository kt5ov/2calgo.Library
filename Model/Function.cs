namespace _2calgo.Model
{
    public class Function
    {
        public string Name { get; set; }
        public string Declaration { get; set; }
        public string Body { get; set; }

        public Function(string name, string declaration, string body)
        {
            Name = name;
            Declaration = declaration;
            Body = body;
        }
    }
}