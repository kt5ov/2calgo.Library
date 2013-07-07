	interface IMq4Array<T>
	{		
		T this[int index] { get; set; }
		int Length { get; }
		bool IsInverted { get; }
		event Action<int, T> Changed;
	}