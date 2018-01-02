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
		T*	_val;
		
		list_node(){_next=nullptr; _val = nullptr;};
	};

	//	Константный итератор для обхода списка
	template<typename T>
		struct onewaylist_const_iterator
		{//	iterator_traits
		  using _Self = onewaylist_const_iterator<T>;
		
		  using difference_type = ptrdiff_t;
		  using iterator_category = std::forward_iterator_tag;
		  using value_type = T;
		  using pointer = const T*;
		  using reference = const T&;
			//	Значение соответствующего узла в списке
		  list_node<T>*	_data;
		  //	Конструктор по умолчанию
		  onewaylist_const_iterator():_data(nullptr){ 
			std::cout << "default ctor iterator" <<std::endl;
		  };
		  //	Конструктор с параметром. Для создания внутри контейнера
		  explicit onewaylist_const_iterator(list_node<T>* ptrNode):_data(ptrNode){};
		  
		  public:
		  //	Получение ссылки на хранимый элемент
		  reference operator*() const   { 
			return *(_data->_val); 
		}
		  //	Получение указателя на хранимый элемент
		  pointer operator->() const  { return _data->_val; }

		  //Переход к следующему элементу контейнера
		  _Self& operator++()  {
			if(nullptr==_data)	return *this;
			
			if(nullptr==_data->_next){
				_data->_val = nullptr;
				_data->_next= nullptr;
			}
			else _data = _data->_next;
				
			return *this;
		  }
	
			//Сравнение итераторов для прохода по циклу
		bool operator==(const _Self& rhs) const {			
			return ((_data->_next == rhs._data->_next) && (_data->_val == rhs._data->_val));
		}
			
		  bool operator!=(const _Self& rhs) const 
		  { return (!(*this==rhs));
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
		
		//Указатель на первый элемент списка
		ptrNode	_first;
		//	Указатель на последний элемент списка
		ptrNode	_last;
		
		//	Узел для реализации окончания списка
		list_node<T> node_end;
		
		//	Аллокатор для размещения элементов
		Allocator 	_allocator;
		
		//	Выделяем память и размещаем элемент в контейнере
		ptrNode	_createNode(const T& new_val){
			//	Выделение памяти
			auto ptr = _allocator.allocate(1);
			//	Конструирование элемента
			_allocator.construct(ptr, new_val);
			
			//	Задаём значения элементу списка
			auto res = new list_node<T>;
			res->_val = ptr;
			res->_next = nullptr;
		};
		
		//	уничтожаем элемент списка
		//void _destructElement(ptrElement rem){}
		
		public:
		oneWayList():_first(nullptr), _last(nullptr), _allocator(){
			
		};
		
		//	Почистить всю выделенную память
		~oneWayList(){			
			for (auto curr = _first; curr->_next!=nullptr; curr=curr->_next){
				//	По очереди удаляем все элементы списка
				_allocator.destroy(curr->_val);
				_allocator.deallocate(curr->_val,1);				
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
				_last = newElement;
			}
		};
		
		onewaylist_const_iterator<T> cbegin() const noexcept{
			return onewaylist_const_iterator<T>(_first);
		}
		onewaylist_const_iterator<T> cend() noexcept{
			return onewaylist_const_iterator<T> (&node_end);
		}
	};
}
