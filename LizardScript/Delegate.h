#pragma once

template <typename R, typename... A>
using fptr = R(*)(A...);
template <typename R, typename O, typename... A>
using mfptr = R(O::*)(A...);

template <typename R, typename... A>
struct AbstractDelegate
{
	R operator()(A... args)
	{
		return this->call(args...);
	}

	virtual R call(A... args) {
		return R();
	}
};

#define DELEGATE_SIZE (sizeof(void*) * 3)

template <typename R, typename... A>
struct Delegate : public AbstractDelegate<R, A...>
{
	char data[DELEGATE_SIZE];

	template <typename T>
	Delegate(const T& other)
	{
		static_assert(sizeof(*this) >= sizeof(other), ":(");
		memcpy(this, &other, sizeof(other));
	}
};

template <typename R, typename... A>
struct fDelegate : public AbstractDelegate<R, A...>
{
	fptr<R, A...> f;

	fDelegate(decltype(f) f) : f(f)
	{
		static_assert(sizeof(*this) <= DELEGATE_SIZE, ":(");
	}

	virtual R call(A... args) override 
	{
		return f(args...);
	}
};

template <typename R, typename O, typename... A>
struct mfDelegate : public AbstractDelegate<R, A...>
{
	mfptr<R, O, A...> mf;
	O* object;

	mfDelegate(decltype(mf) mf, decltype(object) object) : mf(mf), object(object)
	{
		static_assert(sizeof(*this) <= DELEGATE_SIZE, ":(");
	}

	virtual R call(A... args) override
	{
		return ((*object).*mf)(args...);
	}
};

template <typename R, typename O, typename... A>
struct cmfDelegate : public AbstractDelegate<R, A...>
{
	R(O::*mf)(A...) const;
	const O* object;

	cmfDelegate(decltype(mf) mf, decltype(object) object) : mf(mf), object(object)
	{
		static_assert(sizeof(*this) <= DELEGATE_SIZE, ":(");
	}

	virtual R call(A... args) override
	{
		return ((*object).*mf)(args...);
	}
};

template <typename R, typename C, typename... A>
struct cDelegate : public AbstractDelegate<R, A...>
{
	C object;

	cDelegate(decltype(object) object) : object(object)
	{
		static_assert(sizeof(*this) <= DELEGATE_SIZE, ":(");
	}

	virtual R call(A... args) override
	{
		return object(args...);
	}
};

namespace DelegateConverter
{
	//http://www.cplusplus.com/forum/general/223816/

	template <typename T>
	struct deduce_type;

	template <typename RETURN_TYPE, typename CLASS_TYPE, typename... ARGS>
	struct deduce_type<RETURN_TYPE(CLASS_TYPE::*)(ARGS...)>
	{
		using type = cDelegate<RETURN_TYPE, ARGS...>;

		template <typename CLOSURE>
		static Delegate<RETURN_TYPE, ARGS...> get(const CLOSURE& fn)
		{
			return cDelegate<RETURN_TYPE, CLOSURE, ARGS...>(fn);
		}
	};

	template <typename RETURN_TYPE, typename CLASS_TYPE, typename... ARGS>
	struct deduce_type<RETURN_TYPE(CLASS_TYPE::*)(ARGS...) const>
	{
		using type = cDelegate<RETURN_TYPE, ARGS...>;

		template <typename CLOSURE>
		static Delegate<RETURN_TYPE, ARGS...> get(const CLOSURE& fn)
		{
			return cDelegate<RETURN_TYPE, CLOSURE, ARGS...>(fn);
		}
	};

	template <typename CLOSURE>
	inline auto makedel(const CLOSURE& fn)
	{
		return typename deduce_type<decltype(&CLOSURE::operator())>::get(fn);
	}

	template <typename R, typename... A>
	inline Delegate<R, A...> makedel(const Delegate<R, A...>& f)
	{
		return f;
	}

	template <typename R, typename... A>
	inline Delegate<R, A...> makedel(R(*f)(A...))
	{
		return fDelegate<R, A...>(f);
	}

	template <typename O, typename R, typename... A>
	inline Delegate<R, A...> makedel(std::pair<R(O::*)(A...), O*> pair)
	{
		mfDelegate<R, O, A...> del(pair.first, pair.second);
		return del;
	}

	template <typename O, typename R, typename... A>
	inline Delegate<R, A...> makedel(std::pair<R(O::*)(A...) const, const O*> pair)
	{
		cmfDelegate<R, O, A...> del(pair.first, pair.second);
		return del;
	}

	template <typename O, typename R, typename... A>
	inline Delegate<R, A...> makedel(R(O::*first)(A...), O* second)
	{
		mfDelegate<R, O, A...> del(first, second);
		return del;
	}

	template <typename O, typename R, typename... A>
	inline Delegate<R, A...> makedel(R(O::*first)(A...) const, const O* second)
	{
		cmfDelegate<R, O, A...> del(first, second);
		return del;
	}

}
