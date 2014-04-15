	[Conditional("Alert")]
    //{
    //AccessRights = AccessRights.FullAccess
	
	void Alert(params object[] objects)
    {
    	if (IsBacktesting)
    		return; 
    		
        var text = string.Join("", objects.Select(o => o.ToString()));      
        _alertWindowWrapper.Value.ShowAlert(text);
        PlayAlertSoundIfNeeded();
    }

	Lazy<AlertWindowWrapper> _alertWindowWrapper = new Lazy<AlertWindowWrapper>(() => 
	{
		return new AlertWindowWrapper();
	});
    
    private const string _alertSound = @"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";
    void PlayAlertSoundIfNeeded()
    {
        if (!IsLastBar || IsTesting() || (DateTime.Now - _lastPlayedTime) < TimeSpan.FromSeconds(1))
            return;

        _lastPlayedTime = DateTime.Now;
        _mediaPlayer = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationCoreAssembly, "System.Windows.Media.MediaPlayer");

        var soundFilePath = Path.Combine(FolderPaths._2calgoAppDataFolder, "alert_sound.mp3");
        if (!File.Exists(soundFilePath))
        {
            var binaryData = Convert.FromBase64String(_alertSound);
            File.WriteAllBytes(soundFilePath, binaryData);
        }
        var uri = new Uri(soundFilePath);

        ReflectionHelper.InvokeMethod(_mediaPlayer, "Open", uri);
        ReflectionHelper.InvokeMethod(_mediaPlayer, "Play");
    }

    private DateTime _lastPlayedTime;
    private object _mediaPlayer;

	public class AlertWindowWrapper : MarshalByRefObject
    {
        private static Thread _windowThread;
        private static readonly AutoResetEvent LoadedEvent = new AutoResetEvent(false);
        private static readonly object SyncObject = new object();

        private static object _window;
        private static AlertWindowModel _alertWindowModel;

        public void ShowAlert(string message)
        {
            lock (SyncObject)
            {
                if (_window == null)
                    CreateWindow();

                _alertWindowModel.Message = message;
                var items = new List<AlertItem>(_alertWindowModel.Items);
                items.Insert(0, new AlertItem(DateTime.Now, message));
                _alertWindowModel.Items = items;
            }
        }

        private void CreateWindow()
        {
            _windowThread = new Thread(() =>
            {
                try
                {
                    _window = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.Window");
                    SubscribeToEvent(_window, "Loaded", "Window_Loaded");
                    SubscribeToEvent(_window, "Closing", "Window_Closing");
                    ReflectionHelper.SetProperty(_window, "ShowInTaskbar", true);
                    ReflectionHelper.SetProperty(_window, "ShowActivated", true);
                    ReflectionHelper.SetProperty(_window, "Width", 525);
                    ReflectionHelper.SetProperty(_window, "Height", 400);
                    ReflectionHelper.SetProperty(_window, "WindowStartupLocation", ReflectionHelper.GetStaticValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.WindowStartupLocation", "CenterScreen"));
                    ReflectionHelper.SetProperty(_window, "WindowStyle", ReflectionHelper.GetStaticValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.WindowStyle", "ToolWindow"));
                    ReflectionHelper.SetProperty(_window, "Title", "Alert");
                    var rootGrid = WpfReflectionHelper.CreateGrid();
                    WpfReflectionHelper.SetMargin(rootGrid, 5);
                    WpfReflectionHelper.AddAutoRowDefinition(rootGrid);
                    WpfReflectionHelper.AddStarRowDefinition(rootGrid);

                    var messageGrid = WpfReflectionHelper.CreateGrid();
                    WpfReflectionHelper.AddAutoColumnDefinition(messageGrid);
                    WpfReflectionHelper.AddStarColumnDefinition(messageGrid);

                    var image = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly,
                                                                "System.Windows.Controls.Image");
                    WpfReflectionHelper.SetMargin(image, 20);
                    var bitmapImage = Base64ToImage("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");
                    ReflectionHelper.SetProperty(image, "Source", bitmapImage);
                    WpfReflectionHelper.AddChild(messageGrid, image);

                    var messageTextBlock =
                        ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly,
                                                        "System.Windows.Controls.TextBlock");

                    WpfReflectionHelper.SetGridColumn(messageTextBlock, 1);
                    ReflectionHelper.SetProperty(messageTextBlock, "HorizontalAlignment", ReflectionHelper.GetEnumValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.HorizontalAlignment", "Left"));
                    ReflectionHelper.SetProperty(messageTextBlock, "TextWrapping", ReflectionHelper.GetEnumValue(WpfReflectionHelper.PresentationCoreAssembly, "System.Windows.TextWrapping", "Wrap"));
                    ReflectionHelper.SetProperty(messageTextBlock, "VerticalAlignment", ReflectionHelper.GetEnumValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.VerticalAlignment", "Center"));
                    ReflectionHelper.SetProperty(messageTextBlock, "FontSize", 18);
                    WpfReflectionHelper.SetBinding(messageTextBlock, "Message", "System.Windows.Controls.TextBlock", "TextProperty");
                    WpfReflectionHelper.AddChild(messageGrid, messageTextBlock);

                    var allAlertsGrid = WpfReflectionHelper.CreateGrid();
                    WpfReflectionHelper.SetGridRow(allAlertsGrid, 1);
                    WpfReflectionHelper.AddStarRowDefinition(allAlertsGrid);
                    WpfReflectionHelper.AddAutoRowDefinition(allAlertsGrid);
                    var dataGrid = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly,
                                                                   "System.Windows.Controls.DataGrid");
                    WpfReflectionHelper.SetBinding(dataGrid, "Items", "System.Windows.Controls.ItemsControl", "ItemsSourceProperty");
                    WpfReflectionHelper.AddChild(allAlertsGrid, dataGrid);
                    var closeButton = ReflectionHelper.CreateInstance(
                        WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.Controls.Button");
                    SubscribeToEvent(closeButton, "Click", "ButtonOnClick");
                    WpfReflectionHelper.SetGridRow(closeButton, 1);
                    ReflectionHelper.SetProperty(closeButton, "Width", 100);
                    WpfReflectionHelper.SetMargin(closeButton, 0, 5, 0, 0);
                    ReflectionHelper.SetProperty(closeButton, "Content", "Close");
                    WpfReflectionHelper.AddChild(allAlertsGrid, closeButton);
                    
                    WpfReflectionHelper.AddChild(rootGrid, messageGrid);
                    WpfReflectionHelper.AddChild(rootGrid, allAlertsGrid);
                    
                    ReflectionHelper.SetProperty(_window, "Content", rootGrid);
                    _alertWindowModel = new AlertWindowModel();
                    ReflectionHelper.SetProperty(_window, "DataContext", _alertWindowModel);
                    
                    ReflectionHelper.InvokeMethod(_window, "ShowDialog");
                }
                catch (Exception exception)
                {                    
                }
            }
                )
            {
                IsBackground = false,
                Name = "Alert Window thread"
            };

            _windowThread.TrySetApartmentState(ApartmentState.STA);
            _windowThread.Start();
            LoadedEvent.WaitOne();
        }

        private static object Base64ToImage(string stringValue)
        {

            var binaryData = Convert.FromBase64String(stringValue);

            var bitmapImage = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationCoreAssembly,
                                                              "System.Windows.Media.Imaging.BitmapImage");
            
            ReflectionHelper.InvokeMethod(bitmapImage, "BeginInit");
            ReflectionHelper.SetProperty(bitmapImage, "StreamSource", new MemoryStream(binaryData));
            ReflectionHelper.InvokeMethod(bitmapImage, "EndInit");

            return bitmapImage;
        }

        public void ButtonOnClick(object sender, object routedEventArgs)
        {
            ReflectionHelper.InvokeMethod(_window, "Close");
        }

        public void Window_Loaded(object s, object e)
        {
            LoadedEvent.Set();
        }

        public void Window_Closing(object sender, CancelEventArgs e)
        {
            _window = null;
            _alertWindowModel = null;
        }

        private void SubscribeToEvent(object @object, string eventName, string handlerName)
        {
            var eventInfo = @object.GetType().GetEvent(eventName);
            var methodInfo = GetType().GetMethod(handlerName);
            var handler = Delegate.CreateDelegate(eventInfo.EventHandlerType, this, methodInfo);
            eventInfo.AddEventHandler(@object, handler);
        }
    }

    public class AlertWindowModel : INotifyPropertyChanged
    {
        private string _message;
        private IEnumerable<AlertItem> _items = new List<AlertItem>();

        public string Message
        {
            get { return _message; }
            set
            {
                if (_message == value)
                    return;

                _message = value;
                OnPropertyChanged("Message");
            }
        }

        public IEnumerable<AlertItem> Items
        {
            get { return _items; }
            set
            {
                if (_items == value)
                    return;

                _items = value;
                OnPropertyChanged("Items");
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            var handler = PropertyChanged;
            if (handler != null)
                handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    public class AlertItem
    {
        public DateTime Time { get; private set; }
        public string Message { get; private set; }

        public AlertItem(DateTime time, string message)
        {
            Time = time;
            Message = message;
        }
    }

    public static class WpfReflectionHelper
    {
        public static Assembly PresentationFrameworkAssembly = Assembly.Load("PresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35");
        public static Assembly PresentationCoreAssembly = Assembly.Load("PresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35");
        
        public static void SetMargin(object rootGrid, params object[] parameters)
        {
            var thickness = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.Thickness", parameters);
            ReflectionHelper.SetProperty(rootGrid, "Margin", thickness);
        }

        public static object CreateGrid()
        {
            return ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.Controls.Grid");
        }

        public static void AddChild(object grid, object child)
        {
            var rootGridChildren = ReflectionHelper.GetPropertyValue(grid, "Children");
            ReflectionHelper.InvokeMethod(rootGridChildren, "Add", child);
        }

        private static readonly object AutoGridLegth = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly,
                                                                                       "System.Windows.GridLength", "Auto");

        public static void AddAutoRowDefinition(object grid)
        {
            var rowDefinition = CreateRowDefinition();
            ReflectionHelper.SetProperty(rowDefinition, "Height", AutoGridLegth);
            AddRowDefinition(grid, rowDefinition);
        }

        public static void AddStarRowDefinition(object grid)
        {
            var rowDefinition = CreateRowDefinition();
            var starUnitType = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly,
                                                               "System.Windows.GridUnitType", "Star");
            var gridLength = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.GridLength", new[]{ 1, starUnitType});
            ReflectionHelper.SetProperty(rowDefinition, "Height", gridLength);
            AddRowDefinition(grid, rowDefinition);
        }

        private static object CreateRowDefinition()
        {
            var rowDefinition = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly,
                                                                "System.Windows.Controls.RowDefinition");
            return rowDefinition;
        }

        public static void AddRowDefinition(object grid, object rowDefinition)
        {
            var rowDefinitions = ReflectionHelper.GetPropertyValue(grid, "RowDefinitions");
            ReflectionHelper.InvokeMethod(rowDefinitions, "Add", rowDefinition);
        }

        public static void AddAutoColumnDefinition(object grid)
        {
            var ColumnDefinition = CreateColumnDefinition();
            ReflectionHelper.SetProperty(ColumnDefinition, "Width", AutoGridLegth);
            AddColumnDefinition(grid, ColumnDefinition);
        }

        public static void AddStarColumnDefinition(object grid)
        {
            var ColumnDefinition = CreateColumnDefinition();
            var starUnitType = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly,
                                                               "System.Windows.GridUnitType", "Star");
            var gridLength = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.GridLength", new[]{ 1, starUnitType});
            ReflectionHelper.SetProperty(ColumnDefinition, "Width", gridLength);
            AddColumnDefinition(grid, ColumnDefinition);
        }

        private static object CreateColumnDefinition()
        {
            var ColumnDefinition = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly,
                                                                "System.Windows.Controls.ColumnDefinition");
            return ColumnDefinition;
        }

        public static void AddColumnDefinition(object grid, object ColumnDefinition)
        {
            var ColumnDefinitions = ReflectionHelper.GetPropertyValue(grid, "ColumnDefinitions");
            ReflectionHelper.InvokeMethod(ColumnDefinitions, "Add", ColumnDefinition);
        }

        public static void SetGridColumn(object element, int column)
        {
            var columnProperty = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly, "System.Windows.Controls.Grid", "ColumnProperty");
            ReflectionHelper.InvokeMethod(element, "SetValue", columnProperty, column);
        }

        public static void SetGridRow(object element, int row)
        {
            var columnProperty = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly, "System.Windows.Controls.Grid", "RowProperty");
            ReflectionHelper.InvokeMethod(element, "SetValue", columnProperty, row);
        }

        public static void SetBinding(object element, string path, string propertyTypeName, string propertyName)
        {
            var property = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly, propertyTypeName, propertyName);
            ReflectionHelper.InvokeMethod(element, "SetBinding", property, path);
        }
    }

    public static class ReflectionHelper
    {
        public static object GetEnumValue(Assembly assembly, string typeName, string value)
        {
            var type = assembly.GetType(typeName);
            return Enum.Parse(type, value);
        }

        public static void InvokeMethod(object instance, string methodName, params object[] parameters)
        {
            instance.GetType().InvokeMember(methodName, BindingFlags.InvokeMethod, null, instance, parameters);
        }

        public static object InvokeStaticMethod(Assembly assembly, string typeName, string methodName, params object[] parameters)
        {
            return assembly.GetType(typeName).InvokeMember(methodName, BindingFlags.InvokeMethod, null, null, parameters);
        }

        public static object GetStaticValue(Assembly assembly, string typeName, string propertyName)
        {
            var type = assembly.GetType(typeName);
            var propertyInfo = type.GetProperty(propertyName);
            if (propertyInfo != null)
                return propertyInfo.GetValue(null, null);

            var fieldInfo = type.GetField(propertyName);
            return fieldInfo.GetValue(null);
        }

        public static void SetProperty(object instance, string propertyName, Assembly assembly, string typeName, params object[] parameters)
        {
            var propertyInfo = instance.GetType().GetProperty(propertyName);
            var propertyValue = CreateInstance(assembly, typeName, parameters);
            propertyInfo.SetValue(instance, propertyValue, null);
        }

        public static void SetProperty(object instance, string propertyName, object value)
        {
            var propertyInfo = instance.GetType().GetProperty(propertyName);
            propertyInfo.SetValue(instance, value, null);
        }

        public static object GetPropertyValue(object instance, string propertyName)
        {
            var propertyInfo = instance.GetType().GetProperty(propertyName);
            return propertyInfo.GetValue(instance, null);
        }

        public static object CreateInstance(Assembly assembly, string typeName)
        {
            return CreateInstance(assembly, typeName, new object[0]);
        }

        public static object CreateInstance(Assembly assembly, string typeName, object[] parameters)
        {
            var pointType = assembly.GetType(typeName);
            var propertyValue = Activator.CreateInstance(pointType, parameters);
            return propertyValue;
        }
    }
	//}