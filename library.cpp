#include "library.h"
#include <algorithm>

ErrorCode Library::RemoveBook(const std::string& name) {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                           [&name](const std::shared_ptr<Book>& book) {
                               return book->GetName() == name;
                           });

    if (it != m_books.end()) {
        m_books.erase(it);
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::BOOK_NOT_FOUND;
}

std::shared_ptr<Book> Library::FindBook(const std::string& name, ErrorCode& error) {
    for (const auto& book : m_books) {
        if (book->GetName() == name) {
            error = ErrorCode::SUCCESS;
            return book;
        }
    }
    error = ErrorCode::BOOK_NOT_FOUND;
    return nullptr;
}

std::shared_ptr<Reader> Library::FindReader(const std::string& name, ErrorCode& error) {
    for (const auto& reader : m_readers) {
        if (reader->GetName() == name) {
            error = ErrorCode::SUCCESS;
            return reader;
        }
    }
    error = ErrorCode::READER_NOT_FOUND;
    return nullptr;
}

ErrorCode Library::BorrowBook(const std::string& bookName, const std::string& readerName) {
    ErrorCode error;
    auto book = FindBook(bookName, error);
    if (error != ErrorCode::SUCCESS) return error;

    auto reader = FindReader(readerName, error);
    if (error != ErrorCode::SUCCESS) return error;

    if (book->IsBorrowed()) {
        return ErrorCode::BOOK_ALREADY_BORROWED;
    }

    if (reader->GetBorrowedCount() >= reader->GetMaxBorrowCount()) {
        return ErrorCode::BORROW_LIMIT_EXCEEDED;
    }

    if (book->Borrow()) {
        reader->BorrowBook(book);
        m_records.push_back(std::make_shared<BorrowRecord>(book, reader));
        std::cout << "借阅成功！" << std::endl;
        return ErrorCode::SUCCESS;
    }
    return ErrorCode::BOOK_ALREADY_BORROWED;
}

ErrorCode Library::ReturnBook(const std::string& bookName, const std::string& readerName) {
    ErrorCode error;
    auto book = FindBook(bookName, error);
    if (error != ErrorCode::SUCCESS) return error;

    auto reader = FindReader(readerName, error);
    if (error != ErrorCode::SUCCESS) return error;

    bool found = false;
    for (const auto& record : m_records) {
        if (record->GetBook() == book &&
            record->GetReader() == reader &&
            !record->IsReturned()) {

            time_t returnDate = time(nullptr);
            record->SetReturned(true);
            reader->ReturnBook(book);

            std::cout << "归还成功！" << std::endl;

            if (returnDate > record->GetDueDate()) {
                int overdueDays = (returnDate - record->GetDueDate()) / (24 * 60 * 60);
                double fine = overdueDays * m_finePerDay;
                reader->AddFine(fine);
                std::cout << "超期 " << overdueDays << " 天，罚款 " << fine << " 元。" << std::endl;
            }
            found = true;
            return ErrorCode::SUCCESS;
        }
    }

    if (!found) {
        std::cout << "未借该书,归还失败" << std::endl;
        return ErrorCode::BOOK_NOT_FOUND;
    }
    return ErrorCode::SUCCESS;
}

void Library::PrintBooks() const {
    std::cout.imbue(std::locale(""));
    std::cout << "这里有 " << m_books.size() << " 种书:" << std::endl;
    for (const auto& book : m_books) {
        std::cout << "ID=" << book->GetID() << "; 数量=" << book->GetNUM()
            << ";  名字:" << book->GetName()
            << ";  作者:" << book->GetAuthor()
            << ";  类型: " << book->GetBookType() << ";";

        if (const auto* tb = dynamic_cast<const TextBook*>(book.get())) {
            std::cout << "科目: " << tb->GetSub() << ";" << std::endl;
        } else if (const auto* nv = dynamic_cast<const Novel*>(book.get())) {
            std::cout << "内容: " << nv->Getinty() << ";" << std::endl;
        } else {
            std::cout << std::endl;
        }
    }
}

void Library::PrintReaders() const {
    std::cout << "读者信息:" << std::endl;
    for (const auto& reader : m_readers) {
        std::cout << "姓名: " << reader->GetName()
            << "; 类型: " << reader->GetReaderType()
            << "; 罚款: " << reader->GetFine() << " 元;"
            << "; 已借: " << reader->GetBorrowedCount()
            << "/" << reader->GetMaxBorrowCount() << std::endl;
    }
}

