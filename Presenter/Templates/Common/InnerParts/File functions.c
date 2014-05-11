int FILE_READ = 1;
int FILE_WRITE = 2;
//int FILE_BIN = 8;
int FILE_CSV = 8;

int SEEK_END = 2;

class FileInfo
{
	public int Mode { get; set; }
	public int Handle { get; set; }
	public char Separator { get; set; }
	public string FileName { get; set; }
	public List<string> PendingParts { get; set; }
	public StreamWriter StreamWriter { get; set; }
	public StreamReader StreamReader { get; set; }
}

private Dictionary<int, FileInfo> _openedFiles = new Dictionary<int, FileInfo>();
private int _handleCounter = 1000;

[Conditional("FileOpen")]
Mq4Double FileOpen(Mq4String filename, Mq4Double mode)
{
    //AccessRights = AccessRights.FileSystem
	return FileOpen(filename, mode, 59);
}

[Conditional("FileOpen")]
Mq4Double FileOpen(Mq4String filename, Mq4Double mode, Mq4Char separator)
{
	//AccessRights = AccessRights.FileSystem
	var fullFileName = Path.Combine(FolderPaths._2calgoDesktopFolder, filename);
	int handle = 0;
	try
	{
		var directoryPath = Path.GetDirectoryName(fullFileName);
        if (!Directory.Exists(directoryPath))
            Directory.CreateDirectory(directoryPath);

        var intMode = (int)mode;
        if ((intMode & FILE_WRITE) > 0)
        {
            var streamWriter = File.CreateText(fullFileName);
            handle = _handleCounter++;
            var fileInfo = new FileInfo 
            {
                Handle = handle,
                Mode = mode,
                FileName = fullFileName,
                StreamWriter = streamWriter,
                Separator = separator
            };
            _openedFiles.Add(handle, fileInfo);
        }
        else if ((intMode & FILE_READ) > 0)
        {
            var streamReader = new StreamReader(fullFileName);
            handle = _handleCounter++;
            var fileInfo = new FileInfo 
            {
                Handle = handle,
                Mode = mode,
                FileName = fullFileName,
                StreamReader = streamReader,
                Separator = separator
            };
            _openedFiles.Add(handle, fileInfo);
        }
	}
	catch (Exception e)
	{
		Print("Can not open file: " + e.ToString());
		#HandleException_PLACE_HOLDER#
	}
	return handle;
}

[Conditional("FileWrite")]
Mq4Double FileWrite(Mq4Double handle, params object[] parameters) 
{
	//AccessRights = AccessRights.FileSystem
	FileInfo fileInfo;
	if (!_openedFiles.TryGetValue(handle, out fileInfo))
		return -1;
	try
	{
		var stringToWrite = string.Join(fileInfo.Separator.ToString(), parameters.Select(p => p.ToString()));
		fileInfo.StreamWriter.WriteLine(stringToWrite);
		return stringToWrite.Length;
	}
	catch (Exception e)
	{
		Print("Can not write to file file: " + e.ToString());
		#HandleException_PLACE_HOLDER#
		return -1;
	}
}

[Conditional("FileReadString")]
Mq4String FileReadString(Mq4Double handle)
{
	//AccessRights = AccessRights.FileSystem
	try
	{
		FileInfo fileInfo;
		if (!_openedFiles.TryGetValue(handle, out fileInfo))
			return string.Empty;

		if (fileInfo.PendingParts == null || fileInfo.PendingParts.Count == 0)
			fileInfo.PendingParts = fileInfo.StreamReader.ReadLine().Split(fileInfo.Separator).ToList();
	
		if (fileInfo.PendingParts.Count == 0)
			return string.Empty;

		var result = fileInfo.PendingParts[0];
		fileInfo.PendingParts.RemoveAt(0);
		return result;
	}
	catch (Exception e)
	{
		Print("Can not read string from file: " + e.ToString());
		#HandleException_PLACE_HOLDER#
		return string.Empty;
	}
}

[Conditional("FileClose")]
void FileClose(Mq4Double handle) 
{
	//AccessRights = AccessRights.FileSystem
	try
	{
		FileInfo fileInfo;
		if (!_openedFiles.TryGetValue(handle, out fileInfo))
			return;

		_openedFiles.Remove(handle);
		if (fileInfo.StreamWriter != null)
			fileInfo.StreamWriter.Close();
		if (fileInfo.StreamReader != null)
			fileInfo.StreamReader.Close();
	}
	catch (Exception e)
	{
		Print("Can not close file: " + e.ToString());
		#HandleException_PLACE_HOLDER#
	}
}

[Conditional("FileSeek")]
Mq4Double FileSeek(Mq4Double handle, Mq4Double offset, Mq4Double origin) 
{
	//AccessRights = AccessRights.FileSystem
	try
	{
		FileInfo fileInfo;
		if (!_openedFiles.TryGetValue(handle, out fileInfo) || fileInfo.StreamWriter == null)
			return false;
		if ((int)offset != 0 || origin != SEEK_END)
			return false;

		fileInfo.StreamWriter.Close();
		fileInfo.StreamWriter = new StreamWriter(fileInfo.FileName, true);

		return true;
	}
	catch (Exception e)
	{
		Print("Can not seek in file: " + e.ToString());
		#HandleException_PLACE_HOLDER#
		return false;
	}
}

[Conditional("FileFlush")]
void FileFlush(Mq4Double handle)
{
	//AccessRights = AccessRights.FileSystem
	try
	{
		FileInfo fileInfo;
		if (!_openedFiles.TryGetValue(handle, out fileInfo) || fileInfo.StreamWriter == null)
			return;

		fileInfo.StreamWriter.Flush();
	}
	catch (Exception e)
	{
		Print("Can not flush file: " + e.ToString());

		#HandleException_PLACE_HOLDER#
	}
}

[Conditional("FileDelete")]
void FileDelete(Mq4String filename) 
{
	//AccessRights = AccessRights.FileSystem
	try
	{
		File.Delete(filename);
	}
	catch (Exception e)
	{
		Print("Can not delete file: " + e.ToString());

		#HandleException_PLACE_HOLDER#
	}
}