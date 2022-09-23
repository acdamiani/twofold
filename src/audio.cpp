#include "audio.h"
#include <arpa/inet.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

template class Audio<double>;
template class Audio<float>;
template class Audio<long double>;

constexpr size_t hash(char const* str, int32_t h = 0) {
  return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}

template <class T>
AudioType Audio<T>::str_to_type(std::string str) {
  size_t str_h = hash(str.c_str());

  switch (str_h) {
    case hash("wav"):
      return AudioType::WAVE;
    default:
      return AudioType::NONE;
  }
}

template <class T>
Audio<T>::Audio() {
  static_assert(
      std::is_floating_point<T>::value,
      "ERROR: Twofold does not support non floating point sample formats");

  format_ = AudioFmt::NOT_LOADED;
}

template <class T>
Audio<T>::Audio(std::string file) : Audio<T>() {
  load(file);
}

template <class T>
bool Audio<T>::load(std::string file) {
  // determine file type from extension
  // TODO: use file information rather than extension

  std::string extension = file.substr(file.find_last_of(".") + 1);

  AudioType file_t = str_to_type(extension);

  if (file_t == AudioType::NONE) {
    printf("ERROR: File is not of supported type (currently supported: WAVE)");
    return false;
  }

  return load(file, file_t);
}

template <class T>
bool Audio<T>::load(std::string file, AudioType type) {
  std::ifstream infile(file, std::ios_base::binary);

  if (!infile.good()) {
    printf("ERROR: File doesn't exist or otherwise can't load file %s\n",
           file.c_str());
    return false;
  }

  std::vector<uint8_t> buffer;

  infile.unsetf(std::ios::skipws);
  infile.seekg(0, std::ios::end);
  size_t len = infile.tellg();
  infile.seekg(0, std::ios::beg);

  buffer.resize(len);

  infile.read(reinterpret_cast<char*>(buffer.data()), len);

  if (infile.gcount() != len) {
    printf("ERROR: Could not read entire file %s\n", file.c_str());
    return false;
  }

  return load(buffer, type);
}

template <class T>
bool Audio<T>::load(std::vector<uint8_t> buffer, AudioType type) {
  switch (type) {
    case AudioType::WAVE: {
      std::string fmt_specifier(buffer.begin(), buffer.begin() + 4);
      std::string ft_specifier(buffer.begin() + 8, buffer.begin() + 12);

      printf("Found format specifier %s\n", fmt_specifier.c_str());
      printf("Found file type specifier %s\n", ft_specifier.c_str());

      size_t i_fmt = get_chunk_index(buffer, "fmt ", 12);
      size_t i_data = get_chunk_index(buffer, "data", 12);

      printf("Found fmt chunk at byte %d\n", i_fmt);
      printf("Found data chunk at byte %d\n", i_data);

      if (i_data == -1 || i_fmt == -1 || fmt_specifier != "RIFF" ||
          ft_specifier != "WAVE") {
        printf("ERROR: Invalid WAVE buffer supplied\n");
        return false;
      }

      uint16_t audio_fmt = two_byte_int(buffer, i_fmt + 8);
      channels_ = two_byte_int(buffer, i_fmt + 10);
      sample_rate_ = (uint32_t)four_byte_int(buffer, i_fmt + 12);
      uint32_t byte_rate = four_byte_int(buffer, i_fmt + 16);
      uint16_t block_byte_rate = two_byte_int(buffer, i_fmt + 20);
      bit_depth_ = two_byte_int(buffer, i_fmt + 22);

      printf("Found wave format type 0x%03x (%s)\n", audio_fmt,
             fmt_from_int(audio_fmt).c_str());
      printf("Found %d channel(s)\n", channels_);
      printf("Found sample rate %d Hz\n", sample_rate_);
      printf("Found byte rate %d B/s\n", byte_rate);
      printf("Found block byte rate %d B/block\n", block_byte_rate);
      printf("Found bit depth %d Bit\n", bit_depth_);

      uint16_t bytes_per_sample = static_cast<uint16_t>(bit_depth_) / 8;

      if (audio_fmt != SmpFmt::PCM && audio_fmt != SmpFmt::IEEE_FLOAT &&
          audio_fmt != SmpFmt::EXTENSIBLE) {
        printf(
            "ERROR: This WAVE file contains a sampling format currently not "
            "supported");
        return false;
      }

      if (channels_ < 1 || channels_ > 128) {
        printf("ERROR: This WAVE file contains an invalid number of channels");
        return false;
      }

      if (byte_rate != static_cast<uint32_t>(
                           (channels_ * sample_rate_ * bit_depth_) / 8) ||
          block_byte_rate != (channels_ * bytes_per_sample)) {
        printf(
            "ERROR: Header data for this WAVE file inconsistent or corrupted");
        return false;
      }

      if (bit_depth_ != 8 && bit_depth_ != 16 && bit_depth_ != 24 &&
          bit_depth_ != 32) {
        printf("ERROR: This WAVE file has an invalid bit depth");
        return false;
      }

      int32_t dat_chunk_size = four_byte_int(buffer, i_data + 4);
      int32_t num_samples = dat_chunk_size / (channels_ * bit_depth_ / 8);
      int32_t i_start = i_data + 8;

      samples_.resize(channels_);

      printf("%zu\n", num_samples);

      for (size_t i = 0; i < num_samples; i++) {
        for (uint16_t c = 0; c < channels_; c++) {
          size_t i_samp =
              i_start + (block_byte_rate * i) + c * bytes_per_sample;

          if ((i_samp + (bit_depth_ / 8) - 1) >= buffer.size()) {
            printf(
                "ERROR: File metadata indicates more samples in the file data "
                "than there are");
            return false;
          }

          switch (bit_depth_) {
            case 8: {
              T samp = eight_bit_samp(buffer[i_samp]);
              samples_[c].push_back(samp);
              break;
            }
            case 16: {
              T samp = sixteen_bit_samp(two_byte_int(buffer, i_samp));
              samples_[c].push_back(samp);
              break;
            }
            case 24: {
              int32_t int_samp = 0;
              int_samp = (buffer[i_samp + 2] << 16) |
                         (buffer[i_samp + 1] << 8) | buffer[i_samp];

              T samp = twenty_four_bit_samp(int_samp);
              samples_[c].push_back(samp);
              break;
            }
            case 32: {
              T samp = thirty_two_bit_samp(four_byte_int(buffer, i_samp));
              samples_[c].push_back(samp);
              break;
            }
            default: {
              assert(false);
            }
          }
        }
      }

      return true;
    }
    case AudioType::NONE: {
      printf(
          "ERROR: File is not of supported type (currently supported: WAVE)");
      return false;
    }
  }

  return false;
}

