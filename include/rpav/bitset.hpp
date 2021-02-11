#pragma once

#include <cstdint>
#include <cstdlib>
#include <array>
#include <vector>

#include <rpav/bits.hpp>
#include <rpav/iterator.hpp>
#include <rpav/ptr.hpp>

namespace rpav {

template<typename T>
class bitref_t {
    T&     bs;
    size_t index;

public:
    bitref_t(T& v, size_t i) : bs{v}, index{i} {}

    bitref_t& operator=(bool v)
    {
        bs.set(index, v);
        return *this;
    }

    operator bool() const { return bs.get(index); }
};

class variable_bitset {

public:
    using word_type                   = uint32_t;
    static constexpr size_t word_size = sizeof(word_type);
    static constexpr size_t bit_size  = word_size * 8;

    using bitref       = bitref_t<variable_bitset>;
    using const_bitref = bitref_t<const variable_bitset>;

    bitref operator[](size_t i) { return {*this, i}; }
    bool   operator[](size_t i) const { return get(i); }

    variable_bitset& set(size_t index, bool value)
    {
        auto wordIndex = index / bit_size;
        auto localBit  = index % bit_size;

        if(wordIndex >= _data.size()) {
            _data.resize(wordIndex + 1);
        }

        word_type mask   = ~(word_type(1) << localBit);
        _data[wordIndex] = (mask & _data[wordIndex]) | (word_type(value) << localBit);

        return *this;
    }

    bool get(size_t index) const
    {
        auto wordIndex = index / bit_size;
        auto localBit  = index % bit_size;

        if(wordIndex < _data.size()) {
            return (_data[wordIndex] & (word_type(1) << localBit)) != 0;
        }

        return false;
    }

    word_type getWord(size_t wordIndex) const
    {
        if(wordIndex < _data.size()) {
            return _data[wordIndex];
        }

        return 0;
    }

    size_t wordSize() const { return _data.size(); }

    size_t size() const { return _data.size() * bit_size; }

    operator bool() const
    {
        for(auto i : _data) {
            if(i) {
                return true;
            }
        }

        return false;
    }

    variable_bitset operator&(const variable_bitset& v) const
    {
        variable_bitset r;
        r._data.resize(v._data.size());
        for(auto [a, b, i] : iter::zip(_data, v._data, iter::from(0))) {
            r._data[i] = a & b;
        }

        return r;
    }

    variable_bitset operator|(const variable_bitset& v) const
    {
        variable_bitset r;
        r._data.resize(v._data.size());
        for(auto [a, b, i] : iter::zip(_data, v._data, iter::from(0))) {
            r._data[i] = a | b;
        }

        return r;
    }

    variable_bitset operator^(const variable_bitset& v) const
    {
        variable_bitset r;
        r._data.resize(v._data.size());
        for(auto [a, b, i] : iter::zip(_data, v._data, iter::from(0))) {
            r._data[i] = a ^ b;
        }

        return r;
    }

    variable_bitset operator~() const
    {
        variable_bitset v;
        v._data.resize(_data.size());
        for(auto [a, i] : iter::zip(_data, iter::from(0))) {
            v._data[i] = ~a;
        }
        return v;
    }

private:
    std::vector<word_type> _data;
};

template<size_t Bits>
struct BitmaskRange {
    size_t start{};
    size_t end{};
};

/* Because std::bitset lacks important things:

   - iterating as T
   - ffs/clz
*/
template<typename T, size_t BitCount>
class basic_bitset {
    static_assert(std::is_integral_v<T>);

public:
    using type   = T;
    using bitset = basic_bitset<T, BitCount>;

    constexpr static size_t t_bits        = std::numeric_limits<T>::digits;
    constexpr static size_t bits          = BitCount;
    constexpr static size_t extra_entries = (BitCount % t_bits ? 1 : 0);

