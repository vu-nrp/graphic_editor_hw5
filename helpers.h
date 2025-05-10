#pragma once

#include <sstream>
#include "types.h"

//!
//! \brief to_str - value to string representation function
//! \param val - input value
//! \return - string representation
//!

template<typename T>
std::string to_str(const T &val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

//!
//! \brief print_pair - print pair of same type value delimited by delim
//! \param val - pair value
//! \param delim - pair item delimiter
//! \return - string representation
//!

template<typename T>
std::string print_pair(const simple_pair<T> &val, const std::string &delim)
{
    return to_str(val.first) + delim + to_str(val.second);
}
