export module Atl:Allocator;

import :Def;
import :Limits;
import :Math;
import :XMemory;

import "Macros";

namespace Atl
{
  class CompactBaseAllocator
  {
    class LargeAllocator
    {
    public:
      [[nodiscard]] static Void* allocate(Size size) noexcept
      {
        return alloc(alignUpGranularity(size));
      }

      [[nodiscard]] static Void* expand(Void* memory, Void* capacity, Void* newCapacity) noexcept
      {
        return alignUpGranularity(capacity);
      }

      [[nodiscard]] static Void* shrink(Void* memory, Void* capacity, Void* newCapacity) noexcept
      {
        capacity = alignUpPage(capacity);
        newCapacity = alignUpPage(newCapacity);
        if (capacity > newCapacity) {
          recommit(newCapacity, (UInt64)capacity - (UInt64)newCapacity);
        }
        return capacity;
      }

      static Void deallocate(Void* memory, Void* capacity) noexcept
      {
        release(memory);
      }
    };

    class SmallAllocator
    {
      class Chunk
      {
      public:
        struct alignas(16) Node
        {
          UInt16 pL;
          UInt16 pR;
          UInt16 cL;
          UInt16 cR;
          UInt16 index;
          UInt16 size;
          UInt8 pKey;
          UInt8 pPriority;
          union {
            UInt16 cPriority;
            UInt8 pagePriority;
          };

          Void makeFirst(UInt16 i) noexcept
          {
            index = i;
            size = 0x10000 - i;
            pKey = 0xff;
            pagePriority = priority[i];
          }

          Void init(UInt16 i, UInt16 s) noexcept
          {
            pL = 0;
            pR = 0;
            cL = 0;
            cR = 0;
            index = i;
            size = s;
            pKey = s <= 0xff ? s : 0xff;
            pPriority = _tzcnt_u32(pKey);
            cPriority = i;
            pagePriority = priority[i & 0xff];
            if (s > 1 && i + s < 0x10000) {
              this[s - 1].index = i;
              this[s - 1].size = s;
              this[s - 1].pKey = pKey;
            }
          }

          Void resize(UInt16 s) noexcept
          {
            size = s;
            pKey = s <= 0xff ? s : 0xff;
            pPriority = _tzcnt_u32(pKey);
            if (s > 1 && index + s < 0x10000) {
              this[s - 1].index = index;
              this[s - 1].size = s;
              this[s - 1].pKey = pKey;
            }
          }

        private:
          static constexpr UInt8 priority[0x100]{
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
        };

      private:
        struct SplitLR
        {
          Node* l;
          Node* r;
        };

        struct SplitLMR
        {
          Node* l;
          Node* m;
          Node* r;
        };

        SplitLMR splitP3(UInt8 pKey) noexcept
        {
          Node* node{nodes_ + nodes_->cPriority};
          Node* l{nodes_};
          Node* m{nodes_};
          Node* r{nodes_};
          *(UInt64*)nodes_ = 0;
          while (node != nodes_) {
            if (node->pKey == pKey) {
              l->pR = node->pL;
              r->pL = node->pR;
              node->pL = 0;
              node->pR = 0;
              m = node;
              break;
            } else if (node->pKey < pKey) {
              l->pR = node->index;
              l = node;
              node = nodes_ + node->pR;
              l->pR = 0;
            } else {
              r->pL = node->index;
              r = node;
              node = nodes_ + node->pL;
              r->pL = 0;
            }
          }
          return {nodes_ + nodes_->pR, m, nodes_ + nodes_->pL};
        }

        SplitLR splitC2(Node* node, UInt16 index) noexcept
        {
          Node* l{nodes_};
          Node* r{nodes_};
          *(UInt64*)nodes_ = 0;
          while (node != nodes_) {
            if (node->index < index) {
              l->cR = node->index;
              l = node;
              node = nodes_ + node->cR;
              l->cR = 0;
            } else {
              r->cL = node->index;
              r = node;
              node = nodes_ + node->cL;
              r->cL = 0;
            }
          }
          return {nodes_ + nodes_->cR, nodes_ + nodes_->cL};
        }

