#include <cstddef>
#include <stdint.h>
#include <list>

using std::list;

class Chunk {
private:
  const int maxSize = 1024;
  int currentPointer = 0;
  uint8_t* data;
public:
  Chunk* prev;

  Chunk() : data(new uint8_t[maxSize]), prev(nullptr) { }

  Chunk(Chunk* prev) : Chunk() {
    this->prev = prev;
  }

  bool couldAllocate(size_t n) {

    return maxSize - currentPointer >= n;
  }

  void* allocate(size_t n) {
    if (n > maxSize) {
      throw "Unpossible allocate memory";
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
  int linksCount;
  int* linksCountPtr;
  Chunk* chunks;
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template< class U > struct rebind { typedef ChunkAllocator<U> other; };

  ChunkAllocator() : chunks(nullptr) {
    linksCount = 1;
    linksCountPtr = &linksCount;
  }

  ChunkAllocator(ChunkAllocator<T>& allocator) {
    linksCountPtr = allocator.linksCountPtr;
    (*linksCountPtr)++;
    chunks = allocator.chunks;
  }

  ChunkAllocator& operator=(ChunkAllocator<T>& allocator) {
    linksCountPtr = allocator.linksCountPtr;
    (*linksCountPtr)++;
    chunks = allocator.chunks;

    return *this;
  }

  pointer allocate(size_type n) {
    auto currentChunk = chunks;

    while (currentChunk != nullptr) {
      if (currentChunk->couldAllocate(n * sizeof(T))) {
        return (pointer)currentChunk->allocate(n * sizeof(T));
      }

      currentChunk = currentChunk->prev;
    }

    auto newChunk = new Chunk(chunks);
    chunks = newChunk;
    return (pointer)chunks->allocate(n * sizeof(T));
  }

  void deallocate(pointer p, size_type n) { }

  template< class... Args>
  void construct(pointer p,  Args&&... args) {
    new (p) T(args...);
  }

  void destroy(pointer p) {
    p->~T();
  }

  ~ChunkAllocator() {
    if (*linksCountPtr == 1) {
      auto currentChunk = chunks;

      while (currentChunk != nullptr) {
        auto temp = currentChunk;
        currentChunk = currentChunk->prev;

        temp->deallocate();
        delete temp;
      }
    }

    (*linksCountPtr)--;
  }
};
