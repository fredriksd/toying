#include <functional>
#include <iostream>
#include <memory>
#include <utility>

template <typename T>
class function;

template <typename Ret, typename... Param>
class function<Ret(Param...)>
{
public:
   template <typename FunctionObject>
   explicit function(FunctionObject fo)
      : _callable(std::make_unique<callable_impl<FunctionObject>>(fo))
   {
   }

   Ret operator()(Param... param)
   {
      return _callable->call(param...);
   }

private:
   struct callable_interface
   {
      callable_interface() = default;
      ~callable_interface() = default;
      callable_interface(const callable_interface&) = delete;
      callable_interface(callable_interface&&) = default;
      callable_interface& operator=(const callable_interface&) = delete;
      callable_interface& operator=(callable_interface&&) = default;
      
      virtual Ret call(Param...) = 0;
      virtual std::unique_ptr<callable_interface> clone() = 0;
   };

   template <typename Callable>
   struct callable_impl : public callable_interface
   {
      callable_impl(Callable callable)
	 : _callable{std::move(callable)}
      {
      }
      ~callable_impl() = default;
      callable_impl(const callable_impl&) = delete;
      callable_impl(callable_impl&& other)
	 : _callable{std::exchange(other._callable, nullptr)}
      {
      }
      callable_impl& operator=(const callable_impl&) = delete;
      callable_impl& operator=(callable_impl&& other)
      {
	 if (this != &other)
	 {
	    _callable = std::exchange(other._callable, nullptr);
	 }
	 return *this;
      }
      

      Ret call(Param... param) override
      {
	 return std::invoke(_callable, param...);
      }

      std::unique_ptr<callable_interface> clone() override
      {
	 return std::make_unique<callable_impl>(std::move(*this));
      }
      Callable _callable;
   };

   std::unique_ptr<callable_interface> _callable;
};

template <typename Ret, typename... Param>
function(Ret (*)(Param...)) -> function<Ret(Param...)>; 

// template <typename Functor, typename... Param>
// function(Functor f) -> function<decltype(Functor(Param...))(Param...)>;

int foo(int x, int y)
{
   return x + y;
}

int main()
{
   function<int(int, int)> f{&foo};
   auto f1 = function{&foo};
   // function<void()> f2{[](){ std::cout << "Hello, world!\n"; }};

   f1(2, 3);
}
