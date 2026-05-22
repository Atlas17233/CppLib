export module Atl:Allocator;

import :Def;
import :Limits;
import :Math;
import :Memory;

namespace Atl
{
  export
  {
    using Memory = Data<Void>;

    template <typename Type>
    class LargeAllocator final
    {
    public:
      [[nodiscard]] Type* allocate(Size size) noexcept
      {
        return alloc<Type>(size);
      }

      [[nodiscard]] Type* reallocate(Type* memory, Size size, Size newSize) noexcept
      {
        return realloc(memory, size, newSize);
      }

      Void deallocate(Type* memory) noexcept
      {
        dealloc(memory);
      }
    };

    class Chunk
    {
      public:
      struct alignas(0x10) Node
      {
        struct LR
        {
          UInt16 l;
          UInt16 r;
        };
        UInt16 index;
        UInt16 size;
        union
        {
          UInt16 cPriority;
          UInt8 pagePriority;
        };
        LR c;
        UInt8 pPriority;
        union
        {
          UInt32 pLR;
          LR p;
        };
        

        static constexpr UInt8 priority[0x100]
        {
          0x00, 0xc0, 0x80, 0x40, 0x20, 0xe0, 0xa0, 0x60, 0x10, 0xd0, 0x90, 0x50, 0x30, 0xf0, 0xb0, 0x70,
          0x08, 0xc8, 0x88, 0x48, 0x28, 0xe8, 0xa8, 0x68, 0x18, 0xd8, 0x98, 0x58, 0x38, 0xf8, 0xb8, 0x78,
          0x04, 0xc4, 0x84, 0x44, 0x24, 0xe4, 0xa4, 0x64, 0x14, 0xd4, 0x94, 0x54, 0x34, 0xf4, 0xb4, 0x74,
          0x0c, 0xcc, 0x8c, 0x4c, 0x2c, 0xec, 0xac, 0x6c, 0x1c, 0xdc, 0x9c, 0x5c, 0x3c, 0xfc, 0xbc, 0x7c,
          0x02, 0xc2, 0x82, 0x42, 0x22, 0xe2, 0xa2, 0x62, 0x12, 0xd2, 0x92, 0x52, 0x32, 0xf2, 0xb2, 0x72,
          0x0a, 0xca, 0x8a, 0x4a, 0x2a, 0xea, 0xaa, 0x6a, 0x1a, 0xda, 0x9a, 0x5a, 0x3a, 0xfa, 0xba, 0x7a,
          0x06, 0xc6, 0x86, 0x46, 0x26, 0xe6, 0xa6, 0x66, 0x16, 0xd6, 0x96, 0x56, 0x36, 0xf6, 0xb6, 0x76,
          0x0e, 0xce, 0x8e, 0x4e, 0x2e, 0xee, 0xae, 0x6e, 0x1e, 0xde, 0x9e, 0x5e, 0x3e, 0xfe, 0xbe, 0x7e,
          0x01, 0xc1, 0x81, 0x41, 0x21, 0xe1, 0xa1, 0x61, 0x11, 0xd1, 0x91, 0x51, 0x31, 0xf1, 0xb1, 0x71,
          0x09, 0xc9, 0x89, 0x49, 0x29, 0xe9, 0xa9, 0x69, 0x19, 0xd9, 0x99, 0x59, 0x39, 0xf9, 0xb9, 0x79,
          0x05, 0xc5, 0x85, 0x45, 0x25, 0xe5, 0xa5, 0x65, 0x15, 0xd5, 0x95, 0x55, 0x35, 0xf5, 0xb5, 0x75,
          0x0d, 0xcd, 0x8d, 0x4d, 0x2d, 0xed, 0xad, 0x6d, 0x1d, 0xdd, 0x9d, 0x5d, 0x3d, 0xfd, 0xbd, 0x7d,
          0x03, 0xc3, 0x83, 0x43, 0x23, 0xe3, 0xa3, 0x63, 0x13, 0xd3, 0x93, 0x53, 0x33, 0xf3, 0xb3, 0x73,
          0x0b, 0xcb, 0x8b, 0x4b, 0x2b, 0xeb, 0xab, 0x6b, 0x1b, 0xdb, 0x9b, 0x5b, 0x3b, 0xfb, 0xbb, 0x7b,
          0x07, 0xc7, 0x87, 0x47, 0x27, 0xe7, 0xa7, 0x67, 0x17, 0xd7, 0x97, 0x57, 0x37, 0xf7, 0xb7, 0x77,
          0x0f, 0xcf, 0x8f, 0x4f, 0x2f, 0xef, 0xaf, 0x6f, 0x1f, 0xdf, 0x9f, 0x5f, 0x3f, 0xff, 0xbf, 0x7f
        };

        Void init(UInt16 i, UInt16 s) noexcept
        {
          index = i;
          size = s;
          if (i + s < Max<UInt16>) [[likely]] {
            this[s].index = i;
          }
          cPriority = i;
          pagePriority = priority[i & 0xff];
          c = {};
          pPriority = _tzcnt_u32(min(s, 4095ui16));
          p = {};
        }
      };

      using SplitLR = Pair<Node*>;
      struct SplitLMR
      {
        Node* l{0};
        Node* m{0};
        Node* r{0};
      };

      SplitLR splitP2(Node* treap, UInt16 size)
      {
        Node* l = nodes_;
        Node* r = nodes_;
        nodes_->pLR = 0;
        while (treap != nodes_) {
          if (treap->size < size) {
            l->p.r = treap->index;
            l = treap;
            treap = nodes_ + treap->p.r;
            l->p.r = 0;
          } else {
            r->p.l = treap->index;
            r = treap;
            treap = nodes_ + treap->p.l;
            r->p.l = 0;
          }
        }
        return {nodes_ + nodes_->p.r, nodes_ + nodes_->p.l};
      }

      SplitLMR splitP3(Node* treap, UInt16 size)
      {
        Node* l = nodes_;
        Node* m = nodes_;
        Node* r = nodes_;
        nodes_->pLR = 0;
        while (treap != nodes_) {
          if (treap->size == size) {
            m = treap;
            l->p.r = treap->p.l;
            r->p.l = treap->p.r;
            break;
          } else if (treap->size < size) {
            l->p.r = treap->index;
            l = treap;
            treap = nodes_ + treap->p.r;
            l->p.r = 0;
          } else {
            r->p.l = treap->index;
            r = treap;
            treap = nodes_ + treap->p.l;
            r->p.l = 0;
          }
        }
        return {nodes_ + nodes_->p.r, m, nodes_ + nodes_->p.l};
      }

      UInt16 mergeP2(Node* l, Node* r)
      {
        UInt16 root = 0;
        UInt16* pos = &root;
        while (l != nodes_ && r != nodes_) {
          if (l->pPriority > r->pPriority) {
            *pos = l->index;
            pos = &l->p.r;
            l = nodes_ + l->p.r;
          } else {
            *pos = r->index;
            pos = &r->p.l;
            r = nodes_ + r->p.l;
          }
        }
        *pos = l->index ? l->index : r->index;
        return root;
      }

      UInt16 mergeP3(Node* l, Node* m, Node* r)
      {
        UInt16 root = 0;
        UInt16* pos = &root;
        while (l != nodes_ && r != nodes_) {
          if (l->pPriority > r->pPriority) {
            if (l->pPriority > m->pPriority) {
              *pos = l->index;
              pos = &l->p.r;
              l = nodes_ + l->p.r;
              continue;
            }
          } else if(m->pPriority <= r->pPriority) {
            *pos = r->index;
            pos = &r->p.l;
            r = nodes_ + r->p.l;
            continue;
          }
          *pos = m->index;
          m->p.l = l->index;
          m->p.r = r->index;
          return root;
        }
        if (l == nodes_) l = m;
        else r = m;
        while (l != nodes_ && r != nodes_) {
          if (l->pPriority > r->pPriority) {
            *pos = l->index;
            pos = &l->p.r;
            l = nodes_ + l->p.r;
          } else {
            *pos = r->index;
            pos = &r->p.l;
            r = nodes_ + r->p.l;
          }
        }
        *pos = l->index ? l->index : r->index;
        return root;
      }

      Void insert(UInt16 index, UInt16 size)
      {
        nodes_[index].init(index, size);
        if (!nodes_->size) [[unlikely]] {
          nodes_->size = size;
          return;
        }
        SplitLMR LMR = splitP3(nodes_ + nodes_->size, size);
        if (LMR.l == nodes_) nodes_->size = mergeP2(nodes_ + index, LMR.r);
        else if (LMR.r == nodes_) nodes_->size = mergeP2(LMR.l, nodes_ + index);
        else nodes_->size = mergeP3(LMR.l, nodes_ + index, LMR.r);
      }

      Void insert2(UInt16 index, UInt16 size)
      {
        nodes_[index].init(index, size);
        if (!nodes_->size) [[unlikely]] {
          nodes_->size = size;
          return;
        }
        SplitLR LR = splitP2(nodes_ + nodes_->size, size);
        UInt16 MR{index && LR.r != nodes_ ? mergeP2(nodes_ + index, LR.r) : (UInt16)(index + LR.r->index)};
        nodes_->size = LR.l != nodes_ && MR ? mergeP2(LR.l, nodes_ + MR) : LR.l->index + MR;
      }

      Void remove(UInt16 size)
      {
        SplitLMR LMR = splitP3(nodes_ + nodes_->size, size);
        nodes_->size = LMR.l != nodes_ && LMR.r != nodes_ ? mergeP2(LMR.l, LMR.r) : LMR.l->index + LMR.r->index;
        //return {nodes_ + M_R.l, /*size*/};
      }

      Void printTreeHorizontal(Node& r, int depth = 0, const std::string& prefix = "") {
        if (&r == nodes_) return;
        printTreeHorizontal(nodes_[r.p.r], depth + 1, prefix);
        std::cout << std::string(depth * 4, ' ') << r.size << std::endl;
        printTreeHorizontal(nodes_[r.p.l], depth + 1, prefix);
      }

      Pair<UInt16&, Node&> searchSize(UInt16 size) noexcept
      {
        UInt16* targetParent{nullptr};
        Node* target{nullptr};
        UInt16* parent{nullptr};
        Node* node{nodes_ + nodes_->index};
        while (node != nodes_)
        {
          if (size <= node->size) {
            targetParent = parent;
            target = node;
            parent = &node->p.l;
            node = nodes_ + node->p.l;
          } else {
            parent = &node->p.r;
            node = nodes_ + node->p.r;
          }
        }
        return {*targetParent, *target};
      }

    public:
      Chunk() noexcept: nodes_{(Node*)alloc<Node>(1_MiB)}
      {
        nodes_[0].init(1, 0xffff);
        nodes_[1].init(1, 0xffff);
      }

      [[nodiscard]] Void* allocate(UInt16 size) noexcept {
        Pair<UInt16&, Node&> root{searchSize(size)};
        if (root.r.c.l != root.r.c.r) {
          //remove root.r from indexTreap
          //root.l = newIndexTreapRoot.index
          //newindexTreap.pL = root.r.pL
          //newindexTreap.pR = root.r.pR
          //init new small mem
          //insert new small mem
          //return
        } else {
          if (root.r.size == nodes_->size) {
            //change nodes_->size to new max size
          }
          //remove root.r from sizeTreap
          //init new small mem
          //insert new small mem
          //return
        }





        return nullptr;
      }

      Void deallocate(Void* memory) noexcept {}

    private:
      Node* nodes_;
    };
/*
    template <Size sizeBlock> requires(sizeBlock < 8)
    class FixedChunk final
    {
    public:
      Bool reserve(UInt8 nBlock) noexcept
      {
        begin_ = malloc(nBlock * sizeBlock);
        end_ = block(nBlock--);
        Byte* p{begin_};
        for (UInt8 i{0}; i < nBlock; p += sizeBlock) {
          *p = ++i;
        }
        *p = Max<Byte>;
      }

      //Void release() noexcept { free(begin_); }

      Void* allocate() noexcept
      {
        Byte* p{block(top_)};
        top_ = *p;
        ++usedBlocks_;
        return p;
      }

      Void deallocate(Void* p, Size blockSize) noexcept
      {
        *(Byte*)p = top_;
        top_ = (Byte)(((Byte*)p - begin_) / blockSize);
        --usedBlocks_;
      }

      Bool hasBlock(Void* p, Size chunkLength) const noexcept { return (begin_ <= (Byte*)p) && ((Byte*)p < end_); }

      Bool isFull() const noexcept { return !usedBlocks_; }
      Bool isEmpty() const noexcept { return top_ < 0; }
      //Bool notEmpty() const noexcept { return top_ >= 0; }

    private:
      [[msvc::forceinline]] [[nodiscard]] Byte* block(UInt8 i) noexcept { return begin_ + i * sizeBlock; }

    private:
      Byte* begin_{nullptr};
      Byte* end_{nullptr};
      Int8 top_{0};
      Int8 usedBlocks_{0};
    };

    class FixedAllocator
    {
    private:

        void DoDeallocate( void * p );

        bool MakeNewChunk( void );

        Chunk * VicinityFind( void * p ) const;

        FixedAllocator(const FixedAllocator&);
        FixedAllocator& operator=(const FixedAllocator&);

        typedef std::vector< Chunk > Chunks;
        typedef Chunks::iterator ChunkIter;
        typedef Chunks::const_iterator ChunkCIter;

        static unsigned char MinObjectsPerChunk_;

        static unsigned char MaxObjectsPerChunk_;

        std::size_t blockSize_;
        unsigned char numBlocks_;

        Chunks chunks_;
        Chunk * allocChunk_;
        Chunk * deallocChunk_;
        Chunk * emptyChunk_;

    public:
        FixedAllocator();

        ~FixedAllocator();

        void Initialize( std::size_t blockSize, std::size_t pageSize );

        void * Allocate( void );

        bool Deallocate( void * p, Chunk * hint );

        inline std::size_t BlockSize() const { return blockSize_; }

        bool TrimEmptyChunk( void );

        bool TrimChunkList( void );

        std::size_t CountEmptyChunks( void ) const;

        bool IsCorrupt( void ) const;

        const Chunk * HasBlock( void * p ) const;
        inline Chunk * HasBlock( void * p )
        {
            return const_cast< Chunk * >(
                const_cast< const FixedAllocator * >( this )->HasBlock( p ) );
        }

    };

    template <typename Type, typename Alloc>
    class PoolAllocator
    {
    public:
        constexpr Type* allocate(Size n) {}

        constexpr std::allocation_result<Type*, Size> allocateAtLeast(Size n) {}

        Void deallocate(Type* ptr, Size n) {}
    };*/
  }
}
