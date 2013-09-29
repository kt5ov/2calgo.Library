using System.Text;

namespace _2calgo.Presenter
{
    public abstract class AlgoBuilder
    {
        public readonly StringBuilder Parameters = new StringBuilder();
        public readonly StringBuilder DebugActions = new StringBuilder();
        public readonly StringBuilder HandleException = new StringBuilder();
        public readonly StringBuilder References = new StringBuilder();
        public string Fields { get; set; }
        public string Mq4Functions { get; set; }
        public string Mq4Code { get; set; }
        public string AlgoName { get; set; }

        protected string Build(string template)
        {
            var code = TemplateOptimizer.RemoveUnusedCode(template, new Words(Mq4Code));

            code = code.Replace("#AlgoName_PLACE_HOLDER#", AlgoName);
            code = code.Replace("#Parameters_PLACE_HOLDER#", Parameters.ToString());
            code = code.Replace("#Mq4Functions_PLACE_HOLDER#", Mq4Functions);
            code = code.Replace("#Mq4Fields_PLACE_HOLDER#", Fields);
            code = code.Replace("#DebugActions_PLACE_HOLDER#", DebugActions.ToString());
            code = code.Replace("#HandleException_PLACE_HOLDER#", HandleException.ToString());
            code = code.Replace("#References_PLACE_HOLDER#", References.ToString());
            return code;
        }

        public abstract string Build();
    }
}