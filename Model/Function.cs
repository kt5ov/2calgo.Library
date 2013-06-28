namespace _2calgo.Model
{
    public class Function
    {
        public string ReturnType { get; set; }
        public string[] Parameters { get; set; }
        public string Name { get; set; }
        public string Body { get; set; }

        public Function(string returnType, string name, string[] parameters, string body)
        {
            ReturnType = returnType;
            Parameters = parameters;
            Name = name;
            Body = body;
        }
    }
}