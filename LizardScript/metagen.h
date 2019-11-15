#pragma once
#include "TypeInfo.h"
#include "VectorsTuple.h"
#include "metagen_def.h"

#define FIELD(field) rawFieldInfo(&(sample->field), #field)
#define FUNC(...) rawFieldInfo(&__VA_ARGS__::
#define WITHNAME(fname) fname, #fname)
#define PARAMS(...) WithParams<__VA_ARGS__>

#define METAGEN_CLASS(className,...) ([](className* sample) { FromParents<__VA_ARGS__>::createMetadata((className*)nullptr
#define WITH_MEMBERS(...) __VA_ARGS__); })(nullptr)

#define REGISTERS_COUNT 18

namespace LizardScript
{
	struct ptrbuf
	{
		char buffer[sizeof(void*) * 3];
	};

	struct callstruct
	{
		char buffer[sizeof(void*) * 4];
	};

	struct AbstractCallStruct
	{
		virtual void call(void* registers[REGISTERS_COUNT], int n) { }
	};

	template <typename CA>
	struct ArgImpl
	{
		static CA _impl(void* arg)
		{
			return  *(CA*)(arg);
		}
	};

	template <typename CA>
	struct ArgImpl<CA&>
	{
		static CA& _impl(void* arg)
		{
			return  **(CA**)(arg);
		}
	};

	template <typename CA>
	struct ArgImpl<CA&&>
	{
		static CA&& _impl(void* arg)
		{
			return  std::move(*(CA*)(arg));
		}
	};

	template <typename O, typename R, typename... A>
	struct CallStruct : public AbstractCallStruct
	{
		R(O::*funcptr)(A...);
		virtual void call(void* registers[REGISTERS_COUNT], int n) override
		{
			int oldN = n;
			int i = sizeof...(A);
			O* ths = (O*)(registers[n]);

			if (sizeof(R) <= sizeof(void*))
				*(R*)(&registers[oldN]) = ((*ths).*funcptr)(ArgImpl<A>::_impl((void*)&(registers[n + i--]))...);
			else
			{
				*(R*)(registers[16]) = ((*ths).*funcptr)(ArgImpl<A>::_impl((void*)&(registers[n + i--]))...);
			}
		}
	};

	template <typename O, typename... A>
	struct CallStruct<O, void, A...> : public AbstractCallStruct
	{
		void(O::*funcptr)(A...);
		virtual void call(void* registers[REGISTERS_COUNT], int n) override
		{
			int i = sizeof...(A);
			O* ths = (O*)(registers[n]);
			((*ths).*funcptr)(ArgImpl<A>::_impl((void*)&registers[n + i--])...);
		}
	};

	template <typename O, typename... A>
	struct CtorProvider
	{
		O* ctor(A... args)
		{
			return new ((O*)this) O(args...);
		}
	};

	struct MemberInfo
	{
		TypeInfo type;
		std::string name;
	};

	struct FieldInfo : public MemberInfo
	{
		int offset;
	};

	struct FunctionInfo : public MemberInfo
	{
		//only for functions
		callstruct callStruct;
		//TypeInfo returnType;
		std::vector<TypeInfo> args;
	};

	struct TypeInfoEx : public TypeInfo
	{
		TypeInfoEx() { }

		TypeInfoEx(const TypeInfo& from)
		{
			*(TypeInfo*)(this) = from;
		}

		void operator+=(TypeInfoEx& e)
		{
			//for (auto& a : e.members.get<FieldInfo>())
				members.get<FieldInfo>().insert(members.get<FieldInfo>().begin(),
					e.members.get<FieldInfo>().begin(),
					e.members.get<FieldInfo>().end());

			//for (auto& a : e.members.get<FunctionInfo>())
				members.get<FunctionInfo>().insert(members.get<FunctionInfo>().begin(),
					e.members.get<FunctionInfo>().begin(),
					e.members.get<FunctionInfo>().end());

			//for (auto& a : e.members.get<FunctionInfo>())
			//	members.get<FunctionInfo>().push_back(a);

		}

		VectorsTuple<FieldInfo, FunctionInfo> members;
		std::vector<TypeInfo> parents;
		bool parentsProcessed = false;
	};

	extern std::map<TypeInfo, TypeInfoEx> globalMetadataTable;
}

#include "Print.h"

namespace LizardScript
{
	template <typename T>
	struct RawFieldInfo
	{
		void* offset;
		ptrbuf funcptr;
		std::string name;
	};

	template <typename T>
	inline RawFieldInfo<T> rawFieldInfo(T* ptr, std::string name)
	{
		RawFieldInfo<T> r;
		r.offset = (void*)ptr;
		r.name = name;
		return r;
	}

	template <typename R, typename O, typename... A>
	inline RawFieldInfo<R(O::*)(A...)> rawFieldInfo(R(O::*ptr)(A...), std::string name)
	{
		ptrbuf buffer;
		memcpy(&buffer, &ptr, sizeof(ptr) + sizeof(void*));
		return RawFieldInfo<R(O::*)(A...)> { (void*)-1, buffer, name };
	}

