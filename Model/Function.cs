namespace _2calgo.Model
{
    public class Function
    {
        public string ReturnType { get; set; }
        public FunctionParameter[] Parameters { get; set; }
        public string Name { get; set; }
        public string Body { get; set; }

        public Function(string returnType, string name, FunctionParameter[] parameters, string body)
        {
            ReturnType = returnType;
            Parameters = parameters;
            Name = name;
            Body = body;
        }
    }
}