/*!Реализовать свой контейнер, который по аналогии с контейнерами stl
параметризуется аллокатором. Контейнер должен иметь две возможности
- добавить новый элемент и обойти контейнер в одном направлении.
Совместимость с контейнерами stl на усмотрение автора.*/

#include <memory>

namespace customContainer{
	//	Тип для узла списка
	template <typename T>
	struct list_node{
		list_node*	_next;
		T	_val;
		
		list_node(){_next=nullptr; _val = T();};
	};

	//	Константный итератор для обхода списка
	template<typename T>
		struct onewaylist_const_iterator
		{//	iterator_traits
		  using _Self = onewaylist_const_iterator<T>;
		
		  //using difference_type = ptrdiff_t;
		  using iterator_category = std::forward_iterator_tag;
		  using value_type = T;
		  using pointer = const T*;
		  using reference = const T&;
			//	Значение соответствующего узла в списке
		  list_node<T>*	_data;
		  //	Конструктор по умолчанию
		  onewaylist_const_iterator():_data(nullptr){ 
			
		  };
		  //	Конструктор с параметром. Для создания внутри контейнера
		  explicit onewaylist_const_iterator(list_node<T>* ptrNode):_data(ptrNode){};
		  
		  public:
		  //	Получение ссылки на хранимый элемент
		  reference operator*() const   { 
			return _data->_val; 
		}
		  //	Получение указателя на хранимый элемент
		  pointer operator->() const  { return &(_data->_val); }

		  //Переход к следующему элементу контейнера
		  _Self& operator++()  {
			if(nullptr==_data)	return *this;
			
			else _data = _data->_next;
				
			return *this;
		  }
	
			//Сравнение итераторов для прохода по циклу
		bool operator==(const _Self& rhs) const {			
			return _data == rhs._data;
		}
			
		  bool operator!=(const _Self& rhs) const 
		  { return _data!=rhs._data;
		  }
		};

	//	Сам контейнер. Односвязный список.
	template<typename T, //	хранимый тип
		class Allocator = std::allocator<T>> // Распределитель памяти
	class oneWayList{
		//	Типы для использования внутри контейнера
		using pointer = T*;
		
		//	Элемент списка		
		using ptrNode = list_node<T>*;
		
		//	Аллокатор для узлов списка
		typedef  typename Allocator::template rebind<list_node <T> >::other node_allocator;
		
		//Указатель на первый элемент списка
		ptrNode	_first;
		//	Указатель на последний элемент списка
		ptrNode	_last;
		
		//	Узел для реализации окончания списка
		list_node<T> node_end;
		
		//	Аллокатор для размещения элементов
		node_allocator 	_allocator;
		
		//	Выделяем память и размещаем элемент в контейнере
		ptrNode	_createNode(const T& new_val){
			//	Выделение памяти для узла списка
			auto ptr = _allocator.allocate(1);
			//std::cout << "allocate in: " << ptr << std::endl;
			//	Конструирование узла списка
			_allocator.construct(ptr);
			
			//	Задаём значения элементу списка
			ptr->_val = new_val;
			ptr->_next = nullptr;
			//std::cout << "next: " << ptr->_next << std::endl;
			return ptr;
		};
		
		//	уничтожаем элемент списка
		//void _destructElement(ptrElement rem){}
		
		public:
		oneWayList():_first(nullptr), _last(nullptr), _allocator(){
			//std::cout << "sizeof list_node: " << sizeof(list_node<T>) << std::endl;
			//std::cout << "sizeof T: " << sizeof(T) << std::endl;
		};
		
		//	Почистить всю выделенную память
		~oneWayList(){			
			auto curr = _first; 
			while(curr!=nullptr){
				//std::cout << "curr: " << curr << " next: " << curr->_next << std::endl;
				auto ptrRemove(curr);
				curr=curr->_next;
				//	По очереди удаляем все элементы списка
				
				//std::cout << "deallocate: " << ptrRemove << std::endl;
				//std::cout << "next: " << ptrRemove->_next << std::endl;
				_allocator.destroy(ptrRemove);
				_allocator.deallocate(ptrRemove,1);
			}
		};
		
		//	Добавляем элемент в конец списка		
		void push_back( const T& value ){
			//	Выделяем память под элемент
			auto newElement = _createNode(value);
			
			//	Если элементов ещё нет - добавляем первый
			if (nullptr == _last){
				_first = newElement;
				_last = newElement;				
			} else {
			//	Если элементы уже есть - редактируем last	
				_last->_next = newElement;
				//std::cout << "prev: " << _last << " next: " << _last->_next << " _last: " << newElement << std::endl;
				_last = newElement;
				_last->_next=nullptr;
			}
		};
		
		onewaylist_const_iterator<T> cbegin() const noexcept{
			return onewaylist_const_iterator<T>(_first);
		}
		onewaylist_const_iterator<T> cend() noexcept{
			return onewaylist_const_iterator<T> ();
		}
	};
}
