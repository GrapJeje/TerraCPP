#ifndef NATIVECHUNKS_MATERIAL_H
#define NATIVECHUNKS_MATERIAL_H

#pragma once
#include <cstdint>

enum class Material : std::uint8_t {
    AIR,
    STONE,
    GRASS_BLOCK,
    DIRT,
    BEDROCK,
    WATER,
    OAK_LOG,
    OAK_LEAVES,
    SHORT_GRASS,
    SEA_GRASS,
    DEEPSLATE,
};

#endif