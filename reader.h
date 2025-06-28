#ifndef READER_H
#define READER_H

#include <string>
#include <vector>
#include <memory>
#include "book.h"

class Reader {
protected:
    std::string m_name;
    int m_id;
    int m_borrowPeriod;
    double m_fine;
    std::vector<std::shared_ptr<Book>> m_borrowedBooks;
    const int m_maxBorrowCount;

public:
    Reader(const std::string& n, int i, int period = 30, int maxBorrow = 5)
        : m_name(n), m_id(i), m_borrowPeriod(period), m_fine(0), m_maxBorrowCount(maxBorrow) {}

    virtual ~Reader() {}

    std::string GetName() const { return m_name; }
    int GetID() const { return m_id; }
    int GetBorrowPeriod() const { return m_borrowPeriod; }
    double GetFine() const { return m_fine; }

    void AddFine(double amount) { m_fine += amount; }
    const std::vector<std::shared_ptr<Book>>& GetBorrowedBooks() const { return m_borrowedBooks; }

    bool BorrowBook(std::shared_ptr<Book> book) {
        if (m_borrowedBooks.size() >= m_maxBorrowCount) {
            return false;
        }
        m_borrowedBooks.push_back(book);
        return true;
    }

    bool ReturnBook(std::shared_ptr<Book> book) {
        for (auto it = m_borrowedBooks.begin(); it != m_borrowedBooks.end(); ++it) {
            if (*it == book) {
                m_borrowedBooks.erase(it);
                return true;
            }
        }
        return false;
    }

    int GetBorrowedCount() const { return m_borrowedBooks.size(); }
    int GetMaxBorrowCount() const { return m_maxBorrowCount; }

    virtual std::string GetReaderType() const { return "普通会员"; }
};

class VIPReader : public Reader {
public:
    VIPReader(const std::string& n, int i)
        : Reader(n, i, 60, 10) {}

    std::string GetReaderType() const override { return "VIP会员"; }
};

#endif // READER_H
