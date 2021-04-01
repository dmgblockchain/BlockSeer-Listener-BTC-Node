//
// Created by mattcoding4days on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the TransactionFilter class.
//

#ifndef SRC_LIBDMG_CON_MANAGER_H_
#define SRC_LIBDMG_CON_MANAGER_H_

// standard lib
#include <memory>
#include <string>
#include <utility>

// sql library
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>


namespace libDMG {
/* NOTE: _ConnectionDetails should not be used directly
     * */
struct ConnectionDetails {
protected:
    std::string m_host;
    std::string m_user;
    std::string m_pass;
    std::string m_database_name;

public:
    ConnectionDetails();
};


/* NOTE: ConnectionManager only handles connections to mysql and executes queries,
 * it is not meant to have any other functionality. Encapsulating the sql Library functionality
 * in this object is crucial, since MySQL_Driver is a Singleton, it is very rigid to work with.
 * sql::Connection also has a private copy constructor, so passing it around is very difficult as only
 * move semantics can be utilized. A safer approach would have been to make unique_ptr objects out of
 * Driver and Connection, but that approach lead to far more code, and was becoming un-managable. In the name
 * of simplicity I chose the regular ol' pointer approach. If the code fails here for any reason, it is a possibillity
 * the destructor won't be called. But the bitcoin StartShutdown() function should solve that problem.
 *
 * NOTE: since MySQL_Driver is a singleton it deletes it's own memory, the destructor is private and thus cannot be called.
 * */
class ConnectionManager : public ConnectionDetails
{
private:
    sql::mysql::MySQL_Driver* m_driver;

protected:
    // NOTE: keep these in proteced incase we want to inherit
    sql::Connection* m_internal_con;
    sql::Statement* m_statement;

public:
    // NOTE: When instantiating a ConnectionManager obj, must wrap it in try/catch
    ConnectionManager();

    // NOTE: since MySQL_Driver is a singleton it deletes it's own memory, the destructor is private and thus cannot be called.
    ~ConnectionManager();

    /* NOTE: exec_dbpc: execute database procedure
     * This method calls a procedure on btc sql database called is_bad_actor, you need only pass the btc
     * address in question. The returned shared pointer holds all the information that we need. The returned
     * pointer should be passed to the validator_actor method from TransactionValidator class
     * */
    [[nodiscard]] std::shared_ptr<sql::ResultSet>
    exec_dbpc(const std::string& _address, const std::string& _callback) const noexcept;
};

// A safe way to build a connection, this is so I don't have to pollute the
// bitcoin code with try and catch, and deal with try/catch scoping limitations
std::shared_ptr<libDMG::ConnectionManager> conn_manager_factory();
} // namespace libDMG

#endif // SRC_LIBDMG_CON_MANAGER_H_
