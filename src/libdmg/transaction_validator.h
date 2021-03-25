//
// Created by mattcoding4days on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the TransactionFilter class.
//

#ifndef BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
#define BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H

#include <string>
#include <iostream>
#include <utility>

namespace libDMG
{
    class TransactionValidator
    {
    public:
        explicit TransactionValidator(std::string);

        std::string get_name();

    private:
        std::string m_name;
    };

    TransactionValidator::TransactionValidator(std::string p_name)
            : m_name(std::move(p_name)) {}

    inline std::string TransactionValidator::get_name() { return this->m_name; }
}

#endif //BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
