#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <string>
#include <vector>

enum class AudioFmt { ERROR, NOT_LOADED, WAVE };

enum class AudioType { WAVE, NONE };

enum SmpFmt {
  PCM = 0x0001,
  IEEE_FLOAT = 0x0003,
  A_LAW = 0x0006,
  MU_LAW = 0x0007,
  EXTENSIBLE = 0xFFFE,
};

template <class T>
class Audio {
 public:
  static AudioType str_to_type(std::string str);

  Audio();
  Audio(std::string file);

  std::vector<T> samples() const;
  std::vector<T> samples(uint8_t channel) const;

  bool load(std::string file);
  bool load(std::string file, AudioType type);
  bool load(std::vector<uint8_t> buf, AudioType type);

  void reset();

  bool mono() const;
  bool stereo() const;

  AudioFmt format() const;
  SmpFmt sample_format() const;
  double length() const;
  uint16_t channels() const;
  uint32_t sample_rate() const;
  uint32_t filesize() const;
  uint16_t bit_depth() const;
  uint16_t sample_freq() const;
  uint16_t block_alignment() const;

 private:
  enum Endian { LITTLE, BIG };

  std::vector<std::vector<T>> samples_;

  size_t get_chunk_index(std::vector<uint8_t> buffer, const std::string chunk,
                         size_t index) const;

  uint16_t two_byte_int(std::vector<uint8_t>& buffer, size_t index,
                        Endian endian = Endian::LITTLE) const;
  uint32_t four_byte_int(std::vector<uint8_t>& buffer, size_t index,
                         Endian endian = Endian::LITTLE) const;

  constexpr T eight_bit_samp(uint8_t samp) const;
  constexpr T sixteen_bit_samp(int16_t samp) const;
  constexpr T twenty_four_bit_samp(int32_t samp) const;
  constexpr T thirty_two_bit_samp(int32_t samp) const;

  std::string fmt_from_int(uint16_t fmt) const;

  AudioFmt format_;
  SmpFmt sample_format_;
  uint16_t channels_;
  uint32_t sample_rate_;
  uint32_t filesize_;
  uint16_t bit_depth_;
  uint16_t sample_freq_;
  uint16_t block_alignment_;
};
