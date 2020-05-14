/*!
\file TypeInfo.h
\brief metadata
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <typeindex>
#include <type_traits>
#include <string>

//!metadata, yeah
struct TypeInfo
{
public:

	//type of C++ class
	std::type_index cppType;

	int ptr;

	//!if not null, it's a LizardScript class, so you shouldn't look at cppType, it's empty
	std::string* lsOwnClassName;

public:

	//!size of the object, not of the pointer to it
	int byValueSize;

public:

	//!TypeInfo of void
	TypeInfo() : cppType(typeid(void))
	{
		ptr = 0;
		byValueSize = 0;
		lsOwnClassName = nullptr;
	}

	//!get size of this type instanse or size of a pointer if it's a pointer
	size_t size() const
	{
		return ptr == 0 ? byValueSize : sizeof(void*);
	}

	//equality of types (reference to type isn't equal to type)
	bool full_eq(const TypeInfo& info)
	{
		return (*this == info) && (ptr == info.ptr);
	}

	//TypeInfo withPtr(int ptr)
	//{
	//	TypeInfo t = *this;
	//	t.ptr = ptr;
	//	return t;
	//}

	//template <typename T>
	//bool is() const
	//{
	//	return cppType == typeid(T);
	//}

	//!compare types
	//!it's necessary to store them in a map
	//!\warning LizardScript class is always > C++ class
	bool operator<(const TypeInfo& other) const
	{
		if (lsOwnClassName == nullptr)
		{
			if (other.lsOwnClassName == nullptr)
				return cppType < other.cppType;
			else return false;
		}
		else
		{
			if (other.lsOwnClassName == nullptr)
				return true; 
			else return lsOwnClassName < other.lsOwnClassName;
		}
	}

	//!checks if types are equal. pointer to type is equal to type. if you need full equality, use full_eq
	bool operator==(const TypeInfo& other) const
	{
		return lsOwnClassName == other.lsOwnClassName && cppType == other.cppType;
	}

	//!checks if types aren't equal. pointer to type is equal to type. if you need full equality, use full_eq
	bool operator!=(const TypeInfo& other) const
	{
		return !operator==(other);
	}

	//!get text representation of type
	//\warning I should optimize it
	std::string to_string(bool addPtrSymbol = true) const
	{
		std::string s;

		s = lsOwnClassName == nullptr ? cppType.name() : *lsOwnClassName;

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

//!some templates to create TypeInfo for objects, pointers, references and lvalue-references
namespace TypeInfoCreationImpl
{
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

	template <typename T>
	struct NoArgs
	{
		static void create(TypeInfo& t)
		{
			t.cppType = typeid(T);
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
}

//!get text representation of type
//!it's similar to std::to_string (and in some templates we can use it)
inline std::string to_string(const TypeInfo& info)
{
	return info.to_string();
}

//!\param p count of '*'
//!\returns TypeInfo of type T*...* ('*' - p times)
template <typename T>
TypeInfo typeInfo(int p = 0)
{
	TypeInfo info;
	TypeInfoCreationImpl::NoArgs<T>::create(info);
	info.ptr = p;
	return info;
}

//#define TYPEINFO(T, ...) makeTypeInfo<T>(__VA_ARGS__)