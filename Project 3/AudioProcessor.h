#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <queue>
#include <cmath>
#include <stack>

class AudioProcessor {
    public:
        using audio_t = std::vector<short>;
        audio_t Compress(const audio_t& audio, short threshold, float rate);
        audio_t CutOutSilence(const audio_t& audio, short level, int silenceLength);
        audio_t StretchTwice(const audio_t& audio);
        audio_t Normalize(const audio_t& audio, short normalizeTarget);
};
