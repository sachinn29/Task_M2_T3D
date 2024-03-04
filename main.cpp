#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <sstream>

struct TrafficRecord {
  std::time_t timestamp;
  int lightId;
  int vehicleCount;
};

bool operator<(const TrafficRecord& left, const TrafficRecord& right) {
  return left.timestamp < right.timestamp;
}

int main() {
  const std::string dataFile = "traffic_data.txt";
  const int topResults = 5;
  std::ifstream inputStream(dataFile);

  // Read data into a vector
  std::vector<TrafficRecord> records;
  std::string line;
  while (std::getline(inputStream, line)) {
    std::stringstream ss(line);
    std::string timestampStr, idStr, countStr;
    std::getline(ss, timestampStr, ',');
    std::getline(ss, idStr, ',');
    std::getline(ss, countStr, ',');
    TrafficRecord record{
        .timestamp = std::stol(timestampStr),
        .lightId = std::stoi(idStr),
        .vehicleCount = std::stoi(countStr)};
    records.push_back(record);
  }

  // Sort data by timestamp
  std::sort(records.begin(), records.end());
  std::time_t currentHour = records.front().timestamp / 3600 * 3600;

  // Process data hourly
  std::unordered_map<int, int> trafficCounts;
  for (const TrafficRecord& record : records) {
    if (record.timestamp - currentHour >= 3600) {
      // Report top N congested traffic lights

      // Convert unordered_map to a vector for sorting
      std::vector<std::pair<int, int>> lightTotals(trafficCounts.begin(), trafficCounts.end());
      std::sort(lightTotals.begin(), lightTotals.end(),
                [](const std::pair<int, int>& left, const std::pair<int, int>& right) {
                  return left.second > right.second;
                });

      std::cout << "Hour: " << std::asctime(std::localtime(&currentHour)) << std::endl;

      for (int i = 0; i < std::min(topResults, static_cast<int>(lightTotals.size())); i++) {
        std::cout << "Traffic Light #" << lightTotals[i].first << ": " << lightTotals[i].second << " vehicles" << std::endl;
      }
      std::cout << "------------------------" << std::endl;

      // Reset counts for next hour
      trafficCounts.clear();
      currentHour = record.timestamp - (record.timestamp % 3600);
    }
    trafficCounts[record.lightId] += record.vehicleCount;
  }

  // Report top N congested traffic lights for last hour
  std::vector<std::pair<int, int>> lightTotals(trafficCounts.begin(), trafficCounts.end());
  std::sort(lightTotals.begin(), lightTotals.end(),
            [](const std::pair<int, int>& left, const std::pair<int, int>& right) {
              return left.second > right.second;
            });

  std::cout << "Hour: " << std::asctime(std::localtime(&currentHour)) << std::endl;

  for (int i = 0; i < std::min(topResults, static_cast<int>(lightTotals.size())); i++) {
    std::cout << "Traffic Light #" << lightTotals[i].first << ": " << lightTotals[i].second << " vehicles" << std::endl;
  }

  inputStream.close();

  return 0;
}