template <class T>
void Audio<T>::reset() {
  format_ = AudioFmt::NOT_LOADED;
  channels_ = 0;
  sample_rate_ = 0;
  filesize_ = 0;
  bit_depth_ = 0;
  sample_freq_ = 0;
  block_alignment_ = 0;
  samples_.clear();
}

template <class T>
std::vector<T> Audio<T>::samples() const {
  if (samples_.size() > 1 &&
      std::adjacent_find(
          samples_.begin(), samples_.end(),
          [](std::vector<T> const& lhs, std::vector<T> const& rhs) {
            return lhs.size() == rhs.size();
          }) == samples_.cend()) {
    printf("ERROR: Invalid sample buffer\n");
    return std::vector<T>();
  }

  size_t size = samples_[0].size();

  std::vector<T> ret;

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < samples_.size(); j++) {
      ret.push_back(samples_[j][i]);
    }
  }

  printf("1: %zu\n", ret.size());
  printf("2: %zu\n", samples_[0].size());

  return ret;
}

template <class T>
bool Audio<T>::mono() const {
  return channels_ == 1;
}

template <class T>
bool Audio<T>::stereo() const {
  return channels_ == 2;
}

template <class T>
AudioFmt Audio<T>::format() const {
  return format_;
}

template <class T>
double Audio<T>::length() const {
  return (double)samples_.at(0).size() / (double)sample_rate_;
}

template <class T>
SmpFmt Audio<T>::sample_format() const {
  return sample_format_;
}

template <class T>
uint16_t Audio<T>::channels() const {
  return channels_;
}

template <class T>
uint32_t Audio<T>::sample_rate() const {
  return sample_rate_;
}

template <class T>
uint32_t Audio<T>::filesize() const {
  return filesize_;
}

template <class T>
uint16_t Audio<T>::bit_depth() const {
  return bit_depth_;
}

template <class T>
uint16_t Audio<T>::sample_freq() const {
  return sample_freq_;
}

template <class T>
uint16_t Audio<T>::block_alignment() const {
  return block_alignment_;
}

template <class T>
std::string Audio<T>::fmt_from_int(uint16_t fmt) const {
  switch (fmt) {
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
constexpr T Audio<T>::eight_bit_samp(uint8_t samp) const {
  return static_cast<T>(samp - std::numeric_limits<int8_t>::max()) /
         static_cast<T>(std::numeric_limits<int8_t>::max());
}

template <class T>
constexpr T Audio<T>::sixteen_bit_samp(int16_t samp) const {
  return static_cast<T>(samp) /
         static_cast<T>(std::numeric_limits<int16_t>::max());
}

template <class T>
constexpr T Audio<T>::twenty_four_bit_samp(int32_t samp) const {
  return static_cast<T>(samp & 0x800000 ? samp | ~0xFFFFFF : samp) /
         static_cast<T>(8388607.);
}

template <class T>
constexpr T Audio<T>::thirty_two_bit_samp(int32_t samp) const {
  return sample_format_ == SmpFmt::IEEE_FLOAT
             ? static_cast<T>(reinterpret_cast<float&>(samp))
             : static_cast<T>(samp) /
                   static_cast<T>(std::numeric_limits<int32_t>::max());
}

template <class T>
uint16_t Audio<T>::two_byte_int(std::vector<uint8_t>& buffer, size_t index,
                                Endian endian) const {
  if (endian == Endian::BIG)
    return buffer[index] | (buffer[index + 1] << 8);
  else if (endian == Endian::LITTLE)
    return (buffer[index + 1] << 8) | buffer[index];

  return 0;
}

template <class T>
uint32_t Audio<T>::four_byte_int(std::vector<uint8_t>& buffer, size_t index,
                                 Endian endian) const {
  if (endian == Endian::BIG)
    return buffer[index] | (buffer[index + 1] << 8) |
           (buffer[index + 2] << 16) | (buffer[index + 3] << 24);
  else if (endian == Endian::LITTLE)
    return (buffer[index + 3] << 24) | (buffer[index + 2] << 16) |
           (buffer[index + 1] << 8) | buffer[index];

  return 0;
}

template <class T>
size_t Audio<T>::get_chunk_index(std::vector<uint8_t> buffer,
                                 const std::string chunk, size_t index) const {
  constexpr size_t len = 4;

  if (chunk.size() != len) {
    assert(false && "Invalid chunk header string");
    return -1;
  }

  size_t i = index;

  while (i < buffer.size() - len) {
    if (std::memcmp(&buffer[i], chunk.data(), len) == 0) return i;

    i += len;
    uint32_t n = (buffer[i + 3] << 24) | (buffer[i + 2] << 16) |
                 (buffer[i + 1] << 8) | buffer[i];
    i += (len + n);
  }

  return -1;
}
