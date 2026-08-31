#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "easyConfig.h"

namespace
{
class TemporaryConfig
{
public:
    explicit TemporaryConfig(const std::string& contents)
        : path_(std::filesystem::temp_directory_path() / "easy_config_test.txt")
    {
        std::ofstream file(path_);
        file << contents;
    }

    ~TemporaryConfig()
    {
        std::error_code error;
        std::filesystem::remove(path_, error);
    }

    std::string path() const
    {
        return path_.string();
    }

private:
    std::filesystem::path path_;
};
} // namespace

TEST(ConfigReaderTest, ReadsSupportedTypes)
{
    ConfigReader config(TEST_CONFIG_PATH);

    EXPECT_EQ(config.get<int>("someField"), 2);
    EXPECT_EQ(config.get<std::string>("someField2"), "Hello");
    EXPECT_EQ(config.get<double>("doubleField"), 2.5);
}

TEST(ConfigReaderTest, ReturnsEmptyOptionalForMissingKey)
{
    ConfigReader config(TEST_CONFIG_PATH);

    EXPECT_EQ(config.get<std::string>("missing"), std::nullopt);
}

TEST(ConfigReaderTest, ThrowsForMissingFile)
{
    const auto missing =
        std::filesystem::temp_directory_path() / "easy_config_missing_file.txt";
    std::error_code error;
    std::filesystem::remove(missing, error);

    EXPECT_THROW(ConfigReader config(missing.string()), std::logic_error);
}

TEST(ConfigReaderTest, ThrowsForDuplicateKey)
{
    TemporaryConfig configFile("key: first\nkey: second\n");

    EXPECT_THROW(ConfigReader config(configFile.path()), std::logic_error);
}

TEST(ConfigReaderTest, ThrowsForMalformedLine)
{
    TemporaryConfig configFile("missing_separator\n");

    EXPECT_THROW(ConfigReader config(configFile.path()), std::logic_error);
}
