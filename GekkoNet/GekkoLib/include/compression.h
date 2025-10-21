#pragma once

#include "gekko_types.h"

#include <iostream>
#include <vector>

namespace Gekko {
    struct Compression {

        static void PrintArray(const uint8_t* data, u32 length) {
            for (u32 i = 0; i < length; i++) {
                std::cout << +data[i] << ' ';
            }
            std::cout << std::endl;
            std::cout << "_______________\n";
        }

        //static void DeltaEncode(uint8_t* data, u32 length) {
        //    u8 last = 0;
        //    u8 current = 0;

        //    for (u32 i = 0; i < length; i++) {
        //        current = data[i];
        //        data[i] = current - last;
        //        last = current;
        //    }
        //}

        //static void DeltaDecode(uint8_t* data, u32 length) {
        //    u8 last = 0;
        //    u8 delta = 0;

        //    for (u32 i = 0; i < length; i++) {
        //        delta = data[i];
        //        data[i] = delta + last;
        //        last = data[i];
        //    }
        //}

        static std::vector<uint8_t> RLEEncode(const uint8_t* data, u32 length) {
            std::vector<uint8_t> result;

            u32 idx = 0;
            u8 count = 0;

            while (idx < length) {
                count = 1;
                while (count != UINT8_MAX && idx + 1 < length && data[idx] == data[idx + 1]) {
                    idx++;
                    count++;
                }
                result.push_back(count);
                result.push_back(data[idx]);
                idx++;
            }
            return result;
        }

        static std::vector<uint8_t> RLEDecode(const uint8_t* data, u32 length) {
            std::vector<uint8_t> result;

            u32 idx = 0;
            u8 count = 0;
            u8 value = 0;

            while (idx < length) {
                count = data[idx];
                value = data[idx + 1];

                for (i32 x = 0; x < count; x++) {
                    result.push_back(value);
                }

                idx += 2;
            }

            return result;
        }

        Compression() = delete;
    };
}
