//
// Created by mattcoding4days on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the TransactionFilter class.
//

#ifndef SRC_LIBDMG_TX_VALIDATOR_H_
#define SRC_LIBDMG_TX_VALIDATOR_H_

// sql library
#include <cppconn/resultset.h>

// standard lib
#include <memory>
#include <string>
#include <utility>

// bitcoin core headers
#include <core_io.h>
#include <logging.h>
#include <rpc/util.h>
#include <univalue.h>


namespace libDMG {

/* NOTE: TransactionValidator will be used to check for the database for bad actor
* addresses, against the current incoming and outgoing addresses that are being
* processed
* */
class TransactionValidator
{
public:
    TransactionValidator();

    bool is_valid_addr(std::shared_ptr<sql::ResultSet>& res, const std::string& _callback) noexcept;
};

} // namespace libDMG

#endif // SRC_LIBDMG_TX_VALIDATOR_H_
