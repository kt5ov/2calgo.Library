namespace _2calgo.Presenter
{
    class RobotBuilder : AlgoBuilder
    {
        public override string Build()
        {
            var template = new RobotTemplateProvider().GetTemplate();
            var code = Build(template);

            return code;
        }
    }
}