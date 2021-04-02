//
// Created by mattcoding4days on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the TransactionFilter class.
//

#ifndef BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_TX_VALIDATOR_H
#define BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_TX_VALIDATOR_H

// standard lib
#include <string>
#include <utility>
#include <memory>

// bitcoin core headers
#include <logging.h>
#include <core_io.h>
#include <rpc/util.h>
#include <univalue.h>

// sql library
#include <cppconn/resultset.h>


namespace libDMG
{

  /* NOTE: TransactionValidator will be used to check for the database for bad actor
   * addresses, against the current incoming and outgoing addresses that are being
   * processed
   * */
  class TransactionValidator
  {
    public:
      TransactionValidator();

      bool is_valid_addr(std::shared_ptr<sql::ResultSet> &res, const std::string &_callback) noexcept;
  };

} // libDMG namespace

#endif //BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
