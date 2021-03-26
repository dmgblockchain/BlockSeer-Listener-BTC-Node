//
// Created by mattcoding4days on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the TransactionFilter class.
//

#ifndef BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
#define BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H

#include <utility>

#include <core_io.h>
#include <rpc/util.h>
#include <univalue.h>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


namespace libDMG
{
    class TransactionValidator
    {
    private:
        struct SQLConnection
        {
            SQLConnection() = default;
        };

    public:
        explicit TransactionValidator();
    };

    TransactionValidator::TransactionValidator(void) = default;
}

#endif //BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H