//
// Created by mattcoding4days on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the TransactionFilter class.
//

#ifndef BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
#define BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H

// standard lib
#include <cstdlib>
#include <string>
#include <utility>
#include <memory>

// bitcoin core headers
#include <logging.h>
#include <core_io.h>
#include <rpc/util.h>
#include <univalue.h>

// sql library
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


namespace libDMG
{
    namespace PrivateBadActorDB
    {
        // Make an inner name space for SQLConnection, as it should not be used directly.
        class SQLConnection
        {
        private:
            // The sql library only offers support for raw pointers,
            // so I must write this code as if I am some sort of Pilgrim
            sql::Statement *m_statement;
            sql::ResultSet *m_result;
            sql::Driver *m_driver;
            sql::Connection *m_connection;

        public:
            SQLConnection();

            ~SQLConnection();

            bool make_connection();
        };

        // Start Of SQLConnection methods
        SQLConnection::SQLConnection()
                : m_statement(nullptr),
                  m_result(nullptr),
                  m_driver(nullptr),
                  m_connection(nullptr)
        {
            LogPrintf("Building SQLConnector Object");
        }

        SQLConnection::~SQLConnection()
        {
            // statement and connection are the only objects that require the memory
            // to be freed.
            delete this->m_statement;
            delete this->m_connection;
            this->m_result = nullptr;
            this->m_driver = nullptr;
        }

        inline bool SQLConnection::make_connection()
        {
            /* NOTE: AWS does have a C++ API for getting all relevant instance information, like secrets,
             * that maybe a better way to go in the future, but for now this is a simple efficient way to
             * grab environment variables.
             * */
            try {
                this->m_driver = get_driver_instance();
                this->m_connection = m_driver->connect(std::getenv("MYSQL_HOST"),
                                                       std::getenv("MYSQL_USER"),
                                                       std::getenv("MYSQL_PASSWORD"));
            } catch (const sql::SQLException &error) {
                // Emergency shutdown, we couldn't connect to the database for some reason
                // if this happens, call StartShutdown() method defined in Shutdown.h, this will flush the node state
                // and kill all threads safely.
                LogPrintf("Error: %s", std::string(error.what()).c_str());
                LogPrintf("Error Code: %d", error.getErrorCode());
                LogPrintf("SQL State: %s", std::string(error.getSQLState()).c_str());

                return false;
            }
            return true;
        }
    } // PrivateBadActorDB namespace


    class TransactionValidator
    {
    private:
        // use a unique smart pointer, when the Transaction Validator falls out of scope the SQLConnection
        // object will be destroyed automatically, thus calling the SQLConnections destructor, and null/delete
        // all of the raw pointers
        std::unique_ptr<PrivateBadActorDB::SQLConnection> m_sql_connection = std::make_unique<PrivateBadActorDB::SQLConnection>();

    public:
        TransactionValidator();

        bool is_bad_actor();

    };

    TransactionValidator::TransactionValidator() = default;

    inline bool TransactionValidator::is_bad_actor()
    {
        return true;
    }
} // libDMG namespace

#endif //BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
