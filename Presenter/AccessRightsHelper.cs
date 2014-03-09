using System;
using System.Collections.Generic;
using System.Linq;
using cAlgo.API;

namespace _2calgo.Presenter
{
    public static class AccessRightsHelper
    {
        private static AccessRights[] Detect(string code)
        {
            var result = new List<AccessRights>();
            foreach (var accessRights in Enum.GetValues(typeof(AccessRights)).Cast<AccessRights>())
            {
                if (code.Contains(accessRights.ToString()))
                {
                    result.Add(accessRights);
                }
            }

            if (result.Contains(AccessRights.FullAccess))
                result = new List<AccessRights> {AccessRights.FullAccess};

            return result.ToArray();
        }

        public static string Add(string code)
        {
            var accessRights = Detect(code);
            var accessRightsStr = "AccessRights.None";
            if (accessRights.Any())
                accessRightsStr = string.Join(" | ", accessRights.Select(r => "AccessRights." + r));

            return code.Replace("#AccessRights_PLACE_HOLDER#", "AccessRights = " + accessRightsStr);
        }
    }
}