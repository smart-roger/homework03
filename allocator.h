#include <memory>
//	Свой аллокатор
//	BLOCK_SIZE - размер блока для выделения
template <typename T, size_t BLOCK_SIZE>
class customAllocator{
	private:
		//static const size_t	_allocSize = BLOCK_SIZE;
		
		void*	_allocated;
		size_t _counter;
	public:
	//	Allocator traits
		using 	size_type 		= 	size_t;
		using difference_type	=	int;
		using pointer			=	T*;
		using const_pointer		=	const T* ;
		using reference			=	T&;
		using const_reference	=	const T&;
		using value_type		=	T;
		
		customAllocator():_allocated(nullptr), _counter(0){
			//std::cout << "map allocator constructor" << std::endl;			
			_allocated = _allocated = std::malloc (BLOCK_SIZE *sizeof(T));
			
			//	Если выделить не удалось
				if (nullptr == _allocated)
					//	Тогда у нас проблемы
					throw std::bad_alloc();
			
			//std::cout << "allocated: "<<_allocated << std::endl;
		};
		
		~customAllocator(){
			//std::cout << "map allocator destructor" << std::endl;
			free(_allocated);
		}
		
		T* allocate(std::size_t n){
			//std::cout << "allocate " << n << "*"<<sizeof(T) <<std::endl; 
			T* res(nullptr);
			
			//	Вычислим адрес 
			res = reinterpret_cast <T *> (_allocated);
			res+=_counter;
			++_counter;
			
			//std::cout << "return value: " << res << std::endl;
			return res;
		};
		
		template<class U, class... Args>
		void construct( U* p, Args&&... args){
			//std::cout << "construct "<< p << std::endl;
			new(static_cast<void *>(p)) U(std::forward<Args>(args)...);
		}
		
		template<class U>
		void destroy(U* p){
			//std::cout << "destruct "<< p << std::endl;
			p->~U();
		}
		
		template<class U> struct rebind { typedef customAllocator<U, BLOCK_SIZE> other; };
		
		void deallocate(T* p, std::size_t n){
			//std::cout << "deallocate " << n <<std::endl;
			--_counter;
			//std::cout << "counter: " << _counter << std::endl;
		};
};