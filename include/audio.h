#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <arpa/inet.h>

enum class AudioFmt
{
    ERROR,
    NOT_LOADED,
    WAVE
};

enum class AudioType
{
    WAVE,
    NONE
};

enum class AudioChannels
{
    MONO,
    STEREO
};

template <class T>
class Audio
{
public:
    static AudioType str_to_type(std::string str);

    Audio();
    Audio(std::string file);

    bool load(std::string file);
    bool load(std::string file, AudioType type);
    bool load(std::vector<uint8_t> buf, AudioType type);

    AudioFmt get_format() const;
    AudioChannels get_channels() const;
    uint32_t get_sample_rate() const;
    uint32_t get_filesize() const;
    uint16_t get_bit_depth() const;
    uint16_t get_sample_freq() const;
    uint16_t get_block_alignment() const;
private:
    enum class Endian
    {
        LITTLE,
        BIG
    };

    enum class WavFmt
    {
        PCM = 0x0001,
        IEEEFloat = 0x0003,
        Alaw = 0x0006,
        MULaw = 0x0007,
        Extensible = 0xFFFE,
    };

    size_t get_chunk_index(std::vector<uint8_t> buffer, const std::string chunk, size_t index) const;

    uint16_t two_byte_int(std::vector<uint8_t> buffer, size_t index, Endian endian = Endian::LITTLE) const;
    uint32_t four_byte_int(std::vector<uint8_t> buffer, size_t index, Endian endian = Endian::LITTLE) const;

    std::string fmt_from_int(uint16_t fmt) const;

    AudioFmt format;
    AudioChannels channels;
    uint32_t sample_rate;
    uint32_t filesize;
    uint16_t bit_depth;
    uint16_t sample_freq;
    uint16_t block_alignment;
};