        SplitLMR splitC3(Node* node, UInt16 index) noexcept
        {
          Node* l{nodes_};
          Node* m{nodes_};
          Node* r{nodes_};
          *(UInt64*)nodes_ = 0;
          while (node != nodes_) {
            if (node->index == index) {
              m = node;
              l->cR = node->cL;
              r->cL = node->cR;
              m->cL = 0;
              m->cR = 0;
              break;
            } else if (node->index < index) {
              l->cR = node->index;
              l = node;
              node = nodes_ + node->cR;
              l->cR = 0;
            } else {
              r->cL = node->index;
              r = node;
              node = nodes_ + node->cL;
              r->cL = 0;
            }
          }
          return {nodes_ + nodes_->cR, m, nodes_ + nodes_->cL};
        }

        Void mergeP3(Node* l, Node* m, Node* r) noexcept
        {
          UInt16* node{&nodes_->cPriority};
          if (m != nodes_) {
            if (l != nodes_) {
              if (r != nodes_) {
                do {
                  if (l->pPriority > r->pPriority) {
                    if (l->pPriority > m->pPriority) {
                      *node = l->index;
                      node = &l->pR;
                      l = nodes_ + l->pR;
                      continue;
                    }
                  } else if(m->pPriority < r->pPriority) {
                    *node = r->index;
                    node = &r->pL;
                    r = nodes_ + r->pL;
                    continue;
                  }
                  *node = m->index;
                  m->pL = l->index;
                  m->pR = r->index;
                  return;
                } while (l != nodes_ && m != nodes_ && r != nodes_);
                if (l == nodes_) {
                  l = m;
                } else {
                  r = m;
                }
              } else {
                r = m;
              }
            } else {
              l = m;
            }
          }
          while (l != nodes_ && r != nodes_) {
            if (l->pPriority > r->pPriority) {
              *node = l->index;
              node = &l->pR;
              l = nodes_ + l->pR;
            } else {
              *node = r->index;
              node = &r->pL;
              r = nodes_ + r->pL;
            }
          }
          *node = l->index + r->index;
        }

        Node* mergeC(Node* l, Node* r, UInt16* node) noexcept
        {
          while (l != nodes_ && r != nodes_) {
            if (l->cPriority < r->cPriority) {
              *node = l->index;
              node = &l->cR;
              l = nodes_ + l->cR;
            } else {
              *node = r->index;
              node = &r->cL;
              r = nodes_ + r->cL;
            }
          }
          *node = l->index + r->index;
          return nodes_ + nodes_->pL;
        }

        Node* mergeC2(Node* l, Node* r) noexcept
        {
          return mergeC(l, r, (UInt16*)nodes_);
        }

        Node* mergeC3(Node* l, Node* m, Node* r) noexcept
        {
          UInt16* node{(UInt16*)nodes_};
          if (m != nodes_) {
            if (l != nodes_) {
              if (r != nodes_) {
                do {
                  if (l->cPriority < r->cPriority) {
                    if (l->cPriority < m->cPriority) {
                      *node = l->index;
                      node = &l->cR;
                      l = nodes_ + l->cR;
                      continue;
                    }
                  } else if(m->cPriority > r->cPriority) {
                    *node = r->index;
                    node = &r->cL;
                    r = nodes_ + r->cL;
                    continue;
                  }
                  *node = m->index;
                  m->cL = l->index;
                  m->cR = r->index;
                  return nodes_ + nodes_->pL;
                } while (l != nodes_ && m != nodes_ && r != nodes_);
                if (l == nodes_) {
                  l = m;
                } else {
                  r = m;
                }
              } else {
                r = m;
              }
            } else {
              l = m;
            }
          }
          return mergeC(l, r, node);
        }

        Bool isFree(UInt16 index, UInt16 size) noexcept
        {
          Node* node{nodes_ + nodes_->cPriority};
          while (node != nodes_) {
            if (node->pKey == size) {
              while (node != nodes_) {
                if (node->index == index) {
                  return true;
                } else if (node->index > index) {
                  node = nodes_ + node->cL;
                } else {
                  node = nodes_ + node->cR;
                }
              }
              return false;
            } else if (node->pKey > size) {
              node = nodes_ + node->pL;
            } else {
              node = nodes_ + node->pR;
            }
          }
          return false;
        }

        static Node* alignUpNode(Node* node) noexcept
        {
          return (Node*)((UInt64)node + 0xf & 0xfffffffffffffff0);
        }

      public:
        Void reset() noexcept { nodes_ = nullptr; }

        operator Bool() noexcept { return nodes_; }

        Bool hasSize(UInt8 size) noexcept { return !nodes_ || size <= nodes_->pKey; }

