#include <sstream>
//#include <iostream>
#include "processing.h"

namespace db {

//!
//! resultCode
//!

std::string resultCode(const std::string &errCode)
{
    return ("ERR " + errCode);
}

std::string resultCode(const size_t errCode = 0)
{
    std::string result;

    switch (errCode) {
    case 0:
        return "OK";
    case 1:
        result = "duplicate primary key";
        break;
    case 2:
        result = "unknown table name";
        break;
    case 3:
        result = "invalid command syntax";
    default:
        break;
    }

    return resultCode(result);
}

//!
//! Database
//!

const std::string Database::TableA {"A"};
const std::string Database::TableB {"B"};

std::string Database::insert(const size_t id, const std::string &name, Table &table)
{
    if (table.find(id) == table.end()) {
        table.insert({id, name});
        return resultCode();
    }

    return resultCode(1);
}

std::string Database::insert(const std::string &table, const size_t id, const std::string &name)
{
    if (table == Database::TableA)
        return insert(id, name, m_tables.first);
    else if (table == Database::TableB)
        return insert(id, name, m_tables.second);
    else
        return resultCode(2);
}

Rows Database::intersection() const
{
    Rows result;

    for (const auto &pair_a: m_tables.first) {
        const auto iter_b = m_tables.second.find(pair_a.first);
        if (iter_b != m_tables.second.cend())
            result.emplace_back(pair_a.first, pair_a.second, iter_b->second);
    }

    return result;
}

Rows Database::difference() const
{
    Rows result;

    for (const auto &pair_a: m_tables.first) {
        const auto iter_b = m_tables.second.find(pair_a.first);
        if (iter_b == m_tables.second.cend())
            result.emplace_back(pair_a.first, pair_a.second, std::string());
    }

    for (const auto &pair_b: m_tables.second) {
        const auto iter_a = m_tables.first.find(pair_b.first);
        if (iter_a == m_tables.first.end())
            result.emplace_back(pair_b.first, std::string(), pair_b.second);
    }

    return result;
}

std::string Database::drop(const std::string &table)
{
    if (table == Database::TableA)
        m_tables.first.clear();
    else if (table == Database::TableB)
        m_tables.second.clear();
    else
        return resultCode(2);

    return resultCode();
}

//!
//! Interpreter
//!

std::string Interpreter::insert(const Tokens &tokens)
{
    return m_database.insert(tokens.at(1), static_cast<size_t>(std::stoll(tokens.at(2))), tokens.at(3));
}

std::string Interpreter::truncate(const Tokens &tokens)
{
    return m_database.drop(tokens.at(1));
}

std::string Interpreter::to_string(const Rows &rows) const
{
    std::string result;

    for (const auto &row: rows)
        result += (std::to_string(std::get<0>(row)) + "," + std::get<1>(row) + "," + std::get<2>(row) + "\n");

//    std::cout << "Rows: " << result << std::endl;

    return (result + resultCode());
}

std::string Interpreter::intercestion() const
{
    return to_string(m_database.intersection());
}

std::string Interpreter::difference() const
{
    return to_string(m_database.difference());
}

std::string Interpreter::process(const std::string &command)
{
    auto trim = [](std::string str)
    {
        static const std::string whitespaces(" \t\f\v\n\r");
        const auto found = str.find_last_not_of(whitespaces);
        return str.erase(found + 1);
    };

    Tokens tokens;
    std::string item;
    std::stringstream stream(trim(command));

    while (std::getline(stream, item, ' '))
        tokens.push_back(std::move(item));

    if (!tokens.empty())
        try {
            if (tokens.front() == "INSERT") {
                return insert(tokens);
            } else if (tokens.front() == "TRUNCATE") {
                return truncate(tokens);
            } else if (tokens.front() == "INTERSECTION") {
                return intercestion();
            } else if (tokens.front() == "SYMMETRIC_DIFFERENCE") {
                return difference();
            } else {
                throw std::logic_error("unknown command " + tokens.front());
            }
        } catch (std::out_of_range &e) {
            return resultCode(3);
        } catch (std::exception &e) {
            return resultCode(std::string(e.what()));
        }

    return {};
}

}
