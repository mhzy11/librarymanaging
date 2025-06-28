#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "book.h"
#include "reader.h"
#include "borrowrecord.h"

enum class ErrorCode {
    SUCCESS = 0,
    BOOK_NOT_FOUND,
    READER_NOT_FOUND,
    BOOK_ALREADY_BORROWED,
    BORROW_LIMIT_EXCEEDED,
    FILE_OPERATION_FAILED
};

class Library {
private:
    std::vector<std::shared_ptr<Book>> m_books;
    std::vector<std::shared_ptr<Reader>> m_readers;
    std::vector<std::shared_ptr<BorrowRecord>> m_records;
    double m_finePerDay;

    std::string FormatTime(time_t t) const {
        auto tp = std::chrono::system_clock::from_time_t(t);
        auto localTime = std::chrono::system_clock::to_time_t(tp);
        tm* localTm = localtime(&localTime);
        std::stringstream ss;
        ss << std::put_time(localTm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    Library(double fine = 1.0) : m_finePerDay(fine) {}

    ~Library() = default;

    void AddBook(std::shared_ptr<Book> b) { m_books.push_back(b); }
    ErrorCode RemoveBook(const std::string& name);
    std::shared_ptr<Book> FindBook(const std::string& name, ErrorCode& error);
    const std::vector<std::shared_ptr<Book>>& GetBooks() const { return m_books; }

    void AddReader(std::shared_ptr<Reader> r) { m_readers.push_back(r); }
    std::shared_ptr<Reader> FindReader(const std::string& name, ErrorCode& error);
    const std::vector<std::shared_ptr<Reader>>& GetReaders() const { return m_readers; }

    ErrorCode BorrowBook(const std::string& bookName, const std::string& readerName);
    ErrorCode ReturnBook(const std::string& bookName, const std::string& readerName);

    void PrintBooks() const;
    void PrintReaders() const;
    void QueryReaderBorrowRecords(const std::string& readerName) const;

    ErrorCode SaveToFile(const std::string& filename);
    ErrorCode LoadFromFile(const std::string& filename);
};

#endif // LIBRARY_H
