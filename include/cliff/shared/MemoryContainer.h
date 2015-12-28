#ifndef _CLIFF_MEMORY_CONTAINER_H
#define _CLIFF_MEMORY_CONTAINER_H

#include <cstdlib>

namespace cliff {
	template<typename Type>
	class DefaultMemoryContainerAllocator {

	public:
		static const unsigned int memory_block_number_base = 64;
		static const unsigned int growing_memory_number_size = 2;
	};


	template<typename Type, template<typename> class Allocator = DefaultMemoryContainerAllocator>
	class MemoryContainer {

	public:
		MemoryContainer() : _blocks(static_cast<Block*>(std::malloc(sizeof(Block)))), _blocks_allocated(1), _current_blocks_size_remain(Allocator<Type>::memory_block_number_base) {
			new(_blocks) Block(_current_blocks_size_remain);
		}

		MemoryContainer(const MemoryContainer& that) = delete;

		~MemoryContainer() {
			//TODO destruct allocated object

			for(unsigned int block_index=0; block_index<_blocks_allocated; block_index++) {

				for(unsigned int object_index=0; object_index < (block_index == _blocks_allocated-1 ? _blocks[block_index]._size-_current_blocks_size_remain : _blocks[block_index]._size); object_index++) {
					(_blocks[block_index]._memory+object_index)->~Type();
				}

				(_blocks+block_index)->~Block();
			}

			std::free(_blocks);
		}


		template<typename... Args>
		Type& emplace(Args... args) {
			check_memory();

			unsigned int offset = ((_blocks_allocated == 1 ? 1 : (_blocks_allocated-1)*Allocator<Type>::growing_memory_number_size)*Allocator<Type>::memory_block_number_base-_current_blocks_size_remain);

			new (_blocks[_blocks_allocated-1]._memory+offset) Type(args...);
			_current_blocks_size_remain--;
			return *(_blocks[_blocks_allocated-1]._memory+offset);
		}

	private:
		struct Block {

			Block(unsigned int size) {
				_size = size;
				_memory = static_cast<Type*>(std::malloc(size*sizeof(Type)));
			}

			~Block() {
				std::free(_memory);
			}

			unsigned int _size;
			Type* _memory;
		};

		void check_memory() {
			if(_current_blocks_size_remain == 0) {
				_blocks_allocated++;
				_blocks = (Block*)std::realloc(_blocks, _blocks_allocated*sizeof(Block));
				_current_blocks_size_remain = (_blocks_allocated == 1 ? 1 : (_blocks_allocated-1)*Allocator<Type>::growing_memory_number_size)*Allocator<Type>::memory_block_number_base;
				new(_blocks+_blocks_allocated-1) Block(_current_blocks_size_remain);
			}
		}

		Block* _blocks;
		unsigned int _blocks_allocated;
		unsigned int _current_blocks_size_remain;

	};


}
#endif
