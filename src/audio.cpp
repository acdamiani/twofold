#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <cassert>
#include <cstring>
#include <arpa/inet.h>

#include "audio.h"

template class Audio<double>;
template class Audio<float>;

constexpr size_t hash(const char* str, int32_t h = 0)
{
    return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
}

template <class T>
AudioType Audio<T>::str_to_type(std::string str)
{
    size_t str_h = hash(str.c_str());

    switch (str_h)
    {
        case hash("wav"):
            return AudioType::WAVE;
        default:
            return AudioType::NONE;
    }
}

template <class T>
Audio<T>::Audio()
{
    static_assert(std::is_floating_point<T>::value, "ERROR: Twofold does not support non floating point sample formats");

    format = AudioFmt::NOT_LOADED;
}

template <class T>
Audio<T>::Audio(std::string file)
    : Audio<T>()
{
   load(file); 
}

template <class T>
bool Audio<T>::load(std::string file)
{
    // determine file type from extension
    // TODO: use file information rather than extension

    std::string extension = file.substr(file.find_last_of(".") + 1);

    AudioType file_t = str_to_type(extension);

    if (file_t == AudioType::NONE)
    {
        printf("ERROR: File is not of supported type (currently supported: WAVE)");
        return false;
    }

    return load(file, file_t);
}

template <class T>
bool Audio<T>::load(std::string file, AudioType type)
{
    std::ifstream infile(file, std::ios_base::binary);

    if (!infile.good())
    {
        printf("ERROR: File doesn't exist or otherwise can't load file %s\n", file.c_str());
        return false;
    }

    std::vector<uint8_t> buffer;

    infile.unsetf(std::ios::skipws);
    infile.seekg(0, std::ios::end);
    size_t len = infile.tellg();
    infile.seekg(0, std::ios::beg);

    buffer.resize(len);

    infile.read(reinterpret_cast<char*>(buffer.data()), len);

    if (infile.gcount() != len)
    {
        printf("ERROR: Could not read entire file %s\n", file.c_str());
        return false;
    }

    return load(buffer, type);
}

template <class T>
bool Audio<T>::load(std::vector<uint8_t> buffer, AudioType type)
{
    switch (type)
    {
        case AudioType::WAVE: {
            std::string fmt_specifier(buffer.begin(), buffer.begin() + 4);
            std::string ft_specifier(buffer.begin() + 8, buffer.begin() + 12);

            printf("Found format specifier %s\n", fmt_specifier.c_str());
            printf("Found file type specifier %s\n", ft_specifier.c_str());

            size_t i_fmt = get_chunk_index(buffer, "fmt ", 12);
            size_t i_data = get_chunk_index(buffer, "data", 12);

            printf("Found fmt chunk at byte %d\n", i_fmt);
            printf("Found data chunk at byte %d\n", i_data);

            if (i_data == -1 || i_fmt == -1 || fmt_specifier != "RIFF" || ft_specifier != "WAVE")
            {
                printf("ERROR: Invalid WAVE buffer supplied\n");
                return false;
            }

            uint16_t audio_fmt = two_byte_int(buffer, i_fmt + 8);
            uint16_t num_channels = two_byte_int(buffer, i_fmt + 10);

            printf("Found wave format type 0x%03x (%s)\n", audio_fmt, fmt_from_int(audio_fmt).c_str());
            printf("Found %d channel(s)\n", num_channels);

            return true;
        }
        case AudioType::NONE: {
            printf("ERROR: File is not of supported type (currently supported: WAVE)");
            return false;
        }
    }

    return false;
}

template <class T>
std::string Audio<T>::fmt_from_int(uint16_t fmt) const
{
    switch (fmt)
    {
        case 0x0001:
            return "PCM";
        case 0x0003:
            return "IEEEFloat";
        case 0x0006:
            return "Alaw";
        case 0x0007:
            return "MULaw";
        case 0xFFFE:
            return "Extensible";
        default:
            return "INVALID";
    }
}

template <class T>
uint16_t Audio<T>::two_byte_int(std::vector<uint8_t> buffer, size_t index, Endian endian) const
{
    if (endian == Endian::BIG)
        return buffer[index] | (buffer[index + 1] << 8);
    else if (endian == Endian::LITTLE)
        return (buffer[index + 1] << 8) | buffer[index];

    return 0;
}

template <class T>
uint32_t Audio<T>::four_byte_int(std::vector<uint8_t> buffer, size_t index, Endian endian) const
{
    if (endian == Endian::BIG)
        return buffer[index] | (buffer[index + 1] << 8) | (buffer[index + 2] << 16) | (buffer[index + 3] << 24);
    else if (endian == Endian::LITTLE)
        return (buffer[index + 3] << 24) | (buffer[index + 2] << 16) | (buffer[index + 1] << 8) | buffer[index];

    return 0;
}

template <class T>
size_t Audio<T>::get_chunk_index(std::vector<uint8_t> buffer, const std::string chunk, size_t index) const
{
    constexpr size_t len = 4;

    if (chunk.size() != len)
    {
        assert(false && "Invalid chunk header string");
        return -1;
    }

    size_t i = index;

    while (i < buffer.size() - len)
    {
        if (std::memcmp(&buffer[i], chunk.data(), len) == 0)
            return i;

        i += len;
        uint32_t n = (buffer[i + 3] << 24) | (buffer[i + 2] << 16) | (buffer[i + 1] << 8) | buffer[i];
        i += (len + n);
    }

    return -1;
}
