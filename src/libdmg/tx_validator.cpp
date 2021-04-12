#include "tx_validator.h"


libDMG::TransactionValidator::TransactionValidator() = default;


bool libDMG::TransactionValidator::is_valid_addr(std::shared_ptr <sql::ResultSet> &res,
                                         const std::string &_callback) noexcept
{
    while (res->next()) {
        if (res->getString(_callback)->empty()) {
            // address wasn't found in database
            return true;
        }
    }
    return false;
}