	template <typename... A>
	struct WithParams
	{
		template <typename R, typename O>
		static inline RawFieldInfo<R(O::*)(A...)> rawFieldInfo(R(O::*ptr)(A...), std::string name)
		{
			ptrbuf buffer;
			memcpy(&buffer, &ptr, sizeof(ptr) + sizeof(void*));
			return RawFieldInfo<R(O::*)(A...)> { (void*)-1, buffer, name };
		}

		template <typename R, typename O>
		static inline RawFieldInfo<R(O::*)(A...)> rawFieldInfo(R(O::*ptr)(A...) const, std::string name)
		{
			ptrbuf buffer;
			memcpy(&buffer, &ptr, sizeof(ptr) + sizeof(void*));
			return RawFieldInfo<R(O::*)(A...)> { (void*)-1, buffer, name };
		}

		//template <typename O>
		//static inline RawFieldInfo<void(O::*)(A...)> rawFieldInfo(void(O::*ptr)(A...), std::string name)
		//{
		//	ptrbuf buffer;
		//	memcpy(&buffer, &ptr, sizeof(ptr) + sizeof(void*));
		//	return RawFieldInfo<void(O::*)(A...)> { (void*)-1, buffer, name };
		//}

		//template <typename O>
		//static inline RawFieldInfo<void(O::*)(A...)> rawFieldInfo(void(O::*ptr)(A...) const, std::string name)
		//{
		//	ptrbuf buffer;
		//	memcpy(&buffer, &ptr, sizeof(ptr) + sizeof(void*));
		//	return RawFieldInfo<void(O::*)(A...)> { (void*)-1, buffer, name };
		//}
	};

	template <typename T>
	inline FieldInfo createMetadataEntry(RawFieldInfo<T> info)
	{
		FieldInfo f;
		f.type = makeTypeInfo<T>();
		f.name = info.name;
		f.offset = (int)info.offset;
		return f;
	}

	template <typename O, typename R, typename... A>
	inline FunctionInfo createMetadataEntry(RawFieldInfo<R(O::*)(A...)> info)
	{
		CallStruct<O, R, A...> callStruct;
		callStruct.funcptr = *reinterpret_cast<R(O::**)(A...)>(&info.funcptr);
		FunctionInfo f;
		f.type = makeTypeInfo<R>();
		f.args = { (makeTypeInfo<A>())... };
		f.name = info.name;
		memcpy(&f.callStruct, &callStruct, sizeof(callStruct));
		return f;
	}

	template <typename... P>
	class FromParents
	{
	public:
		//вынести общую логику
		template <typename O, typename... T>
		static inline void createMetadata(O* object)
		{
			std::vector<FieldInfo> metaTable;
			TypeInfo info = makeTypeInfo<O>();
			TypeInfoEx einfo = TypeInfoEx(info);
			int i[] = { (einfo.parents.push_back(TYPEINFO(P)), 0)... };
			globalMetadataTable.insert(std::make_pair(info, einfo));
		}

		template <typename O, typename... T>
		static inline void createMetadata(O* object, T... infos)
		{
			TypeInfo info = makeTypeInfo<O>();
			TypeInfoEx einfo = TypeInfoEx(info);
			int i[] = { (einfo.parents.push_back(TYPEINFO(P)), 0)... };
			char c[] = { (einfo.members.push_back(createMetadataEntry(infos)), '\0')... };
			globalMetadataTable.insert(std::make_pair(info, einfo));
		}
	};

	inline void processParents(const TypeInfo& type, TypeInfoEx& typeex)
	{
		if (!typeex.parentsProcessed)
		{
			for (auto& parent : typeex.parents)
				if (type != parent && globalMetadataTable.count(parent) > 0)
				{
					processParents(parent, globalMetadataTable[parent]);
					typeex += globalMetadataTable[parent];
				}
			typeex.parentsProcessed = true;
		}
	}

	inline void endMetadata()
	{
		for (auto& type : globalMetadataTable)
			processParents(type.first, type.second);

	}

	template <>
	class FromParents<>
	{
	public:
		template <typename O, typename... T>
		static inline void createMetadata(O* object)
		{
			std::vector<FieldInfo> metaTable;
			TypeInfo info = makeTypeInfo<O>();
			TypeInfoEx einfo = TypeInfoEx(info);
			globalMetadataTable.insert(std::make_pair(info, einfo));
		}

		template <typename O, typename... T>
		static inline void createMetadata(O* object, T... infos)
		{
			TypeInfo info = makeTypeInfo<O>();
			TypeInfoEx einfo = TypeInfoEx(info);
			//int i[] = { (einfo += globalMetadataTable[TYPEINFO(P)], 0)... };
			char c[] = { (einfo.members.push_back(createMetadataEntry(infos)), '\0')... };
			globalMetadataTable.insert(std::make_pair(info, einfo));
		}
	};

}

