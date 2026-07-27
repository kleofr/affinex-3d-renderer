// LogStorage.cpp
#include "LogStorage.h"

#include <utility>

namespace AffineX
{

    // ------------------------------------------------------------------------
    // Constructor – pre‑allocates the buffer with default‑constructed entries
    // ------------------------------------------------------------------------
    LogStorage::LogStorage(size_t capacity)
        : m_capacity(capacity)
        , m_buffer(capacity)   // Allocates `capacity` default‑constructed LogEntry objects
        , m_head(0)
        , m_tail(0)
        , m_count(0)
    {
        // All done – buffer is ready
    }

    // ------------------------------------------------------------------------
    // Add a new entry – overwrite oldest if full
    // ------------------------------------------------------------------------
    void LogStorage::addEntry(LogEntry entry)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_count < m_capacity)
        {
            // Buffer not full – write at tail, then advance tail and count
            m_buffer[m_tail] = std::move(entry);
            m_tail = (m_tail + 1) % m_capacity;
            ++m_count;
        }
        else
        {
            // Buffer full – overwrite the oldest entry (at head)
            // Since tail == head in a full buffer, we write to tail (which is head).
            m_buffer[m_tail] = std::move(entry);

            // Advance both head and tail – the "window" slides forward by one
            m_head = (m_head + 1) % m_capacity;
            m_tail = (m_tail + 1) % m_capacity;
        }
    }

    // ------------------------------------------------------------------------
    // Clear all entries – effectively resets the buffer to empty
    // ------------------------------------------------------------------------
    void LogStorage::clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // We don't need to destroy the objects – just reset indices.
        // The entries will be overwritten as new ones arrive.
        m_head = 0;
        m_tail = 0;
        m_count = 0;
    }

    // ------------------------------------------------------------------------
    // Returns the number of entries currently stored
    // ------------------------------------------------------------------------
    size_t LogStorage::getSize() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_count;
    }

    // ------------------------------------------------------------------------
    // Returns the maximum capacity
    // ------------------------------------------------------------------------
    size_t LogStorage::getCapacity() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_capacity;
    }

} // namespace AffineX