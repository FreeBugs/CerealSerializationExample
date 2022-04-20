#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/chrono.hpp>
#include <chrono>
#include <iostream>

namespace demo {

class TemperatureSample {
public:
  TemperatureSample() = default;
  explicit TemperatureSample(double temperature) : m_temperature(temperature) {}
  TemperatureSample(std::chrono::system_clock::time_point time,
                    double temperature)
      : m_time(time), m_temperature(temperature) {}

  void Time(const std::chrono::system_clock::time_point &time) {
    m_time = time;
  }

  [[nodiscard]] std::chrono::system_clock::time_point Time() const {
    return m_time;
  }

  void Temperature(const double temperature) { m_temperature = temperature; }

  [[nodiscard]] double Temperature() const { return m_temperature; }

private:
  std::chrono::system_clock::time_point m_time =
      std::chrono::system_clock::now();
  double m_temperature = 0.0;
};

std::ostream &operator<<(std::ostream &os, const TemperatureSample &ts) {
  os << "Time: " << ts.Time().time_since_epoch().count()
     << "; Temperature: " << ts.Temperature() << std::endl;
  return os;
}

template <class Archive>
void save(Archive &archive, TemperatureSample const &obj) {
  archive(cereal::make_nvp("m_time", obj.Time()),
          cereal::make_nvp("m_temperature", obj.Temperature()));
}

template <class Archive> void load(Archive &archive, TemperatureSample &obj) {
  std::chrono::system_clock::time_point tmp_time{};
  double tmp_temp{};

  archive(cereal::make_nvp("m_time", tmp_time),
          cereal::make_nvp("m_temperature", tmp_temp));

  TemperatureSample tmp{tmp_time, tmp_temp};
  obj = tmp;
}

} // namespace

int main() {
  std::stringstream os{};

  {
    cereal::JSONOutputArchive ar{os};
    std::cout << "Serializing:" << std::endl;
    for (auto i = 0; i < 10; i++) {
      demo::TemperatureSample sample{static_cast<double>(i)};
      std::cout << sample << std::endl;
      ar(sample);
    }
  }

  {
    cereal::JSONInputArchive ar{os};
    std::cout << "Deserializing:" << std::endl;
    bool continue_reading = true;
    while (continue_reading) {
      demo::TemperatureSample sample{};
      try {
        ar(sample);
        std::cout << sample << std::endl;
      } catch(const cereal::RapidJSONException& e) {
        std::ignore = e;
        continue_reading = false;
      }
    }
  }

  std::cout << "The JSON file looked like this: " << std::endl
            << os.str() << std::endl;

  return 0;
}