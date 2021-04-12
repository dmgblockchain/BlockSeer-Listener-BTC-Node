#include "con_manager.h"

// bitcoin core headers
#include <core_io.h>
#include <logging.h>
#include <memory>
#include <rpc/util.h>
#include <univalue.h>


libDMG::ConnectionDetails::ConnectionDetails()
    : m_host(""),
      m_user(""),
      m_pass(""),
      m_database_name("")
{
    if (const char* temp = std::getenv("SQL_HOST"))
        this->m_host = temp;
    if (const char* temp = std::getenv("SQL_USER"))
        this->m_user = temp;
    if (const char* temp = std::getenv("SQL_PASS"))
        this->m_pass = temp;
    if (const char* temp = std::getenv("SQL_DB_NAME"))
        this->m_database_name = temp;
}


libDMG::ConnectionManager::ConnectionManager()
{
    // : use the con_manager_factory function
    m_driver = sql::mysql::get_driver_instance();
    this->m_internal_con = m_driver->connect(this->m_host, this->m_user, this->m_pass);
    this->m_internal_con->setSchema(m_database_name);
    this->m_statement = this->m_internal_con->createStatement();
}


libDMG::ConnectionManager::~ConnectionManager()
{
    LogPrintf("++ ================== DESTROYING (%s) ===================== ++\n", __func__);
    // Close the connection, it will reconnect
    // when ever ConnectionManager instantiates
    if (!(this->m_internal_con->isClosed()))
        this->m_internal_con->close();

    // clean up the memory on object destruction
    delete this->m_statement;
    delete this->m_internal_con;

    // null out all pointers for extra safety
    this->m_driver = nullptr;
    this->m_internal_con = nullptr;
    this->m_statement = nullptr;
}


std::shared_ptr<sql::ResultSet>
libDMG::ConnectionManager::exec_dbpc(const std::string& _address, const std::string& _callback) const noexcept
{
    if (true) {
        return nullptr;
    }

    try {
        this->m_statement->execute("CALL is_bad_actor('" + _address + "', @bad_actor)");
        std::shared_ptr<sql::ResultSet> result(m_statement->executeQuery("SELECT @bad_actor AS " + _callback));
        return result;
    } catch (const sql::SQLException& error) {
        LogPrintf("libDMG: %s", error.what());
        return nullptr;
    }
}


std::shared_ptr<libDMG::ConnectionManager> libDMG::conn_manager_factory() noexcept
{
    LogPrintf("++ ================== IN (%s) ===================== ++\n", __func__);
    try {
        std::unique_ptr<libDMG::ConnectionManager> connector =
            std::make_unique<libDMG::ConnectionManager>();
        return connector;
    } catch (const sql::SQLException& error) {
        LogPrintf("libDMG: %s", error.what());
        return nullptr;
    }
}