    using bitref    = bitref_t<basic_bitset>;
    using data_type = std::array<T, (BitCount / t_bits) + extra_entries>;

private:
    data_type _data = {};

public:
    basic_bitset() = default;
    basic_bitset(const T& v) { _data[0] = v; }

    template<size_t BC = BitCount>
    basic_bitset(const BitmaskRange<BitCount>& m, std::enable_if_t<(BC > t_bits)>* = 0) {
        auto start = m.start / t_bits;
        auto end = m.end / t_bits;

        _data[start] = bitMaskL<type>(t_bits - (m.start % t_bits), 0);
        if(end != start)
            _data[end] = bitMaskR<type>((m.end % t_bits), 0);

        for(size_t i = start+1; i < end; ++i) {
            _data[i] = ~type{0};
        }
    }

    template<size_t BC = BitCount>
    basic_bitset(const BitmaskRange<BitCount>& m, std::enable_if_t<(BC <= t_bits)>* = 0) {
        _data[0] = bitMaskR<type>(m.end - m.start, m.start);
    }

    bitref operator[](size_t nthBit) { return {*this, nthBit}; }
    bool   operator[](size_t nthBit) const { return get(nthBit); }

    bool get(size_t nthBit) const
    {
        auto         i   = nthBit / t_bits;
        const auto   bit = nthBit % t_bits;
        ref<const T> v   = _data[i];

        const auto test = T(1) << bit;
        return (v & test);
    }

    basic_bitset& set(size_t nthBit, bool val)
    {
        auto       i   = nthBit / t_bits;
        const auto bit = nthBit % t_bits;
        ref<T>     v   = _data[i];

        const auto test = T(1) << bit;
        const auto mask = ~test;

        *v = (mask & *v) | (T(val) << bit);

        return *this;
    }

    size_t size() const { return _data.size() * t_bits; }

    data_type&       data() { return _data; }
    const data_type& data() const { return _data; }

    inline basic_bitset zip_map(const basic_bitset& b, type (*fn)(type, type)) const
    {
        bitset r;

        for(auto&& [a, b, i] : iter::zip(_data, b._data, iter::from(0))) {
            r._data[i] = fn(a, b);
        }

        return r;
    }

    inline basic_bitset& zip_map_this(const basic_bitset& b, type (*fn)(type, type))
    {
        for(auto&& [a, b, i] : iter::zip(_data, b._data, iter::from(0))) {
            _data[i] = fn(a, b);
        }
        return *this;
    }

    inline basic_bitset map(type (*fn)(type)) const
    {
        bitset r;

        for(auto&& [a, i] : iter::zip(_data, iter::from(0))) {
            r._data[i] = fn(a);
        }

        return r;
    }

    inline basic_bitset operator&(const bitset& b) const
    {
        return zip_map(b, [](T a, T b) -> T { return a & b; });
    }

    inline basic_bitset& operator&=(const bitset& b)
    {
        return zip_map_this(b, [](T a, T b) -> T { return a & b; });
    }

    inline basic_bitset operator|(const bitset& b) const
    {
        return zip_map(b, [](T a, T b) -> T { return a | b; });
    }

    inline basic_bitset& operator|=(const bitset& b)
    {
        return zip_map_this(b, [](T a, T b) -> T { return a | b; });
    }

    inline basic_bitset operator^(const bitset& b) const
    {
        return zip_map(b, [](T a, T b) -> T { return a ^ b; });
    }

    inline basic_bitset operator^=(const bitset& b) const
    {
        return zip_map_this(b, [](T a, T b) -> T { return a ^ b; });
    }

    inline basic_bitset operator~() const
    {
        return map([](T a) -> T { return ~a; });
    }

    inline basic_bitset& operator<<=(size_t n)
    {
        const auto topMask = bitMaskL<T>(n, 0);

        type extra{};
        type lastExtra{};

        for(size_t i = 0; i < _data.size(); ++i) {
            extra = (_data[i] & topMask) >> (t_bits - n);
            _data[i] <<= n;
            _data[i] |= lastExtra;
            lastExtra = extra;
        }
        return *this;
    }

