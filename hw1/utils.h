//
// Created by osama on 11/16/2021.
//

#ifndef HW1_WET_UTILS_H
#define HW1_WET_UTILS_H

#include <exception>

class NodeAlreadyExists : public std::exception{};
class NodeNotExists : public std::exception{};
class TreeEmpty : public std::exception{};
class GroupNotEmpty : public std::exception{};
class SameGroup : public std::exception{};
#endif //HW1_WET_UTILS_H
