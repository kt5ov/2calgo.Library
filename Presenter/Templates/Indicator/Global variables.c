const string GlobalVariablesPath = "Software\\2calgo\\Global Variables\\";
[Conditional("GlobalVariableSet")]
Mq4Double GlobalVariableSet(Mq4String name, Mq4Double value)
{
    var hkcu = Registry.CurrentUser;
    hkcu.CreateSubKey(GlobalVariablesPath);
    var globalVariablesKey = hkcu.OpenSubKey(GlobalVariablesPath, true);
    globalVariablesKey.SetValue(name, (double)value);    
    
    globalVariablesKey.Close();
    hkcu.Close();
    return 0;
}
[Conditional("GlobalVariableGet")]
Mq4Double GlobalVariableGet(Mq4String name)
{
    var hkcu = Registry.CurrentUser;
    hkcu.CreateSubKey(GlobalVariablesPath);
    var globalVariablesKey = hkcu.OpenSubKey(GlobalVariablesPath, false);
    var value = globalVariablesKey.GetValue(name, 0d);    
    
    globalVariablesKey.Close();
    hkcu.Close();

    return double.Parse((string)value);
}
[Conditional("GlobalVariableCheck")]
Mq4Double GlobalVariableCheck(Mq4String name)
{
    var hkcu = Registry.CurrentUser;
    hkcu.CreateSubKey(GlobalVariablesPath);
    var globalVariablesKey = hkcu.OpenSubKey(GlobalVariablesPath, false);
    var isExisting = globalVariablesKey.GetValue(name) != null;    
    
    globalVariablesKey.Close();
    hkcu.Close();

    return isExisting;
}
[Conditional("GlobalVariableDel")]
Mq4Double GlobalVariableDel(Mq4String name)
{
    var hkcu = Registry.CurrentUser;
    hkcu.CreateSubKey(GlobalVariablesPath);
    var globalVariablesKey = hkcu.OpenSubKey(GlobalVariablesPath, true);
    globalVariablesKey.DeleteValue(name);    
    
    globalVariablesKey.Close();
    hkcu.Close();

    return true;
}
[Conditional("GlobalVariableName")]
Mq4String GlobalVariableName(int index)
{
    var hkcu = Registry.CurrentUser;
    hkcu.CreateSubKey(GlobalVariablesPath);
    var globalVariablesKey = hkcu.OpenSubKey(GlobalVariablesPath, false);
    string[] allVariables = globalVariablesKey.GetValueNames();

    globalVariablesKey.Close();
    hkcu.Close();

    return allVariables[index];
}
[Conditional("GlobalVariablesTotal")]
Mq4Double GlobalVariablesTotal()
{
    var hkcu = Registry.CurrentUser;
    hkcu.CreateSubKey(GlobalVariablesPath);
    var globalVariablesKey = hkcu.OpenSubKey(GlobalVariablesPath, false);
    string[] allVariables = globalVariablesKey.GetValueNames();

    globalVariablesKey.Close();
    hkcu.Close();

    return allVariables.Length;
}