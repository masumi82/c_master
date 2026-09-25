// 容量固定の FIFO リングバッファ（ヘッダだけで完結するテンプレート）。
//
// C で書くなら、次のようになるはず。見比べてみよう:
//
//   typedef struct { void *buf; size_t elem_size, cap, head, count; } ring_t;
//   void ring_init(ring_t *r, void *storage, size_t elem_size, size_t cap);
//   bool ring_push(ring_t *r, const void *elem);
//   bool ring_pop(ring_t *r, void *out);
//
// C++ では、要素の型と容量はテンプレート引数になり、格納領域はメンバになる
// （別にバッファを渡す必要がない）。ring_init() の代わりにコンストラクタが
// 走るので、初期化されていない状態で使われることがない。
//
// ルール: malloc/new、std::vector、例外、<iostream> は使わない。

#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <cstddef>

template <typename T, std::size_t N>
class RingBuffer {
    static_assert(N > 0, "RingBuffer capacity must be at least 1");

public:
    // 空のバッファを作る。
    RingBuffer() = default;

    // `value` を末尾に追加する。満杯なら何も変えずに false を返す。
    bool push(const T &value)
    {
        // TODO: 実装する
        (void)value;
        return false;
    }

    // 最も古い要素を取り出して `out` に入れる。空なら `out` に触らずに
    // false を返す。
    bool pop(T &out)
    {
        // TODO: 実装する
        (void)out;
        return false;
    }

    // 今入っている要素の数。
    std::size_t size() const
    {
        // TODO: 実装する
        return 0;
    }

    bool empty() const
    {
        // TODO: 実装する
        return false;
    }

    bool full() const
    {
        // TODO: 実装する
        return false;
    }

    // 全要素を捨てる。
    void clear()
    {
        // TODO: 実装する
    }

    static constexpr std::size_t capacity() { return N; }

private:
    std::array<T, N> buf_{};
    std::size_t head_ = 0;   // 最も古い要素の位置
    std::size_t count_ = 0;  // 入っている要素の数
};

#endif // RING_BUFFER_HPP