        Bool has(Void* memory) noexcept
        {
          return nodes_ && nodes_ < memory && memory < nodes_ + 0x10000;
        }

        Node* allocate(UInt8 size) noexcept
        {
          if (!nodes_) {
            nodes_ = (Node*)alloc(1_MiB);
            nodes_->pKey = 0xff;
            nodes_->cPriority = ++size;
            nodes_[size].makeFirst(size);
            return nodes_ + 1;
          }
          UInt8 pKey;
          Node* node{nodes_ + nodes_->cPriority};
          while (node != nodes_)
          {
            if (node->pKey >= size) {
              pKey = node->pKey;
              node = nodes_ + node->pL;
            } else {
              node = nodes_ + node->pR;
            }
          }
          SplitLMR pLMR{splitP3(pKey)};
          Node* memory{pLMR.m};
          pLMR.m = mergeC2(nodes_ + memory->cL, nodes_ + memory->cR);
          Bool needResize{memory->pKey == nodes_->pKey && pLMR.m == nodes_};
          if (memory->size > size) {
            node = memory + size;
            node->init(memory->index + size, memory->size - size);
            if (node->size < 0xff) {
              mergeP3(pLMR.l, pLMR.m, pLMR.r);
              pLMR = splitP3(node->pKey);
            } else {
              needResize = false;
            }
            SplitLR cLR{splitC2(pLMR.m, node->index)};
            pLMR.m = mergeC3(cLR.l, node, cLR.r);
          }
          mergeP3(pLMR.l, pLMR.m, pLMR.r);
          if (needResize) {
            if (nodes_->cPriority) {
              node = nodes_ + nodes_->cPriority;
              while (node->pR) {
                node = nodes_ + node->pR;
              }
              nodes_->pKey = node->pKey;
            } else {
              nodes_->pKey = 0;
            }
          }
          return memory;
        }

        Node* expand(Node* memory, Node* capacity, Node* newCapacity) noexcept
        {
          capacity = alignUpNode(capacity);
          newCapacity = alignUpNode(newCapacity);
          if (capacity < newCapacity && capacity - nodes_ < 0x10000 && isFree(capacity->index, capacity->pKey)) {
            SplitLMR pLMR{splitP3(capacity->pKey)};
            SplitLMR cLMR{splitC3(pLMR.m, capacity->index)};
            pLMR.m = mergeC2(cLMR.l, cLMR.r);
            Bool needResize{capacity->pKey == nodes_->pKey && pLMR.m == nodes_};
            if (capacity + capacity->size > newCapacity) {
              newCapacity->init(newCapacity - nodes_, capacity + capacity->size - newCapacity);
              if (newCapacity->size < 0xff) {
                mergeP3(pLMR.l, pLMR.m, pLMR.r);
                pLMR = splitP3(newCapacity->pKey);
              } else {
                needResize = false;
              }
              SplitLR cLR{splitC2(pLMR.m, newCapacity->index)};
              pLMR.m = mergeC3(cLR.l, newCapacity, cLR.r);
            } else {
              newCapacity = capacity + capacity->size;
            }
            mergeP3(pLMR.l, pLMR.m, pLMR.r);
            if (needResize) {
              if (nodes_->cPriority) {
                Node* node{nodes_ + nodes_->cPriority};
                while (node->pR)
                {
                  node = nodes_ + node->pR;
                }
                nodes_->pKey = node->pKey;
              } else {
                nodes_->pKey = 0;
              }
            }
            return newCapacity;
          }
          return capacity;
        }