    inline basic_bitset& operator>>=(size_t n)
    {
        const auto botMask = bitMaskR<T>(n, 0);

        type extra{};
        type lastExtra{};

        for(size_t i = _data.size(); i > 0; --i) {
            extra = (_data[i - 1] & botMask) << (t_bits - n);
            _data[i - 1] >>= n;
            _data[i - 1] |= lastExtra;
            lastExtra = extra;
        }
        return *this;
    }

    inline basic_bitset operator<<(size_t n) const
    {
        basic_bitset r{*this};
        r <<= n;
        return r;
    }

    inline basic_bitset operator>>(size_t n) const
    {
        basic_bitset r{*this};
        r >>= n;
        return r;
    }

    inline size_t clz() const
    {
        size_t r = 0;
        size_t n = 0;
        for(auto i = _data.size(); i > 0; --i) {
            r = rpav::clz(_data[i - 1]);
            if(r < t_bits || i == 1) break;
            n += t_bits;
        }
        return r + n;
    }

    inline size_t clz(size_t offset) const
    {
        size_t r = 0;
        size_t n = 0;

        type       mask      = ~bitMaskL<type>(offset % t_bits, 0);
        const auto offsIndex = (offset / t_bits);

        for(size_t i = _data.size() - offsIndex; i > 0; --i) {
            r = rpav::clz<type>(_data[i - 1] & mask);
            if(r < t_bits || i == 1) break;
            n += t_bits;
            mask = ~type(0);
        }
        return r + n;
    }

    inline size_t ctz() const
    {
        size_t r = 0;
        size_t n = 0;
        for(size_t i = 0; i < _data.size(); ++i) {
            r = rpav::ctz(_data[i]);
            if(r < t_bits || i == _data.size()-1) break;
            n += t_bits;
        }
        return r + n;
    }

    inline size_t ctz(size_t offset) const
    {
        size_t r = 0;
        size_t n = 0;

        type       mask      = ~bitMaskR<type>(offset % t_bits, 0);
        const auto indexOffs = (offset / t_bits);

        for(auto i = indexOffs; i < _data.size(); ++i) {
            r = rpav::ctz<type>(_data[i] & mask);
            if(r < t_bits || i == _data.size() - 1) break;
            n += t_bits;
            mask = ~type{0};
        }
        return r + n;
    }

    inline size_t popcount() const
    {
        size_t r = 0;

        for(auto i : _data) r += rpav::popcount(i);

        return r;
    }

    // True if all bits are zero
    inline bool empty() const
    {
        for(auto i : _data)
            if(i) return false;

        return true;
    }
};

template<size_t BitCount>
using bitset64 = basic_bitset<uint64_t, BitCount>;

template<size_t BitCount>
using bitset32 = basic_bitset<uint32_t, BitCount>;

template<size_t BitCount>
using bitset16 = basic_bitset<uint16_t, BitCount>;

template<size_t BitCount>
using bitset8 = basic_bitset<uint8_t, BitCount>;

/// Pick a the optimally-sized bitset for N bits
template<size_t N>
struct bitset_for {
    template<size_t M, typename = std::enable_if_t<(M >= 64)>>
    static bitset64<N> filter()
    {
        return {};
    }

    template<size_t M, typename = std::enable_if_t<(M > 16) and (M <= 32)>>
    static bitset32<M> filter()
    {
        return {};
    }

    template<size_t M, typename = std::enable_if_t<(M > 8) and (M <= 16)>>
    static bitset16<M> filter()
    {
        return {};
    }

    template<size_t M, typename = std::enable_if_t<(M <= 8)>>
    static bitset8<M> filter()
    {
        return {};
    }

    using type = decltype(filter<N>());
};

} // namespace rpav
