#pragma once

template <class T>
inline void append(std::vector<T> &dest, const std::vector<T> &src)
{
    // Append the source vector to the end of the destination vector
    dest.insert(dest.end(), src.begin(), src.end());
};
