#include "FileManager.h"

void FileManager::LoadFile(const wchar_t* filepath)
{
	std::wifstream fs(filepath);
	try
	{
		if (!fs.is_open())
		{
			throw FileFailedToOpen("err: couldnt open file");
		}
		wchar_t c;
		fs.get(c);
		cursor_a = Cursor<wchar_t>(c);
		cursor_b = cursor_a;
		while (fs.get(c))
		{
			cursor_b.Insert(c);
		}

	}
	catch (FileFailedToOpen)
	{
		// idk return failed or smth
	}
	fs.close();
}

std::wstring FileManager::GetString()
{
	std::wstringstream ss;
	Cursor<wchar_t> cursor_it = cursor_a;
	while (cursor_it.Get())
	{
		ss << *(cursor_it.Get()->element);
		cursor_it.Next();
	}
	return ss.str();
}
