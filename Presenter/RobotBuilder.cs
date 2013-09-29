using _2calgo.Model;

namespace _2calgo.Presenter
{
    class RobotBuilder : AlgoBuilder
    {
        public override string Build()
        {
            var template = TemplateProvider.GetTemplate(AlgoType.Robot);
            var code = Build(template);

            return code;
        }
    }
}