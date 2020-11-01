#include <cstddef>
#include <stdint.h>
#include <list>

using std::list;

class Chunk {
private:
  static const size_t maxSize = 1024;
  size_t currentPointer = 0;
  uint8_t* data;
public:
  Chunk* prev;
  int linksCount;

  Chunk() : data(new uint8_t[maxSize]), prev(nullptr) {
    linksCount = 1;
  }

  Chunk(Chunk* prev) : Chunk() {
    this->prev = prev;
  }

  bool couldAllocate(size_t n) {

    return maxSize - currentPointer >= n;
  }

  void* allocate(size_t n) {
    if (n > maxSize) {
      throw std::bad_alloc();
    }

    void* result = data + currentPointer;
    currentPointer += n;
    return result;
  }

  void deallocate() {
    delete[] data;
  }
};

template< class T>
class ChunkAllocator {
private:
  Chunk* chunks;

  void inscreaseChunkLinks() {
    auto currentChunk = chunks;

    while (currentChunk != nullptr) {
      currentChunk->linksCount++;
      currentChunk = currentChunk->prev;
    }
  }
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template< class U > struct rebind { typedef ChunkAllocator<U> other; };

  ChunkAllocator() : chunks(nullptr) { }

  ChunkAllocator(ChunkAllocator<T>& allocator) {
    chunks = allocator.chunks;
    inscreaseChunkLinks();
  }

  ChunkAllocator& operator=(ChunkAllocator<T>& allocator) {
    chunks = allocator.chunks;
    inscreaseChunkLinks();

    return *this;
  }

  pointer allocate(size_type n) {
    const size_type totalSize = n * sizeof(T);
    auto currentChunk = chunks;

    while (currentChunk != nullptr) {
      if (currentChunk->couldAllocate(totalSize)) {
        return (pointer)currentChunk->allocate(totalSize);
      }

      currentChunk = currentChunk->prev;
    }

    auto newChunk = new Chunk(chunks);
    chunks = newChunk;
    return (pointer)chunks->allocate(totalSize);
  }

  void deallocate(pointer p, size_type n) { }

  template< class... Args>
  void construct(pointer p,  Args&&... args) {
    new (p) T(std::forward<T>(args...));
  }

  void destroy(pointer p) {
    p->~T();
  }

  ~ChunkAllocator() {
    auto currentChunk = chunks;

    while (currentChunk != nullptr) {
      auto temp = currentChunk;
      currentChunk = currentChunk->prev;

      if (temp->linksCount == 1) {
        temp->deallocate();
        delete temp;
      } else {
        temp->linksCount--;

        if (currentChunk->linksCount == 1) {
          temp->prev = nullptr;
        }
      }
    }
  }
};
