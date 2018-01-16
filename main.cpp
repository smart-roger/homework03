#include <iostream>
#include <map>

#include "allocator.h"
#include "container.h"
template <typename typeMap>
void test_map(typeMap& map ){
	auto val(1);
	for(auto key=0; key<10; ++key){
		val*=(key>0)?key:1;
		map[key] = val;
	}
		
	for(auto el : map)
		std::cout << el.first <<" "<<el.second<<std::endl;
}

int main(int argc, char const *argv[])
{
    try
    {
		std::map<int, int> stdMap;
		test_map(stdMap);
		
		std::map<int, int, std::less<int>, customAllocator<std::pair<const int, int>, 10>> allocMap;
		test_map(allocMap);
		
		{
			customContainer::oneWayList<int> testContainerWithStdAllocator;		
			for(auto key=0; key<10; ++key)
				testContainerWithStdAllocator.push_back(key);
		}
		/*for (auto iter = testContainerWithStdAllocator.cbegin(), iterEnd(testContainerWithStdAllocator.cend()); 
			iter!=iterEnd; 
			++iter)
			std:: cout << *iter << std::endl;*/
		{	
			customContainer::oneWayList<int, customAllocator<int, 10>> testContainerWithCustomAllocator;
			for(auto key=0; key<10; ++key)
				testContainerWithCustomAllocator.push_back(key);
		
			for (auto iter = testContainerWithCustomAllocator.cbegin(), iterEnd(testContainerWithCustomAllocator.cend()); 
					iter!=iterEnd; 
					++iter){
				std:: cout << *iter << std::endl;
			}
		}
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
	catch(...){
		std::cerr << "Unexpected problem!" << std::endl;
	}

    return 0;
}
