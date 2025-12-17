#pragma once

#include <optional>
#include <atomic>


template<typename T, uint32_t SIZE = 128>
class Ring
{
public:
    std::optional<T> remove()
    {
        if (read_pos == write_pos)
        {
            return std::nullopt;
        }
        const auto rp = read_pos;
        T elt = m_ring[rp];
        read_pos = (rp + 1) % SIZE;
        return elt;
    }

    void add(const T& elt)
    {
        // don't care if we overwrite sth, log should fill slowly
        const auto wp = write_pos;
        write_pos = (wp + 1) % SIZE;
        m_ring[write_pos] = elt;
    }

private:
    uint32_t read_pos = 0;
    uint32_t write_pos = 0;
    T m_ring[SIZE];
};

