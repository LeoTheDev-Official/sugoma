#pragma once
#include "sugoma_memory.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
namespace sugoma::core 
{
	template<typename BucketCount_t = uint16_t, typename BucketSize_t = uint16_t>
	class BlockAllocator 
	{
#pragma region Structs
	public:
		struct AllocationPos 
		{
			BucketCount_t bucketIndex = 0;
			BucketSize_t blockIndex = 0;
		};
	private:
		class Bucket 
		{
		public:
			enum BlockFlags : uint8_t 
			{
				Used = (1 << 0)
			};
		public:
			Bucket() = default;
			Bucket(BucketSize_t blocks, size_t blockSize) : m_blockCount(blocks), m_blockSize(blockSize + sizeof(BlockFlags)) {}
			void Create() 
			{
				m_bufferSize = m_blockCount * m_blockSize;
				m_emptyBlocks = m_blockCount;
				m_buffer = static_cast<uint8_t*>(sugoma_malloc(m_bufferSize));
				memset(m_buffer, 0, m_bufferSize);
			}
			void Destroy()
			{
				if (m_buffer)
				{
					sugoma_free(m_buffer);
					m_buffer = nullptr;
				}
			}
			bool Valid() const { return m_buffer != nullptr; }
			uint8_t* GetBlockStart(BucketSize_t index)
			{
				if (index >= m_blockCount) return nullptr;
				return m_buffer + (index * m_blockSize);
			}
			uint8_t* GetBlock(BucketSize_t index)
			{
				if (index >= m_blockCount) return nullptr;
				return m_buffer + (index * m_blockSize) + sizeof(BlockFlags);
			}
			uint8_t* AllocateBlock(BucketSize_t& index) 
			{
				if (m_emptyBlocks == 0) return nullptr;
				for (BucketSize_t i = 0; i < m_blockCount; ++i)
				{
					BucketSize_t blockIndex = (m_blockCursor + i) % m_blockCount;
					uint8_t* block = GetBlockStart(blockIndex);
					uint8_t& blockFlags = *(uint8_t*)block;
					if (blockFlags & BlockFlags::Used) continue;
					blockFlags |= BlockFlags::Used;
					--m_emptyBlocks;
					index = blockIndex;
					m_blockCursor = (blockIndex + 1) % m_blockCount;
					return block + sizeof(BlockFlags);
				}
				return nullptr;
			}
			void FreeBlock(BucketSize_t index)
			{
				if (index >= m_blockCount) return;
				uint8_t* block = GetBlockStart(index);
				memset(block, 0, sizeof(BlockFlags));
				if (index < m_blockCursor) m_blockCursor = index;
				m_blockCursor = index;
				++m_emptyBlocks;
			}
			BucketSize_t GetEmptyBlocks() const { return m_emptyBlocks; }
		private:
			BucketSize_t m_blockCount;
			size_t m_blockSize;
			size_t m_bufferSize;
			uint8_t* m_buffer = nullptr;
			BucketSize_t m_emptyBlocks;
			BucketSize_t m_blockCursor = 0;
		};
#pragma endregion
	public:
		BlockAllocator() = delete;
		BlockAllocator(BucketSize_t blocksPerBucket, size_t blockSize)
			: m_blocksPerBucket(blocksPerBucket), m_blockSize(blockSize)
		{
			NewBucket();
		}
		~BlockAllocator() 
		{
			for(auto& bucket : m_buckets)
				if (bucket.Valid())
					bucket.Destroy();
		}
		uint8_t* AllocateBlock(AllocationPos* pos = nullptr)
		{
			Bucket* bucket = nullptr;
			BucketCount_t bucketIndex = 0;
			Bucket* invalidBucket = nullptr;
			BucketCount_t invalidBucketIndex = 0;
			for (BucketCount_t i = 0; i < (BucketCount_t)m_buckets.size(); ++i)
			{
				BucketCount_t index = (m_bucketCursor + i) % m_buckets.size();
				Bucket* b = GetBucket(index);
				if (!b->Valid()) 
				{
					if (!invalidBucket) 
					{
						invalidBucket = b;
						invalidBucketIndex = index;
					}
					continue;
				}
				if (b->GetEmptyBlocks() == 0) continue;
				bucket = b;
				bucketIndex = index;
			}
			if (!bucket)
			{
				if (invalidBucket) 
				{
					invalidBucket->Create();
					bucket = invalidBucket;
					bucketIndex = invalidBucketIndex;
				}
				else
				{
					bucketIndex = (BucketCount_t)m_buckets.size();
					bucket = &NewBucket();
				}
			}
			m_bucketCursor = bucketIndex;
			BucketSize_t blockIndex = 0;
			uint8_t* block = bucket->AllocateBlock(blockIndex);
			if (pos) 
			{
				pos->blockIndex = blockIndex;
				pos->bucketIndex = bucketIndex;
			}
			return block;
		}
		uint8_t* GetBlock(AllocationPos position)
		{
			Bucket* bucket = GetBucket(position.bucketIndex);
			if (!bucket || !bucket->Valid()) return nullptr;
			return bucket->GetBlock(position.blockIndex);
		}
		bool FreeBlock(void* block) 
		{
			auto it = m_allocations.find(block);
			if (it == m_allocations.end()) return false;
			AllocationPos& pos = it->second;
			Bucket* bucket = GetBucket(pos.bucketIndex);
			if (!bucket || !bucket->Valid()) return false;
			bucket->FreeBlock(pos.blockIndex);
			if (pos.blockIndex != 0) {
				if (bucket->GetEmptyBlocks() == m_blocksPerBucket)
				{
					DeleteBucket(pos.bucketIndex);
					m_bucketCursor = pos.bucketIndex;
				}
				else
				{
					m_bucketCursor = pos.bucketIndex;
				}
			}
			m_allocations.erase(it);
			return true;
		}
		bool FreeBlock(AllocationPos position) 
		{
			Bucket* bucket = GetBucket(position.bucketIndex);
			if (!bucket || !bucket->Valid()) return false;
			void* b = bucket->GetBlock(position.blockIndex);
			bucket->FreeBlock(position.blockIndex);
			if (position.blockIndex != 0) {
				if (bucket->GetEmptyBlocks() == m_blocksPerBucket)
				{
					DeleteBucket(position.bucketIndex);
					m_bucketCursor = position.bucketIndex;
				}
				else
				{
					m_bucketCursor = position.bucketIndex;
				}
			}
			m_allocations.erase(b);
			return true;
		}

	public:
		BucketCount_t GetBucketCount() const { return (BucketCount_t)m_buckets.size(); }
		BucketSize_t BlocksPerBucket() const { return m_blocksPerBucket; }


	protected:
		Bucket& NewBucket()
		{
			Bucket& b = m_buckets.emplace_back(m_blocksPerBucket, m_blockSize);
			b.Create();
			return b;
		}
		Bucket* GetBucket(BucketCount_t index)
		{
			if (index >= m_buckets.size()) return nullptr;
			return &m_buckets[index];
		}
		void DeleteBucket(BucketCount_t index) 
		{
			if (index >= m_buckets.size()) return;
			m_buckets[index].Destroy();
		}
	private:
		BucketSize_t m_blocksPerBucket;
		size_t m_blockSize;
		std::vector<Bucket> m_buckets;
		BucketCount_t m_bucketCursor = 0;
		std::unordered_map<void*, AllocationPos> m_allocations;
	};
}