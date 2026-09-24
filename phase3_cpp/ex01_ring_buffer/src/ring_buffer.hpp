// Fixed-capacity FIFO ring buffer (header-only template).
//
// Compare with the C version you would write:
//
//   typedef struct { void *buf; size_t elem_size, cap, head, tail, count; } ring_t;
//   void ring_init(ring_t *r, void *storage, size_t elem_size, size_t cap);
//   bool ring_push(ring_t *r, const void *elem);
//   bool ring_pop(ring_t *r, void *out);
//
// In C++ the element type and capacity are template parameters, storage is a
// member (no separate buffer to pass in), and the constructor replaces
// ring_init() so the object can never be used uninitialized.
//
// Rules: no malloc/new, no std::vector, no exceptions, no <iostream>.

#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <cstddef>

template <typename T, std::size_t N>
class RingBuffer {
    static_assert(N > 0, "RingBuffer capacity must be at least 1");

public:
    // Construct an empty buffer.
    RingBuffer() = default;

    // Append `value` at the tail. Returns false (and changes nothing) if full.
    bool push(const T &value)
    {
        // TODO: implement
        (void)value;
        return false;
    }

    // Remove the oldest element into `out`. Returns false (and leaves `out`
    // untouched) if empty.
    bool pop(T &out)
    {
        // TODO: implement
        (void)out;
        return false;
    }

    // Number of elements currently stored.
    std::size_t size() const
    {
        // TODO: implement
        return 0;
    }

    bool empty() const
    {
        // TODO: implement
        return false;
    }

    bool full() const
    {
        // TODO: implement
        return false;
    }

    // Discard all elements.
    void clear()
    {
        // TODO: implement
    }

    static constexpr std::size_t capacity() { return N; }

private:
    std::array<T, N> buf_{};
    std::size_t head_ = 0;   // index of the oldest element
    std::size_t count_ = 0;  // number of stored elements
};

#endif // RING_BUFFER_HPP
