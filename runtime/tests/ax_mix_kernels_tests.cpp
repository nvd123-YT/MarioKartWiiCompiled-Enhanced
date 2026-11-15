#include "ax_mix_kernels.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
uint32_t g_random = 0x12345678u;
unsigned g_checks = 0;

uint32_t NextRandom() {
    g_random ^= g_random << 13;
    g_random ^= g_random >> 17;
    g_random ^= g_random << 5;
    return g_random;
}

int32_t RandomBounded(int32_t limit) {
    const uint32_t span = static_cast<uint32_t>(limit) * 2u + 1u;
    return static_cast<int32_t>(NextRandom() % span) - limit;
}

void Require(bool condition, const std::string& message) {
    ++g_checks;
    if (!condition) throw std::runtime_error(message);
}

constexpr uint32_t kCounts[] = {
    0, 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 95, 96, 97
};

void TestMixAddRamp() {
    constexpr uint16_t volumes[] = {0, 1, 0x7FFF, 0x8000, 0xFFFF};
    constexpr uint16_t deltas[] = {0, 1, 0x4000, 0x7FFF, 0xFFFE, 0xFFFF};
    for (uint32_t count : kCounts) {
        std::vector<int16_t> input(count + 1);
        for (auto& value : input) value = static_cast<int16_t>(NextRandom());
        for (uint16_t volume : volumes) for (uint16_t delta : deltas) {
            std::vector<int32_t> expected(count + 1), actual(count + 1);
            for (uint32_t i = 0; i < count; ++i) {
                expected[i] = actual[i] = RandomBounded(100000000);
            }
            const int16_t initialDpop = static_cast<int16_t>(NextRandom());
            int16_t expectedDpop = initialDpop;
            int16_t actualDpop = initialDpop;
            const uint16_t expectedVolume = AxMixKernels::MixAddRampScalar(
                expected.data(), input.data(), count, volume, delta, expectedDpop);
            const uint16_t actualVolume = AxMixKernels::MixAddRamp(
                actual.data(), input.data(), count, volume, delta, actualDpop);
            Require(expected == actual, "MixAddRamp output differs from scalar reference");
            Require(expectedVolume == actualVolume, "MixAddRamp final volume differs");
            Require(expectedDpop == actualDpop, "MixAddRamp de-pop sample differs");
        }
    }
}

void TestScaleRamp() {
    constexpr uint16_t volumes[] = {0, 1, 0x7FFF, 0x8000, 0xFFFF};
    constexpr uint16_t deltas[] = {0, 1, 0x4000, 0x7FFF, 0xFFFE, 0xFFFF};
    for (uint32_t count : kCounts) {
        std::vector<int16_t> input(count + 1);
        for (auto& value : input) value = static_cast<int16_t>(NextRandom());
        for (uint16_t volume : volumes) for (uint16_t delta : deltas) {
            auto expected = input;
            auto actual = input;
            const uint16_t expectedVolume =
                AxMixKernels::ScaleRampScalar(expected.data(), count, volume, delta);
            const uint16_t actualVolume =
                AxMixKernels::ScaleRamp(actual.data(), count, volume, delta);
            Require(expected == actual, "ScaleRamp samples differ from scalar reference");
            Require(expectedVolume == actualVolume, "ScaleRamp final volume differs");
        }
    }
}

void TestMixAccumRamp32() {
    for (uint32_t count : kCounts) {
        std::vector<int32_t> source(count + 1), expected(count + 1), actual(count + 1);
        std::vector<uint16_t> ramp(count + 1);
        for (uint32_t i = 0; i < count; ++i) {
            source[i] = RandomBounded(900000000);
            expected[i] = actual[i] = RandomBounded(100000000);
            ramp[i] = static_cast<uint16_t>(NextRandom());
        }
        AxMixKernels::MixAccumRamp32Scalar(expected.data(), source.data(), ramp.data(), count);
        AxMixKernels::MixAccumRamp32(actual.data(), source.data(), ramp.data(), count);
        Require(expected == actual, "MixAccumRamp32 differs from scalar reference");
    }
}

void TestBigEndianMarshalling() {
    for (size_t count = 0; count <= 97; ++count) {
        std::vector<int32_t> source(count + 1);
        for (auto& value : source) value = static_cast<int32_t>(NextRandom());

        std::vector<uint8_t> expectedBytes(count * 4 + 3, 0xA5);
        std::vector<uint8_t> actualBytes(count * 4 + 3, 0xA5);
        AxMixKernels::StoreBigEndian32Scalar(expectedBytes.data() + 1, source.data(), count);
        AxMixKernels::StoreBigEndian32(actualBytes.data() + 1, source.data(), count);
        Require(expectedBytes == actualBytes, "StoreBigEndian32 differs from scalar reference");

        std::vector<int32_t> expectedValues(count + 1), actualValues(count + 1);
        AxMixKernels::LoadBigEndian32Scalar(expectedValues.data(), expectedBytes.data() + 1, count);
        AxMixKernels::LoadBigEndian32(actualValues.data(), actualBytes.data() + 1, count);
        Require(expectedValues == actualValues, "LoadBigEndian32 differs from scalar reference");
    }
}
} // namespace

int main() {
    try {
        TestMixAddRamp();
        TestScaleRamp();
        TestMixAccumRamp32();
        TestBigEndianMarshalling();
#if MKW_AX_MIX_AVX2
        constexpr const char* backend = "AVX2";
#elif MKW_AX_MIX_NEON
        constexpr const char* backend = "NEON";
#else
        constexpr const char* backend = "scalar";
#endif
        std::cout << "AX mix kernels (" << backend << "): passed " << g_checks << " checks\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
