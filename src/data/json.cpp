#include "data/json.hpp"

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

fem::data::JsonNode::JsonNode() : type_(Type::NONE) {}
fem::data::JsonNode::JsonNode(const Type& type) : type_(type) {}
fem::data::JsonNode::JsonNode(const int& value)
    : type_(Type::NUMBER), number_(value) {}
fem::data::JsonNode::JsonNode(const double& value)
    : type_(Type::NUMBER), number_(value) {}
fem::data::JsonNode::JsonNode(const std::string& value)
    : type_(Type::STRING), string_(value) {}
fem::data::JsonNode::JsonNode(const char* value)
    : type_(Type::STRING), string_(value) {}
fem::data::JsonNode::JsonNode(const bool& value)
    : type_(Type::BOOLEAN), boolean_(value) {}
fem::data::JsonNode::JsonNode(const std::map<std::string, JsonNode>& value)
    : type_(Type::OBJECT), object_(value) {}
fem::data::JsonNode::JsonNode(const std::vector<JsonNode>& value)
    : type_(Type::ARRAY), array_(value) {}
fem::data::JsonNode::JsonNode(const JsonNode& copy)
    : type_(copy.type_),
      number_(copy.number_),
      string_(copy.string_),
      boolean_(copy.boolean_),
      array_(copy.array_),
      object_(copy.object_) {}

fem::data::JsonNode& fem::data::JsonNode::operator=(const int& value) {
  type_ = NUMBER;
  number_ = value;
  return *this;
}
fem::data::JsonNode& fem::data::JsonNode::operator=(const double& value) {
  type_ = NUMBER;
  number_ = value;
  return *this;
}
fem::data::JsonNode& fem::data::JsonNode::operator=(const std::string& value) {
  type_ = STRING;
  string_ = value;
  return *this;
}
fem::data::JsonNode& fem::data::JsonNode::operator=(const char* value) {
  type_ = STRING;
  string_ = std::string(value);
  return *this;
}
fem::data::JsonNode& fem::data::JsonNode::operator=(const bool& value) {
  type_ = BOOLEAN;
  boolean_ = value;
  return *this;
}
fem::data::JsonNode& fem::data::JsonNode::operator=(
    const std::map<std::string, JsonNode>& value) {
  type_ = OBJECT;
  object_ = value;
  return *this;
}
fem::data::JsonNode& fem::data::JsonNode::operator=(
    const std::vector<JsonNode>& value) {
  type_ = ARRAY;
  array_ = value;
  return *this;
}

void fem::data::JsonNode::prepend(const JsonNode& value) {
  if (type_ != ARRAY) {
    throw std::logic_error("Json node type must be array to prepend.");
  } else {
    array_.insert(array_.begin(), value);
  }
}

void fem::data::JsonNode::insert(std::size_t pos, const JsonNode& value) {
  if (type_ != ARRAY) {
    throw std::logic_error("Json node type must be array to insert.");
  } else {
    array_.insert(array_.begin() + pos, value);
  }
}

void fem::data::JsonNode::append(const JsonNode& value) {
  if (type_ != ARRAY) {
    throw std::logic_error("Json node type must be array to append.");
  } else {
    array_.push_back(value);
  }
}

void fem::data::JsonNode::insert(const std::string& key,
                                 const JsonNode& value) {
  if (type_ != OBJECT) {
    throw std::logic_error("Json node type must be object to insert.");
  } else {
    object_.insert({{key, value}});
  }
}

void fem::data::JsonNode::clear() {
  if (type_ == OBJECT) {
    object_.clear();
  } else if (type_ == ARRAY) {
    array_.clear();
  } else {
    throw std::logic_error("Json node type must be object or array to clear.");
  }
}
std::size_t fem::data::JsonNode::size() {
  if (type_ == OBJECT) {
    return object_.size();
  } else if (type_ == ARRAY) {
    return array_.size();
  } else {
    throw std::logic_error(
        "Json node type must be object or array to get size.");
  }
}

fem::data::JsonNode& fem::data::JsonNode::operator[](const std::size_t& pos) {
  if (type_ != ARRAY) {
    throw std::logic_error("Json node type must be array to subscript.");
  } else {
    return array_[pos];
  }
}

fem::data::JsonNode& fem::data::JsonNode::operator[](const std::string& key) {
  if (type_ != OBJECT) {
    throw std::logic_error("Json node type must be object to use key.");
  } else {
    return object_[key];
  }
}

std::string fem::data::JsonNode::dump() const {
  switch (type_) {
    case NONE: {
      return "null";
    }
    case NUMBER: {
      std::stringstream ss;
      ss << number_;
      return ss.str();
    }
    case STRING: {
      return "\"" + string_ + "\"";
    }
    case BOOLEAN: {
      return (boolean_) ? "true" : "false";
    }
    case ARRAY: {
      std::string dump = "[";
      for (std::size_t i = 0; i < array_.size(); ++i) {
        dump += array_[i].dump();
        if (i != array_.size() - 1) {
          dump += ',';
        }
      }
      dump += ']';
      return dump;
    }
    case OBJECT: {
      std::string dump = "{";
      std::size_t index = 0;
      for (auto& key_value : object_) {
        dump += "\"" + key_value.first + "\":" + key_value.second.dump();
        if (index != object_.size() - 1) {
          dump += ',';
        }
        index++;
      }
      dump += "}";
      return dump;
    }
  }
}

std::ostream& fem::data::operator<<(std::ostream& out, const JsonNode& rhs) {
  out << rhs.dump();
  return out;
}
