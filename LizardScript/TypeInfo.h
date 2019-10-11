#pragma once
#include <typeindex>
#include <type_traits>

struct TypeInfo
{
public:
	std::type_index t;
	int ptr;

private:
	int byValueSize;

public:

	TypeInfo() : t(typeid(void))
	{
		ptr = 0;
		byValueSize = 0;
	}

	int size()
	{
		return ptr == 0 ? byValueSize : sizeof(void*);
	}

	//template <typename T>
	//void create(T& object)
	//{
	//	t = typeid(object);
	//	size = sizeof(object);
	//}

	//template <typename T>
	//void create(T&& object)
	//{
	//	t = typeid(object);
	//	size = sizeof(object);
	//}

	//template <typename T>
	//void create(T*& object)
	//{
	//	ptr++;
	//	create(*object);
	//	
	//}

	//template <typename T>
	//void create(T*&& object)
	//{
	//	ptr++;
	//	create(*object);
	//}

	template <typename T>
	struct Wrapper
	{

	};

	template <typename T, typename K>
	struct SizeOf
	{

	};

	template <typename T, typename K>
	struct SizeOf<Wrapper<T>, K>
	{
		int get() { return sizeof(T); }
	};

	template <typename K>
	struct SizeOf<Wrapper<void>, K>
	{
		int get() { return 0; }
	};

	//size of pointer!!
	template <typename T>
	struct NoArgs
	{
		static void create(TypeInfo& t)
		{
			t.t = typeid(T);
			t.byValueSize = SizeOf<Wrapper<T>, int>().get();
		}
	};

	template <typename T>
	struct NoArgs<T*>
	{
		static void create(TypeInfo& t)
		{
			t.ptr++;
			NoArgs<T>::create(t);
		}
	};

	template <typename T>
	bool is() const
	{
		return t == typeid(T);
	}

	bool operator<(const TypeInfo& other) const
	{
		return t < other.t;
	}

	bool operator==(const TypeInfo& other) const
	{
		return t == other.t;
	}

	std::string text(bool addPtrSymbol = true) const
	{
		std::string s = t.name();
		if (addPtrSymbol)
			for (size_t i = 0; i < ptr; i++)
				s += '*';

		size_t spacePos = s.find_first_of(' ', 0);
		if (spacePos != std::string::npos)
			s = s.substr(spacePos+1);

		//сделать по-нормальному. а ещё тут может быть class и пространство имён.
		//	а может лучше просто там написать полное имя?
		//if (s[0] == 's' && s[1] == 't')
		//{
		//	s = s.substr(7);
		//}
		return s;
	}
};

template <typename T>
TypeInfo makeTypeInfo()
{
	TypeInfo info;
	TypeInfo::NoArgs<T>::create(info);
	return info;
}

//template <typename T>
//TypeInfo makeTypeInfo(T& object)
//{
//	TypeInfo info;
//	info.create(object);
//	return info;
//}
//
//template <typename T>
//TypeInfo makeTypeInfo(T&& object)
//{
//	TypeInfo info;
//	info.create(object);
//	return info;
//}