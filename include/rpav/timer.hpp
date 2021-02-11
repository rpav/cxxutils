#pragma once

#include <chrono>

namespace rpav {

// FIXME: this is all entirely fucked
class timer {
public:
    using clock = std::chrono::high_resolution_clock ;
    using time_point = typename clock::time_point;
    using duration = typename clock::duration;

    using float_dur = std::chrono::duration<double>;
    using time_ms = std::chrono::milliseconds;

private:
    std::chrono::time_point<clock> _start{};

public:
    timer() { reset(); }
    void start() { reset(); }

    duration delta() {
        time_point now = clock::now();
        duration diff = now - _start;
        return diff;
    }

    double delta_ms() {
        return as_ms(delta());
    }

    time_point now() {
        return clock::now();
    }

    static double as_ms(duration d) {
        return std::chrono::duration_cast<time_ms>(d).count();
    }

    void reset() { _start = clock::now(); }
};

class fps_timer {
public:
    enum SampleType {
        None,
        OnFrameCount,
        OnTime,
    };

private:
    timer _wallClock;
    timer _frameClock;
    SampleType _sampleType{None};
    unsigned int _samples{1000};
    unsigned int _frames{0};

    timer::duration _accum{0};

    double _fps{0};
    double _msF{0};

public:
    fps_timer() = default;

    void setSampling(unsigned int samples, SampleType rt = OnTime) {
        _samples = samples;
        _sampleType = rt;
    }

    void start() {
        _wallClock.start();
        _frameClock.start();
    }

    void frameBegin() {
        _frameClock.reset();
    }

    bool frameEnd() {
        ++_frames;

        _accum += _frameClock.delta();
        auto ms = timer::as_ms(_accum);

        if(((_sampleType == OnTime && _wallClock.delta_ms() >= _samples)
            || (_sampleType == OnFrameCount && _frames >= _samples))) {
            _fps = (float)_frames/((float)ms/1000.0);
            _msF = ((double)ms)/_frames;

            _frames = 0;
            _accum = _accum.zero();
            _wallClock.reset();

            return true;
        }

        return false;
    }

    double fps() const {
        if(_sampleType != None) return _fps;
        auto ms = timer::as_ms(_accum);
        return _frames/(ms/1000.0);
    }

    double msF() const {
        if(_sampleType != None) return _msF;
        auto ms = timer::as_ms(_accum);
        return ms/_frames;
    }

    double ms() const {
        return timer::as_ms(_accum);
    }
};

}