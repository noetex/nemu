#if 0
static void*
memcpy(void* Destination, void* Source, size_t NumBytes)
{
	//bool_t Overlapping = (Destination - Source) < NumBytes;
	//if(Overlapping)
	while(NumBytes > 0)
	{
		*(char*)Destination = *(char*)Source;
		(char*)Destination += 1;
		(char*)Source += 1;
		NumBytes -= 1;
	}
	return Destination;
}

static void*
memset(void* Destination, int Value, size_t NumBytes)
{
	while(NumBytes > 0)
	{
		*(char*)Destination = (char)Value;
		(char*)Destination += 1;
		NumBytes -= 1;
	}
	return Destination;
}
#endif

static void*
pointer_align(void* Data, size_t Alignment)
{
	size_t Mask = Alignment - 1;
	intptr_t Temp = (intptr_t)Data;
	void* Result = (void*)((Temp + Mask) & ~Mask);
	return Result;
}
