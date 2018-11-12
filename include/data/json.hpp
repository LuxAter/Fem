#ifndef FEM_DATA_JSON_HPP_
#define FEM_DATA_JSON_HPP_

#include <map>
#include <string>
#include <vector>

namespace fem {
namespace data {
  class JsonNode {
   public:
    enum Type { NONE, NUMBER, STRING, BOOLEAN, OBJECT, ARRAY };

    JsonNode();
    JsonNode(const Type& type);
    JsonNode(const int& value);
    JsonNode(const double& value);
    JsonNode(const std::string& value);
    JsonNode(const char* value);
    JsonNode(const bool& value);
    JsonNode(const std::map<std::string, JsonNode>& value);
    JsonNode(const std::vector<JsonNode>& value);
    JsonNode(const JsonNode& copy);

    JsonNode& operator=(const int& value);
    JsonNode& operator=(const double& value);
    JsonNode& operator=(const std::string& value);
    JsonNode& operator=(const char* value);
    JsonNode& operator=(const bool& value);
    JsonNode& operator=(const std::map<std::string, JsonNode>& value);
    JsonNode& operator=(const std::vector<JsonNode>& value);

    void prepend(const JsonNode& value);
    void insert(std::size_t pos, const JsonNode& value);
    void append(const JsonNode& value);

    void insert(const std::string& key, const JsonNode& value);

    void clear();
    std::size_t size();

    JsonNode& operator[](const std::size_t& pos);
    JsonNode& operator[](const std::string& key);

    std::string dump() const;

   private:
    Type type_;
    double number_;
    std::string string_;
    bool boolean_;
    std::vector<JsonNode> array_;
    std::map<std::string, JsonNode> object_;
  };
  std::ostream& operator<<(std::ostream& out, const JsonNode& rhs);

}  // namespace data
}  // namespace fem

#endif  // FEM_DATA_JSON_HPP_