        Node* shrink(Node* memory, Node* capacity, Node* newCapacity) noexcept
        {
          capacity = alignUpNode(capacity);
          newCapacity = alignUpNode(newCapacity);
          if (capacity > newCapacity) {
            Node* begin{alignUpPage(newCapacity + 1)};
            Node* end;
            if (capacity - nodes_ < 0x10000) {
              if (isFree(capacity->index, capacity->pKey)) {
                if (newCapacity == nodes_ + 1 && capacity->index + capacity->size == 0x10000) {
                  release(nodes_);
                  nodes_ = nullptr;
                  return newCapacity;
                }
                SplitLMR pLMR{splitP3(capacity->pKey)};
                SplitLMR cLMR{splitC3(pLMR.m, capacity->index)};
                mergeP3(pLMR.l, mergeC2(cLMR.l, cLMR.r), pLMR.r);
                capacity += capacity->size;
                end = min(alignUpPage(cLMR.m + 1), capacity - nodes_ < 0x10000 ? alignDownPage(capacity - 1) : capacity);
              } else {
                end = alignDownPage(capacity - 1);
              }
            } else {
              end = capacity;
            }
            newCapacity->init(newCapacity - nodes_, capacity - newCapacity);
            if (nodes_->pKey < newCapacity->pKey) {
              nodes_->pKey = newCapacity->pKey;
            }
            if (begin != end) {
              recommit(begin, end - begin);
            }
            SplitLMR pLMR{splitP3(newCapacity->pKey)};
            SplitLR cLR{splitC2(pLMR.m, newCapacity->index)};
            mergeP3(pLMR.l, mergeC3(cLR.l, newCapacity, cLR.r), pLMR.r);
            return newCapacity;
          }
          return capacity;
        }

        Void deallocate(Node* memory, Node* capacity) noexcept
        {
          capacity = alignUpNode(capacity);
          Node* begin;
          Node* end;
          if (capacity - nodes_ < 0x10000) {
            if (isFree(capacity->index, capacity->pKey)) {
              SplitLMR pLMR{splitP3(capacity->pKey)};
              SplitLMR cLMR{splitC3(pLMR.m, capacity->index)};
              mergeP3(pLMR.l, mergeC2(cLMR.l, cLMR.r), pLMR.r);
              capacity += capacity->size;
              end = min(alignUpPage(cLMR.m + 1), capacity - nodes_ < 0x10000 ? alignDownPage(capacity - 1) : capacity);
            } else {
              end = alignDownPage(capacity - 1);
            }
          } else {
            end = capacity;
          }
          UInt16 i{memory[-1].index};
          UInt16 s{memory[-1].size};
          if (i && i + s == memory->index && isFree(i, s <= 0xff ? s : 0xff)) {
            begin = nodes_ + i;
            SplitLMR pLMR{splitP3(begin->pKey)};
            SplitLMR cLMR{splitC3(pLMR.m, begin->index)};
            mergeP3(pLMR.l, mergeC2(cLMR.l, cLMR.r), pLMR.r);
            begin = max(alignUpPage(begin + 1), alignDownPage(memory - 1));
            memory = cLMR.m;
            memory->resize(capacity - memory);
          } else {
            memory->init(memory - nodes_, capacity - memory);
            begin = alignUpPage(memory + 1);
          }
          if (memory->index == 1 && memory->size == 0xffff) {
            release(nodes_);
            nodes_ = nullptr;
          } else {
            if (nodes_->pKey < memory->pKey) {
              nodes_->pKey = memory->pKey;
            }
            if (begin != end) {
              recommit(begin, end - begin);
            }
            SplitLMR pLMR{splitP3(memory->pKey)};
            SplitLR cLR{splitC2(pLMR.m, memory->index)};
            mergeP3(pLMR.l, mergeC3(cLR.l, memory, cLR.r), pLMR.r);
          }
        }

        Void printTreeHorizontal(Node* r, Int depth = 0, const std::string& prefix = "")
        {
          if (r == nodes_) return;
          printTreeHorizontal(nodes_ + r->pR, depth + 1, prefix);
          std::cout << std::string(depth * 4, ' ') << r->size << std::endl;
          printTreeHorizontal(nodes_ + r->pL, depth + 1, prefix);
        }

      private:
        Node* nodes_;
      };

      static UInt16 alignUpNode(UInt16 size) noexcept { return size + 0xf >> 4; }

    public:
      [[nodiscard]] static Void* allocate(UInt16 size) noexcept
      {
        size = alignUpNode(size);
        for (Chunk* i{chunks_}; i < end_; ++i) {
          if (i->hasSize(size)) {
            return i->allocate(size);
          }
        }
        return end_++->allocate(size);
      }

      [[nodiscard]] static Void* expand(Void* memory, Void* capacity, Void* newCapacity) noexcept
      {
        for (Chunk* i{chunks_}; i < end_; ++i) {
          if (i->has(memory)) {
            return i->expand((Chunk::Node*)memory, (Chunk::Node*)capacity, (Chunk::Node*)newCapacity);
          }
        }
        return capacity;
      }

