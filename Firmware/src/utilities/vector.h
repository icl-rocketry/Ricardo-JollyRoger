#pragma once

/**
 * @brief Append a vector of elements to the end of another vector
 * 
 * @author Max Hallgarten La Casta
 * 
 * @tparam T Vector element type
 * @param[in] dest Destination vector
 * @param[out] src Source vector
 */
template <class T>
inline void append(std::vector<T> &dest, const std::vector<T> &src)
{
    // Append the source vector to the end of the destination vector
    dest.insert(dest.end(), src.begin(), src.end());
};