void Library::QueryReaderBorrowRecords(const std::string& readerName) const {
    ErrorCode error;
    auto reader = FindReader(readerName, error);
    if (error != ErrorCode::SUCCESS) {
        std::cout << "未找到读者: " << readerName << std::endl;
        return;
    }

    std::cout << "读者 " << reader->GetName() << " 的借阅记录：" << std::endl;
    bool hasRecords = false;

    for (const auto& record : m_records) {
        if (record->GetReader() == reader) {
            hasRecords = true;
            std::cout << "书名：" << record->GetBookName()
                      << "，借阅日期：" << FormatTime(record->GetBorrowDate())
                      << "，应还日期：" << FormatTime(record->GetDueDate())
                      << "，状态：" << (record->IsReturned() ? "已归还" : "未归还") << std::endl;

            if (!record->IsReturned()) {
                time_t now = time(nullptr);
                if (now > record->GetDueDate()) {
                    int overdueDays = (now - record->GetDueDate()) / (24 * 60 * 60);
                    std::cout << "    已超期 " << overdueDays << " 天" << std::endl;
                } else {
                    int remainingDays = (record->GetDueDate() - now) / (24 * 60 * 60);
                    std::cout << "    剩余 " << remainingDays << " 天到期" << std::endl;
                }
            }
        }
    }

    if (!hasRecords) {
        std::cout << "没有找到借阅记录。" << std::endl;
    }
}

ErrorCode Library::SaveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return ErrorCode::FILE_OPERATION_FAILED;
    }

    for (const auto& book : m_books) {
        if (const auto* tb = dynamic_cast<const TextBook*>(book.get())) {
            file << "TextBook " << tb->GetName() << " " << tb->GetSub() << " "
                 << tb->GetAuthor() << " " << tb->GetIntroduction() << " "
                 << tb->GetID() << " " << tb->GetNUM() << std::endl;
        } else if (const auto* nv = dynamic_cast<const Novel*>(book.get())) {
            file << "Novel " << nv->GetName() << " " << nv->Getinty() << " "
                 << nv->GetAuthor() << " " << nv->GetIntroduction() << " "
                 << nv->GetID() << " " << nv->GetNUM() << std::endl;
        } else {
            file << "Book " << book->GetName() << " " << book->GetAuthor() << " "
                 << book->GetIntroduction() << " " << book->GetID() << " "
                 << book->GetNUM() << std::endl;
        }
    }

    for (const auto& reader : m_readers) {
        if (const auto* vr = dynamic_cast<const VIPReader*>(reader.get())) {
            file << "VIPReader " << vr->GetName() << " " << vr->GetID() << " "
                 << vr->GetFine() << std::endl;
        } else {
            file << "Reader " << reader->GetName() << " " << reader->GetID() << " "
                 << reader->GetFine() << " " << reader->GetBorrowPeriod() << std::endl;
        }
    }

    for (const auto& record : m_records) {
        file << "BorrowRecord " << record->GetBookName() << " "
             << record->GetReaderName() << " " << record->GetBorrowDate() << " "
             << record->GetDueDate() << " " << record->IsReturned() << std::endl;
    }

    file.close();
    return ErrorCode::SUCCESS;
}

ErrorCode Library::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "没有找到图书馆数据文件，将创建新文件。" << std::endl;
        return ErrorCode::FILE_OPERATION_FAILED;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "Book") {
            std::string name, author, intro;
            int id, num;
            iss >> name >> author >> intro >> id >> num;
            AddBook(std::make_shared<Book>(id, num, name, intro, author));
        }
        else if (type == "TextBook") {
            std::string name, sub, author, intro;
            int id, num;
            iss >> name >> sub >> author >> intro >> id >> num;
            AddBook(std::make_shared<TextBook>(id, num, name, intro, author, sub));
        }
        else if (type == "Novel") {
            std::string name, gtyp, author, intro;
            int id, num;
            iss >> name >> gtyp >> author >> intro >> id >> num;
            AddBook(std::make_shared<Novel>(id, num, name, intro, author, gtyp));
        }
        else if (type == "Reader") {
            std::string name;
            int id, period;
            double fine;
            iss >> name >> id >> fine >> period;
            auto reader = std::make_shared<Reader>(name, id, period);
            reader->AddFine(fine);
            AddReader(reader);
        }
        else if (type == "VIPReader") {
            std::string name;
            int id;
            double fine;
            iss >> name >> id >> fine;
            auto reader = std::make_shared<VIPReader>(name, id);
            reader->AddFine(fine);
            AddReader(reader);
        }
        else if (type == "BorrowRecord") {
            std::string bookName, readerName;
            time_t borrowDate, dueDate;
            bool returned;
            iss >> bookName >> readerName >> borrowDate >> dueDate >> returned;

            ErrorCode error;
            auto book = FindBook(bookName, error);
            if (error != ErrorCode::SUCCESS) continue;

            auto reader = FindReader(readerName, error);
            if (error != ErrorCode::SUCCESS) continue;

            auto record = std::make_shared<BorrowRecord>(book, reader);
            record->SetReturned(returned);
            m_records.push_back(record);
        }
    }

    file.close();
    return ErrorCode::SUCCESS;
}
