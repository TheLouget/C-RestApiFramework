#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>

class Serializer {
public:
    static std::string serializeToJson(const std::unordered_map<std::string, std::string>& data) {
        nlohmann::json j(data);
        return j.dump();
    }


    static std::unordered_map<std::string, std::string> deserializeFromJson(const std::string& jsonStr) {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return j.get<std::unordered_map<std::string, std::string>>();
    }

    static std::string serializeToXml(const std::unordered_map<std::string, std::string>& data) {
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        for (const auto& [key, value] : data) {
            pugi::xml_node node = root.append_child(key.c_str());
            node.text() = value.c_str();
        }
        std::ostringstream oss;
        doc.save(oss);
        return oss.str();
    }

    static std::unordered_map<std::string, std::string> deserializeFromXml(const std::string& xmlStr) {
        std::unordered_map<std::string, std::string> data;
        pugi::xml_document doc;
        if (doc.load_string(xmlStr.c_str())) {
            for (pugi::xml_node node : doc.child("root").children()) {
                data[node.name()] = node.child_value();
            }
        }
        return data;
    }
};

#endif
