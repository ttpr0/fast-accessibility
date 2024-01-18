

#include "./node_ordering.h"

MappingBuilder::MappingBuilder(int node_count) : indices(node_count)
{
    for (int i = 0; i < this->indices.size(); i++) {
        indices[i] = {i, 0};
    }
}
void MappingBuilder::updateOrdering(std::vector<int>& order_values)
{
    if (order_values.size() != this->indices.size()) {
        return;
    }
    for (int i = 0; i < this->indices.size(); i++) {
        int id = this->indices[i][0];
        this->indices[i][1] = order_values[id];
    }

    std::stable_sort(this->indices.begin(), this->indices.end(), [](const std::array<int, 2>& a, const std::array<int, 2>& b) {
        int val_a = a[1];
        int val_b = b[1];
        return val_a < val_b;
    });
}
void MappingBuilder::updateOrdering(std::vector<int>& order_values, _IDMapping& id_mapping, bool to_target)
{
    if (order_values.size() != this->indices.size()) {
        return;
    }
    for (int i = 0; i < this->indices.size(); i++) {
        int id = this->indices[i][0];
        int m_id;
        if (to_target) {
            m_id = id_mapping.get_target(id);
        } else {
            m_id = id_mapping.get_source(id);
        }
        this->indices[i][1] = order_values[m_id];
    }

    std::stable_sort(this->indices.begin(), this->indices.end(), [](const std::array<int, 2>& a, const std::array<int, 2>& b) {
        int val_a = a[1];
        int val_b = b[1];
        return val_a < val_b;
    });
}
std::vector<int> MappingBuilder::buildMapping() const
{
    std::vector<int> mapping(this->indices.size());
    for (int i = 0; i < this->indices.size(); i++) {
        int new_id = i;
        int id = this->indices[i][0];
        mapping[id] = new_id;
    }
    return mapping;
}
