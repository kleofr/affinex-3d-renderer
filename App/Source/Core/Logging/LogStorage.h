// LogStorage.h
#pragma once

#include "LogEntry.h"

#include <vector>
#include <mutex>
#include <cstddef>

namespace AffineX
{

    // ------------------------------------------------------------------------
    // Thread‑safe ring buffer for log entries.
    // - Owns a fixed‑size buffer of LogEntry objects.
    // - Stores entries in chronological order.
    // - New entries overwrite the oldest when full.
    // - All public methods are thread‑safe.
    // ------------------------------------------------------------------------
    class LogStorage
    {
    public:
        // Default capacity: 10'000 entries
        explicit LogStorage(size_t capacity = 10000);

        // Move‑only – we don't want copies of such a large buffer
        LogStorage(LogStorage&&) = default;
        LogStorage& operator=(LogStorage&&) = default;

        // Disable copying
        LogStorage(const LogStorage&) = delete;
        LogStorage& operator=(const LogStorage&) = delete;

        // ---- Write ----
        // Adds a new entry (takes by value, moves it into the buffer).
        // If the buffer is full, the oldest entry is discarded.
        void addEntry(LogEntry entry);

        // ---- Read ----
        // Returns the current number of entries stored.
        size_t getSize() const;

        // Returns the maximum capacity.
        size_t getCapacity() const;

        // Clears all entries.
        void clear();

        // ---- Iteration ----
        // Calls the given functor for every stored entry, in chronological order
        // (oldest first). The functor is invoked while the mutex is locked,
        // so it should execute quickly and not block.
        //
        // Example:
        //   storage.forEach([](const LogEntry& e) { std::cout << e.format(); });
        template <typename Func>
        void forEach(Func&& func) const;

    private:
        size_t m_capacity;
        std::vector<LogEntry> m_buffer;    // fixed size, pre‑allocated
        size_t m_head = 0;                 // index of the oldest entry
        size_t m_tail = 0;                 // index where the next entry will be written
        size_t m_count = 0;                // number of entries currently stored
        mutable std::mutex m_mutex;
    };

    // ------------------------------------------------------------------------
    // Template implementation (must be in header)
    // ------------------------------------------------------------------------
    template <typename Func>
    void LogStorage::forEach(Func&& func) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_count == 0)
            return;

        size_t idx = m_head;
        for (size_t i = 0; i < m_count; ++i)
        {
            func(m_buffer[idx]);
            idx = (idx + 1) % m_capacity;
        }
    }

} // namespace AffineX