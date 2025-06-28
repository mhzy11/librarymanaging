#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <ctime>
#include <memory>
#include "book.h"
#include "reader.h"

class BorrowRecord {
private:
    time_t m_borrowDate;
    time_t m_dueDate;
    bool m_isReturned;
    std::shared_ptr<Book> m_book;
    std::shared_ptr<Reader> m_reader;
public:
    BorrowRecord(std::shared_ptr<Book> b, std::shared_ptr<Reader> r)
        : m_book(b), m_reader(r), m_isReturned(false),
        m_borrowDate(time(nullptr)),
        m_dueDate(time(nullptr) + r->GetBorrowPeriod() * 24 * 60 * 60) {}

    time_t GetBorrowDate() const { return m_borrowDate; }
    time_t GetDueDate() const { return m_dueDate; }
    bool IsReturned() const { return m_isReturned; }

    void SetReturned(bool returned) {
        m_isReturned = returned;
        if (returned && m_book) {
            m_book->Return();
        }
    }

    std::shared_ptr<Book> GetBook() const { return m_book; }
    std::shared_ptr<Reader> GetReader() const { return m_reader; }

    std::string GetBookName() const {
        return m_book ? m_book->GetName() : "未知书籍";
    }

    std::string GetReaderName() const {
        return m_reader ? m_reader->GetName() : "未知读者";
    }
};

#endif // BORROWRECORD_H