      [[nodiscard]] static Void* shrink(Void* memory, Void* capacity, Void* newCapacity) noexcept
      {
        for (Chunk* i{chunks_}; i < end_; ++i) {
          if (i->has(memory)) {
            capacity = i->shrink((Chunk::Node*)memory, (Chunk::Node*)capacity, (Chunk::Node*)newCapacity);
            if (!*i) {
              if (i < --end_) {
                Chunk* i1{i + 1};
                *i = *i1;
                while (i1 < end_) {
                  *++i = *++i1;
                }
                end_->reset();
              }
              if (!((UInt64)end_ & 0xfff)) {
                recommit(end_, PageSize);
              }
            }
            return capacity;
          }
        }
        return capacity;
      }

      static Void deallocate(Void* memory, Void* capacity) noexcept
      {
        for (Chunk* i{chunks_}; i < end_; ++i) {
          if (i->has(memory)) {
            i->deallocate((Chunk::Node*)memory, (Chunk::Node*)capacity);
            if (!*i) {
              if (i != --end_) {
                *i = *end_;
              }
              if (!((UInt64)end_ & 0xfff)) {
                recommit(end_, PageSize);
              }
            }
            return;
          }
        }
      }

    private:
      static Chunk* chunks_;
      static Chunk* end_;
    };

  public:
    [[nodiscard]] static Void* allocate(Size size) noexcept
    {
      if (size <= 4080) {
        return SmallAllocator::allocate(size);
      } else {
        return LargeAllocator::allocate(size);
      }
    }

    [[nodiscard]] static Void* expand(Void* memory, Void* capacity, Void* newCapacity) noexcept
    {
      if ((UInt64)capacity - (UInt64)memory <= 4080) {
        return SmallAllocator::expand(memory, capacity, newCapacity);
      } else {
        return LargeAllocator::expand(memory, capacity, newCapacity);
      }
    }

    [[nodiscard]] static Void* shrink(Void* memory, Void* capacity, Void* newCapacity) noexcept
    {
      if ((UInt64)capacity - (UInt64)memory <= 4080) {
        return SmallAllocator::shrink(memory, capacity, newCapacity);
      } else {
        return LargeAllocator::shrink(memory, capacity, newCapacity);
      }
    }

    static Void deallocate(Void* memory, Void* capacity) noexcept
    {
      if ((UInt64)capacity - (UInt64)memory <= 4080) {
        return SmallAllocator::deallocate(memory, capacity);
      } else {
        return LargeAllocator::deallocate(memory, capacity);
      }
    }
  };

  CompactBaseAllocator::SmallAllocator::Chunk* CompactBaseAllocator::SmallAllocator::chunks_{(Chunk*)alloc(0x200000)};
  CompactBaseAllocator::SmallAllocator::Chunk* CompactBaseAllocator::SmallAllocator::end_{chunks_};

  template <typename T, typename Allocator>
  class BaseAllocator
  {
  public:
    using Type = T;

    [[nodiscard]] Type* allocate(Size size) noexcept
    {
      return (Type*)Allocator::allocate(size * sizeof(Type));
    }

    [[nodiscard]] Type* expand(Type* memory, Type* capacity, Type* newCapacity) noexcept
    {
      return memory + ((Type*)Allocator::expand(memory, capacity, newCapacity) - memory);
    }

    [[nodiscard]] Type* shrink(Type* memory, Type* capacity, Type* newCapacity) noexcept
    {
      return memory + ((Type*)Allocator::shrink(memory, capacity, newCapacity) - memory);
    }

    Void deallocate(Type* memory, Type* capacity) noexcept
    {
      Allocator::deallocate(memory, capacity);
    }
  };

  export
  {
    template <typename Type>
    using CompactAllocator = BaseAllocator<Type, CompactBaseAllocator>;

    template <typename Type>
    using Allocator = CompactAllocator<Type>;

/*
    template <Size sizeBlock>
    requires (sizeBlock < 8)
    class FixedChunk final
    {
    public:
      Bool reserve(UInt8 nBlock) noexcept
      {
        begin_ = malloc(nBlock * sizeBlock);
        end_ = block(nBlock--);
        Byte* p{begin_};
        for (UInt8 i{}; i < nBlock; p += sizeBlock) {
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
      Byte* block(UInt8 i) noexcept { return begin_ + i * sizeBlock; }

    private:
      Byte* begin_{};
      Byte* end_{};
      Int8 top_{};
      Int8 usedBlocks_{};
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
