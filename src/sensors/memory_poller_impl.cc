#include "memory_poller.h"

#include "utils.h"

#include <cctype>
#include <fstream>
#include <map>
#include <memory>
#include <string>

namespace
{

    class MemoryPollerImpl : public MemoryPoller
    {
    public:
        explicit MemoryPollerImpl(const std::string& procMemInfoPath):
            _procMemInfoPath(procMemInfoPath)
        {
        }

        std::map<std::string, MemorySample> Poll() override
        {
            std::ifstream file(_procMemInfoPath);
            if (!file)
                return {};

            long long total = -1;
            long long free = -1;
            long long buffers = -1;

            std::string line;
            while (std::getline(file, line))
            {
                std::size_t colon = line.find(':');
                if (colon == std::string::npos)
                    continue;
                std::string key = Trim(line.substr(0, colon));
                std::string valuePart = Trim(line.substr(colon + 1));
                if (valuePart.empty())
                    continue;

                std::size_t idx = 0;
                while (idx < valuePart.size() &&
                       std::isspace(static_cast<unsigned char>(valuePart[idx])))
                    idx++;
                std::size_t numEnd = idx;
                while (
                    numEnd < valuePart.size() &&
                    std::isdigit(static_cast<unsigned char>(valuePart[numEnd])))
                    numEnd++;
                if (numEnd == idx)
                    continue;
                std::string numStr = valuePart.substr(idx, numEnd - idx);
                try
                {
                    long long v = std::stoll(numStr);
                    if (key == "MemTotal")
                        total = v;
                    else if (key == "MemFree")
                        free = v;
                    else if (key == "Buffers")
                        buffers = v;
                }
                catch (...)
                {
                    continue;
                }

                if (total != -1 && free != -1 && buffers != -1)
                    break;
            }

            if (total == -1 || free == -1 || buffers == -1)
                return {};

            long long usedKb = total - free - buffers;
            if (usedKb < 0)
                usedKb = 0;

            long long totalBytes = total * 1024;
            long long usedBytes = usedKb * 1024;

            MemorySample sample{totalBytes, usedBytes};
            std::map<std::string, MemorySample> result;
            result["mem"] = sample;
            return result;
        }

    private:
        std::string _procMemInfoPath;
    };

} // namespace

std::unique_ptr<MemoryPoller> CreateMemoryPoller(
    const std::string& procMemInfoPath)
{
    return std::make_unique<MemoryPollerImpl>(procMemInfoPath);
}
