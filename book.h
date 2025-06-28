#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <memory>

class Book {
private:
    unsigned int m_ID;
    unsigned int m_number;
    std::string m_Name;
    std::string m_Introduction;
    std::string m_Author;
    static unsigned int totalBooks;

public:
    Book() : m_ID(0), m_number(0), m_Name(""), m_Introduction(""), m_Author("") {
        totalBooks++;
    }

    Book(unsigned int id, unsigned int num, const std::string& name,
         const std::string& intro, const std::string& author)
        : m_ID(id), m_number(num), m_Name(name), m_Introduction(intro), m_Author(author) {
        totalBooks++;
    }

    virtual ~Book() {
        totalBooks--;
    }

    Book(const Book& other)
        : m_ID(other.m_ID), m_number(other.m_number), m_Name(other.m_Name),
        m_Introduction(other.m_Introduction), m_Author(other.m_Author) {
        totalBooks++;
    }

    unsigned int GetID() const { return m_ID; }
    unsigned int GetNUM() const { return m_number; }
    const std::string& GetName() const { return m_Name; }
    const std::string& GetIntroduction() const { return m_Introduction; }
    const std::string& GetAuthor() const { return m_Author; }

    bool IsBorrowed() const {
        return m_number <= 0;
    }

    bool Borrow() {
        if (m_number > 0) {
            m_number--;
            return true;
        }
        return false;
    }

    void Return() {
        if (m_number < 100) {
            m_number++;
        }
    }

    static unsigned int GetTotalBooks() { return totalBooks; }
    virtual std::string GetBookType() const { return "普通书籍"; }
};

unsigned int Book::totalBooks = 0;

class TextBook : public Book {
private:
    std::string m_subject;

public:
    TextBook(unsigned int id, unsigned int num, const std::string& name,
             const std::string& intro, const std::string& author, const std::string& sub)
        : Book(id, num, name, intro, author), m_subject(sub) {}

    ~TextBook() {}

    const std::string& GetSub() const { return m_subject; }
    std::string GetBookType() const override { return "教科书"; }
};

class Novel : public Book {
private:
    std::string m_intype;

public:
    Novel(unsigned int id, unsigned int num, const std::string& name,
          const std::string& intro, const std::string& author, const std::string& gtyp)
        : Book(id, num, name, intro, author), m_intype(gtyp) {}

    ~Novel() {}

    const std::string& Getinty() const { return m_intype; }
    std::string GetBookType() const override { return "小说"; }
};

#endif // BOOK_H
