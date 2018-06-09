//
// Created by ¶­å· on 2018/6/10.
//

#ifndef LEARNCPP_MAX_HPP
#define LEARNCPP_MAX_HPP

template <typename T>
inline T const& max (T const& a, T const& b)
{
    // if a < b then use b else use a
    return  a < b ? b : a;
}

template <typename T, T VALUE>
inline T const add (T const a)
{
    // if a < b then use b else use a
    return  a + VALUE;
}
#endif //LEARNCPP_MAX_HPP
