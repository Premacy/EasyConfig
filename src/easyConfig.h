#include <iostream>
#include <unordered_map>
#include <optional>
#include <string>
#include <fstream>
#include <sstream>

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
}
//TODO сделать запрет на компиляцию при отсутсвии реализации
template <typename T>
struct packTrait
{
    std::string pack(const T&)
    {
        return "";
    }

    std::optional<T> unpack(const std::string&)
    {
        return std::nullopt;
    }
};

template <>
struct packTrait<int>
{
    std::string pack(const int& i)
    {
        return std::to_string(i);
    }

    std::optional<int> unpack(const std::string& data)
    {
        return std::atoi(data.c_str());
    }
};

template <>
struct packTrait<std::string>
{
    std::string pack(const std::string& str)
    {
        return str;
    }

    std::optional<std::string> unpack(const std::string& data)
    {
        return data;
    }
};

class ConfigReader
{
public:
    ConfigReader(std::string configPath): configPath_(std::move(configPath)) 
    {
        parse();
    }
    
    template<typename T> std::optional<T> get(const std::string& configName)
    {
        if (!configMap.contains(configName)) {
            return std::nullopt;
        }
        return packTrait<T>().unpack(configMap[configName]);
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
                auto [key_, value_] = *data;
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
        //REFACTOR ME
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

