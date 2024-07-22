#pragma once

#define LOG(str) \
    std::cout << __FILE__ << ":" \
        << __LINE__ << " " << str << std::endl;