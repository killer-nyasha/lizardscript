#pragma once

inline const TypeInfo findType(TCHAR* name)
{
	int ptr = 0;
	TCHAR* ch = name;
	while (*ch != 0 && *ch != '*')
		ch++;
	while (*ch != 0)
	{
		*(ch++) = 0; ptr++;
	}

	for (auto& m : globalMetadataTable)
	{
		std::string t = m.first.text(false);
		if (t == name)
		{
			TypeInfo ret = m.first;
			ret.ptr = ptr;
			return ret;
		}
	}
	throw Exception(std::string("Unknown identifier ") + name + "");
}