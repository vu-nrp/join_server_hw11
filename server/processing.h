#pragma once

#include <map>
#include <tuple>
#include <string>
#include <vector>

namespace db {


//!
//!
//!

using Row = std::tuple<size_t, std::string, std::string>;
using Rows = std::vector<Row>;
using Table = std::map<size_t, std::string>;
using Tokens = std::vector<std::string>;

//!
//! \brief The Database class - main storage and processing class
//!

class Database
{
public:

    static const std::string TableA;
    static const std::string TableB;

    std::string insert(const std::string &table, const size_t id, const std::string &name);
    std::string drop(const std::string &table);
    Rows intersection() const;
    Rows difference() const;

private:
    std::string insert(const size_t id, const std::string &name, Table &table);

    // table A, table B
    std::pair<Table, Table> m_tables;

};

//!
//! \brief The Interpreter class - command interpreted class
//!

class Interpreter
{
public:
    std::string process(const std::string &command);

private:
    std::string insert(const Tokens &tokens);
    std::string truncate(const Tokens &tokens);
    std::string intercestion() const;
    std::string difference() const;
    std::string to_string(const Rows &rows) const;

    Database m_database;

};

}
