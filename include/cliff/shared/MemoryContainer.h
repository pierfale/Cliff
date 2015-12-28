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


		template<typename... Args>
		Type& emplace(Args... args) {
			check_memory();

			unsigned int offset = ((_blocks_allocated == 1 ? 1 : (_blocks_allocated-1)*Allocator<Type>::growing_memory_number_size)*Allocator<Type>::memory_block_number_base-_current_blocks_size_remain);

			new (_blocks[_blocks_allocated-1]._memory+offset) Type(args...);
			_current_blocks_size_remain--;
			return *(_blocks[_blocks_allocated-1]._memory+offset);
		}

		Type& test() {
			return *(_blocks[0]._memory);
		}

	private:
		struct Block {

			Block(unsigned int size) {
				_memory = static_cast<Type*>(std::malloc(size*sizeof(Type)));
			}

			~Block() {
				std::free(_memory);
			}

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
