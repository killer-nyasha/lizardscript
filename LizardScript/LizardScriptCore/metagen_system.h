/*!
\file metagen_def.h
\brief core of metagen
\version 0.2
\date 23.00.2020
*/
#pragma once
#include "TypeInfo.h"
#include "VectorsTuple.h"

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
			if (sizeof(CA) <= sizeof(void*))
				return  *(CA*)(arg);
			else
				return  **(CA**)(arg);
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
				*(R*)(registers[16]) = ((*ths).*funcptr)(ArgImpl<A>::_impl((void*)&(registers[n + i--]))...);
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

    template <typename O>
    struct DtorProvider
    {
        void dtor()
        {
            delete ((O*)this);
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
		std::vector<TypeInfo> args;
	};

	struct ParentInfo
	{
		size_t offset;
		TypeInfo type;

		bool operator==(const ParentInfo& info) const
		{
			return type == info.type;
		}

		bool operator<(const ParentInfo& info) const
		{
			return type < info.type;
		}
	};

	//!extended type metadata
	//!contains TypeInfo of its members
	struct TypeInfoEx : public TypeInfo
	{
		TypeInfoEx() { }

		TypeInfoEx(const TypeInfo& from)
		{
			*(TypeInfo*)(this) = from;
		}

		void addParentMembers(TypeInfoEx& e, size_t offset)
		{
			members.get<FieldInfo>().insert(members.get<FieldInfo>().begin(),
				e.members.get<FieldInfo>().begin(),
				e.members.get<FieldInfo>().end());

			for (size_t i = 0; i < e.members.get<FieldInfo>().size(); i++)
				members.get<FieldInfo>()[i].offset += static_cast<int>(offset);

			members.get<FunctionInfo>().insert(members.get<FunctionInfo>().begin(),
				e.members.get<FunctionInfo>().begin(),
				e.members.get<FunctionInfo>().end());
		}

		VectorsTuple<FieldInfo, FunctionInfo> members;

		std::vector<ParentInfo> parents;
		bool parentsProcessed = false;
	};

	//all metadata of types
	extern std::map<TypeInfo, TypeInfoEx> globalMetadataTable;
}
