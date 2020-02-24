/*!
\file Delegate.h
\brief Simple alternative to std::function
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once

//todo:
//destructor
//store callable object or refer to it???
template <typename R, typename... A>
class Fptr
{
    //!max size of callable object which can be stored in Fptr instanses
#define FPTR_SIZE 24

    //!should we free it or not
#define FREE_FPTR_OBJECT

private:

    //!pointer to function which calls callable object from data
    R(Fptr::*call)(A...);

#ifdef FREE_FPTR_OBJECT
    void(Fptr::*free)();
#endif

    //!callable object is stored in this buffer
    char data[FPTR_SIZE];

    //!interpret buffer data as any type instanse
    template <typename T>
    T& obj() { return *reinterpret_cast<T*>(this->data); }

    template <typename T>
    R _call(A... args)
    {
        return obj<T>()(args...);
    }

#ifdef FREE_FPTR_OBJECT
    template <typename T>
    void _free()
    {

    }
#endif

public:

    //!\bug will it work with pointer to virtual function of multiply-inherited class? (its sizeof and real size are different)
    template <typename T>
    static Fptr init(const T& from)
    {
        static_assert(sizeof(from) <= FPTR_SIZE, "your class is too big =)");
        Fptr f;
        f.call = &_call<T>;

#ifdef FREE_FPTR_OBJECT
        f.free = &_free<T>;
#endif

        //memcpy(&f.data, &from, sizeof(from));
        f.obj<T>() = from;

        return f;
    }

    R operator()(A... args)
    {
        return (this->*call)(args...);
    }

    //!\bug i don't know should i 
    ~Fptr()
    {
#ifdef FREE_FPTR_OBJECT
        (this->*free)();
#endif
    }
};

//!contains pointer to object and pointer to his member function
//!and can be called as a function
//!\bug how can we know that an object wasn't deleted? using weak_ref is too expensive. use LizardScript garbage collector?
template <typename R, typename O, typename... A>
struct Delegate
{
    O* ths;
    R(O::* func)(A...);

    R operator()(A... args)
    {
        return (ths->*func)(args...);
    }
};

//!contains pointer to constant object and pointer to his constant member function
//!it's similar to Delegate
template <typename R, typename O, typename... A>
struct ConstDelegate
{
    const O* ths;
    R(O::* func)(A...) const;

    R operator()(A... args)
    {
        return (ths->*func)(args...);
    }
};

//!create Fptr from C-style function pointer
template <typename R, typename... A>
Fptr<R, A...> nmakedel(R(*func)(A...))
{
    Fptr<R, A...> fptr = Fptr<R, A...>::init(func);
    return fptr;
}

//!create Fptr from pointer to object and member function pointer (using Delegate)
template <typename R, typename O, typename... A>
Fptr<R, A...> nmakedel(O* object, R(O::* func)(A...))
{
    Delegate<R, O, A...> del = { object, func };
    Fptr<R, A...> fptr = Fptr<R, A...>::init(del);
    return fptr;
}

//!create Fptr from pointer to constant object and constant member function pointer (using ConstDelegate)
template <typename R, typename O, typename... A>
Fptr<R, A...> nmakedel(const O* object, R(O::* func)(A...) const)
{
    ConstDelegate<R, O, A...> del = { object, func };
    Fptr<R, A...> fptr = Fptr<R, A...>::init(del);
    return fptr;
}
