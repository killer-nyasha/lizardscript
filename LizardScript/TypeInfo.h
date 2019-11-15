#pragma once
#include <typeindex>
#include <type_traits>
#include <string>
//namespace std
//{
//	class string;
//}

struct TypeInfo
{
public:
	std::type_index t;
	int ptr;
	std::string* lsOwnClassName;

public:
	int byValueSize;

public:

	TypeInfo() : t(typeid(void))
	{
		ptr = 0;
		byValueSize = 0;
		lsOwnClassName = nullptr;
	}

	int size()
	{
		return ptr == 0 ? byValueSize : sizeof(void*);
	}

	int openedSize()
	{
		return (ptr-1) == 0 ? byValueSize : sizeof(void*);
	}

	bool full_eq(const TypeInfo& info)
	{
		return (*this == info) && (ptr == info.ptr);
	}

	TypeInfo withPtr(int ptr)
	{
		TypeInfo t = *this;
		t.ptr = ptr;
		return t;
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
	struct NoArgs<T&>
	{
		static void create(TypeInfo& t)
		{
			t.ptr++;
			NoArgs<T>::create(t);
		}
	};

	template <typename T>
	struct NoArgs<T&&>
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
		if (lsOwnClassName == nullptr)
		{
			if (other.lsOwnClassName == nullptr)
				return t < other.t;
			else return false;
		}
		else
		{
			if (other.lsOwnClassName == nullptr)
				return true; 
			else return lsOwnClassName < other.lsOwnClassName;
		}
	}

	bool operator==(const TypeInfo& other) const
	{
		return t == other.t && lsOwnClassName == other.lsOwnClassName;
	}

	bool operator!=(const TypeInfo& other) const
	{
		return !operator==(other);
	}

	std::string text(bool addPtrSymbol = true) const
	{
		std::string s;

		s = lsOwnClassName == nullptr ? t.name() : *lsOwnClassName;

		if (addPtrSymbol)
			for (size_t i = 0; i < ptr; i++)
				s += '&';

		size_t spacePos = s.find_first_of(' ', 0);
		if (spacePos != std::string::npos)
			s = s.substr(spacePos + 1);

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

template <typename T>
TypeInfo makeTypeInfo(int p)
{
	TypeInfo info;
	TypeInfo::NoArgs<T>::create(info);
	info.ptr = p;
	return info;
}

#define TYPEINFO(T, ...) makeTypeInfo<T>(__VA_ARGS__)

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