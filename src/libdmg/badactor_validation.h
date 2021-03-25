//
// Created by matt on 3/24/21.
// It is a good idea to separate all DMG's source code into its own folder, essentially
// acting as a header only library, at least for the Filtration class.
//

#ifndef BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
#define BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H

#include <string>
#include <iostream>
#include <utility>

namespace libDMG {
    class TransactionFilter {
    public:
        explicit TransactionFilter(std::string p_name);

        void get_name();

    private:
        std::string m_name;

    };

    inline TransactionFilter::TransactionFilter(std::string p_name)
            : m_name(std::move(p_name)) {}

    inline void TransactionFilter::get_name() { return this->m_name; }
}

#endif //BLOCKSEER_LISTENER_BTC_NODE_BADACTOR_VALIDATION_H
