#include <iostream>
#include <unordered_map>
#include <optional>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace utils
{
    
void trim(std::string& str)
{
    auto itr = std::remove_if(str.begin(), str.end(), [](char c) {
        return c == ' ' || c == '\n' || c == '\t' || c == 13;
    });
    str.erase(itr, str.end());
}

} // namespace utils

namespace traits
{

template <typename T>
struct packTrait
{
    std::string pack(const T& t)
    {
        return t;
    }

    std::optional<T> unpack(const std::string& data)
    {
        return data;
    }
};

template <>
struct packTrait<int>
{
    std::string pack(int i)
    {
        return std::to_string(i);
    }

    std::optional<int> unpack(const std::string& data)
    {
        int res;
        try {
            res = std::stoi(data.c_str());
        } catch(...) {
            return std::nullopt;
        }
        return res;
    }
};

template <>
struct packTrait<double>
{
    std::string pack(double val)
    {
        return std::to_string(val);
    }

    std::optional<double> unpack(const std::string& data)
    {
        double res;
        try {
            res = std::stof(data.c_str());
        } catch(...) {
            return std::nullopt;
        }
        return res;
    }
};

} // namespace traits

class ConfigReader
{
public:
    ConfigReader(std::string configPath): configPath_(std::move(configPath)) 
    {
        parse();
    }
    
    // навесить constraint?
    template<typename T> std::optional<T> get(const std::string& configName)
    {
        if (!configMap.contains(configName)) {
            return std::nullopt;
        }
        return traits::packTrait<T>().unpack(configMap[configName]);
    }

private:
    using key = std::string;
    using value = std::string;

    void parse()
    {
        std::ifstream file(configPath_);
        if (!file.is_open()) {
            throw std::logic_error("Bad config path");
        }
        std::string line;
        while (std::getline(file, line)) {
            if (auto data = parseLine(line, ':')) {
                auto [key_, value_] = std::move(*data);
                if (configMap.contains(key_)) {
                    throw std::logic_error("Duplicate key");
                }
                configMap[key_] = value_;
            } else {
                throw std::logic_error("Bad config settings");
            }
        }
    }
    
    std::optional<std::pair<key, value>> parseLine(const std::string& line, char sep)
    {
        key key_;
        value value_;
        std::stringstream str(line);
        std::getline(str, key_, sep);
        std::getline(str, value_, sep);

        utils::trim(key_);
        utils::trim(value_);

        if (key_.empty() || value_.empty()) {
            return std::nullopt;
        }
        return std::make_pair(std::move(key_), std::move(value_));
    }

private:
    std::string configPath_;
    std::unordered_map<key, value> configMap;
};

